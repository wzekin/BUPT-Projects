#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dns_port.h"
static void big_to_little_endian(char* buf, unsigned short int v) {
  unsigned short int* p = &v;
  *buf = *((char*)p + 1);
  *(buf + 1) = *(char*)p;
}

static unsigned short int little_to_big_endian(unsigned char* point) {
  unsigned short int value;
  *(unsigned char*)&value = *(point + 1);
  *((unsigned char*)&value + 1) = *point;
  return value;
}

void DNSPort_free(struct DNSPort* p) {
  if (!p)
    return;
  if (p->header)
    free(p->header);
  if (p->question)
    free(p->question);
  free(p);
}

struct DNSPort* DNSPort_parse_from_query(unsigned char* buf, size_t len) {
  struct DNSPort* port = malloc(sizeof(struct DNSPort));
  port->header = malloc(sizeof(struct Header));
  port->header->ID = little_to_big_endian(buf);
  port->header->FLAGS = *(unsigned short int*)(buf + 2);
  port->header->QDCOUNT = little_to_big_endian(buf + 4);
  port->header->ANCOUNT = little_to_big_endian(buf + 6);
  port->header->NSCOUNT = little_to_big_endian(buf + 8);
  port->header->ARCOUNT = little_to_big_endian(buf + 10);
  buf += 12;
  if (port->header->QDCOUNT) {
    port->question = malloc(sizeof(struct Question));
    struct Question* q = port->question;
    int len = strlen((char*)buf);
    memcpy(q->NAME, buf, len + 1);
    q->NAME[len] = 0;
    q->TYPE = little_to_big_endian(buf + len + 1);
    q->CLASS = little_to_big_endian(buf + len + 3);
    buf += len + 5;
  }
  return port;
}

int GenerateAnswer(char* buf, struct DNSPort* question, char* answer) {
  int len = 12;
  int name_len = strlen(question->question->NAME);

  // HEADER
  memcpy(buf, ACCEPT_DNS_HEADER, 12);
  big_to_little_endian(buf, question->header->ID);
  buf += 12;

  // QUESTION
  memcpy(buf, question->question->NAME, name_len + 1);
  big_to_little_endian(buf + name_len + 1, question->question->TYPE);
  big_to_little_endian(buf + name_len + 3, question->question->CLASS);
  buf += name_len + 5;
  len += name_len + 5;

  // ANSWER
  name_len = strlen(answer);
  memcpy(buf, answer, name_len + 15);
  len += name_len + 15;
  return len;
}

int GenerateBanedAnswer(char* buf, struct DNSPort* question) {
  int len = 12;
  int name_len = strlen(question->question->NAME);

  // HEADER
  memcpy(buf, BAN_DNS_HEADER, 12);
  big_to_little_endian(buf, question->header->ID);
  buf += 12;

  // QUESTION
  memcpy(buf, question->question->NAME, name_len + 1);
  big_to_little_endian(buf + name_len + 1, question->question->TYPE);
  big_to_little_endian(buf + name_len + 3, question->question->CLASS);
  len += name_len + 5;
  return len;
}

void DNSPort_print(struct DNSPort* p) {
  printf("Header:\n");
  printf("ID: %hu\n", p->header->ID);
  printf("FLAGS: %hu\n", p->header->FLAGS);
  printf("QDCOUNT: %hu\n", p->header->QDCOUNT);
  printf("ANCOUNT: %hu\n", p->header->ANCOUNT);
  printf("NSCOUNT: %hu\n", p->header->NSCOUNT);
  printf("ARCOUNT: %hu\n", p->header->ARCOUNT);
  if (p->header->QDCOUNT) {
    struct Question* q = p->question;
    printf("Question %d:\n", 0);
    printf("NAME: %s\n", q->NAME);
    printf("TYPE: %hu\n", q->TYPE);
    printf("CLASS: %hu\n", q->CLASS);
  }
}
