#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dns_port.h"
#include "hashmap.h"
#include "log.h"
#include "utils.h"

struct Hashmap* Hashmap_create(size_t size,
                               size_t (*hashcode)(const void* key),
                               bool (*eq)(const void* self, const void* other),
                               void (*key_free)(void* key),
                               void (*data_free)(void* data)) {
  size = hight_bit(size);
  struct Hashmap* map = malloc(sizeof(struct Hashmap));
  map->data = calloc(sizeof(struct DataItem), size);
  map->bucket_size = size;
  map->hashcode = hashcode;
  map->size = 0;
  map->eq = eq;
  map->key_free = key_free;
  map->data_free = data_free;
  return map;
}

struct DataItem* Hashmap_search(const struct Hashmap* self, const void* key) {
  size_t hashIndex = self->hashcode(key) & (self->bucket_size - 1);
  struct DataItem* item = self->data[hashIndex];
  while (item) {
    if (self->eq(item->key, key))
      return item;
    item = item->hash_next;
  }

  return NULL;
}

void Hashmap_insert(struct Hashmap* self, void* key, void* data) {
  struct DataItem* item = malloc(sizeof(struct DataItem));
  item->key = key;
  item->data = data;
  item->hash_next = NULL;
  int hashIndex = self->hashcode(key) & (self->bucket_size - 1);
  struct DataItem* temp = self->data[hashIndex];
  if (!temp) {
    self->data[hashIndex] = item;
  } else {
    while (temp->hash_next) {
      temp = temp->hash_next;
    }
    temp->hash_next = item;
  }

  self->size++;
}

struct DataItem* Hashmap_delete(struct Hashmap* self, void* key) {
  int hashIndex = self->hashcode(key) & (self->bucket_size - 1);
  struct DataItem* item = self->data[hashIndex];
  if (self->eq(item->key, key)) {
    self->data[hashIndex] = item->hash_next;
    self->size--;
    return item;
  }
  while (item->hash_next) {
    if (self->eq(item->hash_next->key, key)) {
      struct DataItem* temp = item->hash_next;
      item->hash_next = temp->hash_next;
      self->size--;
      return temp;
    }
    item = item->hash_next;
  }
  return NULL;
}

size_t Hashmap_size(struct Hashmap* self) {
  return self->size;
}

void Hashmap_free(struct Hashmap* map) {
  struct DataItem *item, *temp;
  for (int i = 0; i <= map->bucket_size; i++) {
    item = map->data[i];
    while (item) {
      temp = item;
      item = item->hash_next;
      if (temp->key) {
        map->key_free(temp->key);
      }
      if (temp->data) {
        map->data_free(temp->data);
      }
      free(temp);
    }
  }
  free(map->data);
  free(map);
}
