#include "utils.h"
#include "dns_port.h"

#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include <string.h>

size_t hight_bit(size_t x) {
  if (!(x & (x - 1))) {
    return x;
  }
  x = x | (x >> 1);
  x = x | (x >> 2);
  x = x | (x >> 4);
  x = x | (x >> 8);
  x = x | (x >> 16);
  x = x | (x >> 32);
  return x + 1;
}

size_t str_hashcode(const void* key) {
  size_t hash = 0;
  size_t len = strlen(key);
  for (int i = 0; i < len; i++) {
    hash = hash * 31 + *((char*)key + i);
  }
  return hash;
}

size_t question_hashcode(const void* key) {
  const struct Question* q = key;
  size_t hash = 0;
  size_t len = strlen(q->NAME);
  for (int i = 0; i < len; i++) {
    hash = hash * 31 + *((char*)q->NAME + i);
  }
  hash = hash * 31 + q->TYPE;
  hash = hash * 31 + q->CLASS;
  return hash;
}

size_t int_hashcode(const void* key) {
  return *(short*)key;
}

bool str_eq(const void* self, const void* other) {
  return strcmp(self, other) == 0;
}

bool question_eq(const void* self, const void* other) {
  const struct Question* q1 = self;
  const struct Question* q2 = other;
  return q1->CLASS == q2->CLASS && q1->TYPE == q2->TYPE &&
         strcmp(q1->NAME, q2->NAME) == 0;
}

bool int_eq(const void* self, const void* other) {
  return *(short*)self == *(short*)other;
}
