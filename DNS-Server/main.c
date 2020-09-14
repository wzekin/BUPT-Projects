#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <uv.h>

#include "dns_port.h"
#include "file_reader.h"
#include "hashmap.h"
#include "linked_hashmap.h"
#include "log.h"
#include "utils.h"

#define SERVER_PORT 53
#define BUFF_LEN 1024

struct timer_context {
  uv_udp_t* client;
  unsigned short int* id;
};

struct idle_context {
  uv_udp_t* server;
  uv_timer_t* LRU_clear;
};

struct server_context {
  struct Hashmap* hosts;
  struct Hashmap* ban_hosts;
  struct Hashmap* forwarding_client;
  struct LinkedHashmap* LRU;
};

struct client_context {
  uv_udp_t* server;
  uv_buf_t* send_buf;
  unsigned short origin_id;
  unsigned short forwarding_id;
  struct DNSPort* port;
  struct sockaddr* addr;
};

int stop = 0;
struct sockaddr_in recv_addr;
struct sockaddr_in send_addr;

void uv_buf_t_free(void* data) {
  uv_buf_t* buf = data; 
  free(buf->base);
  free(buf);
}

//释放资源
static void close_(uv_handle_t* handle) {
  free(handle);
}

static void close_cb_(uv_handle_t* handle) {
  struct client_context* c = handle->data;
  free(c->port->header);
  free(c->port->question);
  free(c->port);
  free(c->send_buf->base);
  free(c->send_buf);
  free(c->addr);
  free(c);
  free(handle);
}

static void close_cb(uv_handle_t* handle) {
  struct client_context* c = handle->data;
  free(c->port->header);
  free(c->port);
  free(c->send_buf->base);
  free(c->send_buf);
  free(c->addr);
  free(c);
}

void intHandler(int dummy) {
  stop = 1;
}

static void alloc_buffer(uv_handle_t* handle,
                         size_t suggested_size,
                         uv_buf_t* buf) {
  buf->base = malloc(suggested_size);
  buf->len = suggested_size;
}

static void on_forwarding_done(uv_udp_send_t* req, int status) {
  log_info("FORWARDING DONE Status: %d", status);
  free(req);
}

static void on_send(uv_udp_send_t* req, int status) {
  log_info("Sending Status: %d", status);
  uv_buf_t* buf = req->data;
  free(buf->base);
  free(buf);
  free(req);
}


// 接受从dns服务器发回的数据
static void cl_recv_cb(uv_udp_t* handle,
                       ssize_t nread,
                       const uv_buf_t* rcvbuf,
                       const struct sockaddr* addr,
                       unsigned flags) {
  if (nread < 0) {
    free(rcvbuf->base);
    log_warn("Bad Return");
    return;
  }

  if (nread == 0) {
    free(rcvbuf->base);
    log_info("Zero Return");
    return;
  }

  struct client_context* c = handle->data;
  struct server_context* sc = handle->loop->data;
  //id校验
  if (c->forwarding_id != *(unsigned short*)rcvbuf->base) {
    return;
  }

  //更换id
  *rcvbuf->base = *((char*)&(c->origin_id) + 1);
  *(rcvbuf->base + 1) = *(char*)&c->origin_id;

  //发送回请求方，并加入LRU
  uv_buf_t* buf = malloc(sizeof(uv_buf_t));
  uv_udp_send_t* req = malloc(sizeof(uv_udp_send_t));
  buf->base = rcvbuf->base;
  buf->len = nread;
  LinkedHashmap_insert(sc->LRU, c->port->question, buf);
  int res = uv_udp_send(req, c->server, buf, 1, c->addr, on_forwarding_done);
  log_info("FORWARDING DONE, UV_UDP_SEND_STATUS: %d", res);

  uv_close((uv_handle_t*)handle, close_cb);
}

static void uv_timer_t_free(void* data) {
  uv_timer_t* handle = data;
  struct timer_context* tc = handle->data;
  uv_timer_stop(handle);
  if (!uv_is_closing((uv_handle_t*)tc->client)) {
    uv_udp_recv_stop((uv_udp_t*)tc->client);
    uv_close((uv_handle_t*)tc->client, close_cb_);
  } else {
    free(tc->client);
  }
  uv_close((uv_handle_t*)handle, close_);
  free(tc);
}

// 超时时关闭监听
static void wait_timeout(uv_timer_t* handle) {
  struct timer_context* tc = handle->data;
  if (!uv_is_closing((uv_handle_t*)tc->client)) {
    uv_udp_recv_stop((uv_udp_t*)tc->client);
    uv_close((uv_handle_t*)tc->client, close_cb_);
  } else {
    free(tc->client);
  }

  struct server_context* sc = uv_default_loop()->data;
  struct DataItem* item = Hashmap_delete(sc->forwarding_client, tc->id);
  free(item->key);
  free(item);

  uv_timer_stop(handle);
  uv_close((uv_handle_t*)handle, close_);
  free(tc);
}

// 发送到远程dns服务器以后，打开监听
static void cl_send_cb(uv_udp_send_t* req, int status) {
  log_info("FORWARDING Status: %d", status);
  uv_timer_t* timer = malloc(sizeof(uv_timer_t));
  struct timer_context* tc = malloc(sizeof(struct timer_context));
  timer->data = tc;
  tc->client = req->handle;

  // 进行计时，如果3秒没有反应，则关闭监听
  uv_timer_init(uv_default_loop(), timer);
  uv_timer_start(timer, wait_timeout, 3000, 0);

  // 打开监听
  uv_udp_recv_start(req->handle, alloc_buffer, cl_recv_cb);

  struct client_context* c = req->handle->data;
  struct server_context* sc = uv_default_loop()->data;
  unsigned short int* id = malloc(sizeof(unsigned short int));
  *id = c->port->header->ID;
  tc->id = id;
  Hashmap_insert(sc->forwarding_client, id, timer);
  free(req);
}

// 读取查询的请求
static void on_read(uv_udp_t* handle,
                    ssize_t nread,
                    const uv_buf_t* rcvbuf,
                    const struct sockaddr* addr,
                    unsigned flags) {
  // 排除空包
  if (nread == 0) {
    free(rcvbuf->base);
    return;
  }

  //解析DNS数据
  struct DNSPort* p =
      DNSPort_parse_from_query((unsigned char*)rcvbuf->base, nread);
  
  // 过滤响应请求
  if (p->header->FLAGS & 0x80) {
    free(rcvbuf->base);
    return;
  }

  //获取server_context
  struct server_context* c = handle->loop->data;

  //malloc数据
  uv_buf_t* buf = malloc(sizeof(uv_buf_t));
  uv_udp_send_t* req = malloc(sizeof(uv_udp_send_t));

  //如果是IPV4查询，则查找host
  if (p->question->TYPE == A) {
    struct DataItem* item;

    log_info("SEARCHING %s ", p->question->NAME);
    item = Hashmap_search(c->hosts, p->question->NAME);
    if (item) {
      log_info("FIND %s in hosts", item->key);
      buf->base = malloc(sizeof(char) * 1024);
      buf->len = GenerateAnswer(buf->base, p, item->data);
      req->data = buf;
      int res = uv_udp_send(req, handle, buf, 1, addr, on_send);
      DNSPort_free(p);
      free(rcvbuf->base);
      return;
    }

    item = Hashmap_search(c->ban_hosts, p->question->NAME);
    if (item) {
      log_info("FIND %s in ban hosts", item->key);
      buf->base = malloc(sizeof(char) * 1024);
      buf->len = GenerateBanedAnswer(buf->base, p);
      req->data = buf;
      int res = uv_udp_send(req, handle, buf, 1, addr, on_send);
      DNSPort_free(p);
      free(rcvbuf->base);
      return;
    }
  }

  // 如果是其他查询或没有找到则去查询缓存
  struct LinkedDataItem* item = LinkedHashmap_search(c->LRU, p->question);
  if (item) {
    uv_buf_t* cached_buf = item->data;
    *cached_buf->base = *((char*)&(p->header->ID) + 1);
    *(cached_buf->base + 1) = *(char*)&p->header->ID;
    int res = uv_udp_send(req, handle, item->data, 1, addr, on_forwarding_done);
    log_info("FIND CACHE, UV_UDP_SEND_STATUS: %d", res);
    DNSPort_free(p);
    free(rcvbuf->base);
    free(buf);
    return;
  }

  // 如果依旧没有，则去请求其他的dns服务器
  uv_udp_t* client = malloc(sizeof(uv_udp_t));
  uv_udp_init(uv_default_loop(), client);
  unsigned short forwarding_code = rand() % 65535;
  *(unsigned short*)rcvbuf->base = forwarding_code;
  buf->base = rcvbuf->base;
  buf->len = nread;

  struct client_context* cc = malloc(sizeof(struct client_context));
  cc->origin_id = p->header->ID;
  cc->forwarding_id = forwarding_code;
  cc->port = p;
  cc->server = handle;
  cc->send_buf = buf;
  cc->addr = malloc(sizeof(struct sockaddr));
  client->data = cc;

  memcpy(cc->addr, addr, sizeof(struct sockaddr));

  int res = uv_udp_send(req, client, buf, 1, (const struct sockaddr*)&send_addr,
                        cl_send_cb);
  log_info("FORWARDING DNS");
}

//监听空闲请求，从而释放资源
void idle_handle(uv_idle_t* handle) {
  if (stop) {
    struct idle_context* ic = handle->data;
    uv_idle_stop(handle);
    uv_close((uv_handle_t*)handle, close_);
    uv_udp_recv_stop(ic->server);
    uv_close((uv_handle_t*)ic->server, close_);
    uv_timer_stop(ic->LRU_clear);
    uv_close((uv_handle_t*)ic->LRU_clear, close_);

    struct server_context* sc = uv_default_loop()->data;
    Hashmap_free(sc->forwarding_client);
  }
}

//进行LRU的清理
void LRU_clear(uv_timer_t* handle) {
  log_info("LRU clear");
  struct server_context* c = handle->loop->data;
  LinkedHashmap_clear(c->LRU, 600000);
}

int main(int argc, char* argv[]) {
  int res;
  struct server_context c;
  struct idle_context ic;
  signal(SIGINT, intHandler);

  // 初始化hosts
  c.hosts = Hashmap_create(1000, str_hashcode, str_eq, free, free);
  c.ban_hosts = Hashmap_create(1000, str_hashcode, str_eq, free, free);

  // 初始化LRU
  c.LRU = LinkedHashmap_create(1000, question_hashcode, question_eq, free,
                               uv_buf_t_free);

  //初始化client 
  c.forwarding_client =
      Hashmap_create(1000, int_hashcode, int_eq, free, uv_timer_t_free);

  // 读取host数据
  read_hosts("./hosts", c.hosts, c.ban_hosts);
  log_debug("c.hosts size: %d", Hashmap_size(c.hosts));
  log_debug("c.ban_hosts size: %d", Hashmap_size(c.ban_hosts));

  uv_idle_t* idle = malloc(sizeof(uv_idle_t));
  uv_udp_t* server = malloc(sizeof(uv_udp_t));
  uv_timer_t* timer = malloc(sizeof(uv_timer_t));
  idle->data = &ic;

  ic.server = server;
  ic.LRU_clear = timer;

  //设置定时器
  uv_timer_init(uv_default_loop(), timer);
  uv_timer_start(timer, LRU_clear, 120000, 120000);

  //设置空闲时操作
  uv_idle_init(uv_default_loop(), idle);
  uv_idle_start(idle, idle_handle);

  //监听UDP
  uv_udp_init(uv_default_loop(), server);
  uv_ip4_addr("0.0.0.0", 53, &recv_addr);
  uv_ip4_addr("114.114.114.114", 53, &send_addr);
  res =
      uv_udp_bind(server, (const struct sockaddr*)&recv_addr, UV_UDP_REUSEADDR);
  if (res < 0) {
    log_error("server bind failed: %s\n", strerror(errno));
    exit(1);
  }

  //绑定server_context
  uv_default_loop()->data = &c;
  uv_udp_recv_start(server, alloc_buffer, on_read);

  //开始运行
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);

  //释放资源
  res = uv_loop_close(uv_default_loop());
  if (res == UV_EBUSY) {
    log_error("loop still busy");
  }
  Hashmap_free(c.hosts);
  Hashmap_free(c.ban_hosts);
  LinkedHashmap_free(c.LRU);
  return 0;
}
