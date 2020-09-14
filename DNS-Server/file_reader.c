#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "hashmap.h"
#include "log.h"

static unsigned char ANSWER_DATA[] = {
    0x00, 0x01,              // TYPE
    0x00, 0x01,              // CLASS
    0x00, 0x00, 0x00, 0x00,  // TTL
    0x00, 0x04,              // DLENGTH
};

static unsigned char strtil(const char* str, const char til) {
  unsigned char len = 0;
  while (*str != til && *str != '\0') {
    str++;
    len++;
  }
  return len;
}

static unsigned char parse_domain(char* dst, const char* domain) {
  unsigned char l = 1;
  do {
    unsigned char len = strtil(domain, '.');
    *dst = len;
    memcpy(dst + 1, domain, len);
    dst += len + 1;
    l += len + 1;
    domain += len;
  } while (*(domain++));
  *dst = 0;
  return l;
}

void read_hosts(char* filename,
                struct Hashmap* hosts,
                struct Hashmap* ban_hosts) {
  FILE* fp = fopen(filename, "r");
  if (!fp) {
    return;
  }
  unsigned char value[4];
  char* domain = malloc(sizeof(char[256]));
  while (!feof(fp)) {
    memset(domain, 0, 256);
    fscanf(fp, "%hhu.%hhu.%hhu.%hhu %s\n", value, value + 1, value + 2,
           value + 3, domain);
    if (*(int*)value == 0) {
      char* key = malloc(sizeof(char[256]));
      unsigned char len = parse_domain(key, domain);
      Hashmap_insert(ban_hosts, key, NULL);
      log_info("read baned domain: %s", domain);
    } else {
      char* key = malloc(sizeof(char[256]));
      char* data = malloc(sizeof(char[1024]));
      unsigned char len = parse_domain(key, domain);
      memcpy(data, key, len);
      memcpy(data + len, ANSWER_DATA, 10);
      memcpy(data + len + 10, value, 4);
      Hashmap_insert(hosts, key, data);
      log_info("read domain: %s, value: %hhu.%hhu.%hhu.%hhu", domain, *value,
               *(value + 1), *(value + 2), *(value + 3));
    }
  }
  fclose(fp);
  free(domain);
}

/*int main() {*/
/*smart struct Hashmap *hosts = Hashmap_create(1000, str_hashcode, str_eq);*/
/*smart struct Hashmap *ban_hosts = Hashmap_create(1000, str_hashcode,
 * str_eq);*/
/*read_hosts("./hosts", hosts, ban_hosts);*/
/*[>str_display(hosts);<]*/
/*[>str_display(ban_hosts);<]*/
/*}*/
