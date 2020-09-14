#ifndef DNS_PORT_H
#define DNS_PORT_H

// 解析DNS报文

static unsigned char ACCEPT_DNS_HEADER[] = {
    0x00, 0x00,  // ID
    0x80, 0x00,  // FLAGS
    0,    1,     // Number of Questions
    0,    1,     // Number of Answer RRs
    0,    0,     // Number of Authority RRs
    0,    0,     // Number of Additional RRs
};

static unsigned char BAN_DNS_HEADER[] = {
    0x00, 0x00,  // ID
    0x80, 0x03,  // FLAGS
    0,    1,     // Number of Questions
    0,    0,     // Number of Answer RRs
    0,    0,     // Number of Authority RRs
    0,    0,     // Number of Additional RRs
};

enum TYPE { A = 1, NS = 2, CNAME = 5, TXT = 16, AAAA = 28 };
// enum CLASS { IN = 1, CS = 2, CH = 3, HS = 4 };

struct Header {
  unsigned short int ID;
  unsigned short int FLAGS;
  unsigned short int QDCOUNT;
  unsigned short int ANCOUNT;
  unsigned short int NSCOUNT;
  unsigned short int ARCOUNT;
};

struct Question {
  char NAME[64];
  unsigned short int TYPE;
  unsigned short int CLASS;
};

struct DNSPort {
  struct Header* header;
  struct Question* question;
};

struct DNSPort* DNSPort_parse_from_query(unsigned char* buf, size_t len);
// 打印DNS数据
void DNSPort_print(struct DNSPort* p);
// 释放资源
void DNSPort_free(struct DNSPort* p);

// 生成报文
int GenerateAnswer(char* buf, struct DNSPort* question, char* answer);

// 生成baned报文
int GenerateBanedAnswer(char* buf, struct DNSPort* question);
#endif
