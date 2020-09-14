#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "linked_hashmap.h"
#include "log.h"
#include "utils.h"

static void delete_item_from_link(struct LinkedHashmap* self,
                                  struct LinkedDataItem* item) {
  if (item == self->head) {
    self->head = item->next;
  }
  if (item == self->tail) {
    self->tail = item->last;
  }
  if (item->last) {
    item->last->next = item->next;
  }
  if (item->next) {
    item->next->last = item->last;
  }
}

struct LinkedHashmap* LinkedHashmap_create(size_t size,
                                           size_t (*hashcode)(const void* key),
                                           bool (*eq)(const void* self,
                                                      const void* other),
                                           void (*key_free)(void* key),
                                           void (*data_free)(void* data)) {
  size = hight_bit(size);
  struct LinkedHashmap* map = malloc(sizeof(struct LinkedHashmap));
  map->data = calloc(sizeof(struct LinkedDataItem), size);
  map->bucket_size = size;
  map->size = 0;
  map->head = NULL;
  map->tail = NULL;
  map->hashcode = hashcode;
  map->eq = eq;
  map->key_free = key_free;
  map->data_free = data_free;
  return map;
}

struct LinkedDataItem* LinkedHashmap_search(const struct LinkedHashmap* self,
                                            const void* key) {
  size_t hashIndex = self->hashcode(key) & (self->bucket_size - 1);
  struct LinkedDataItem* item = self->data[hashIndex];
  while (item) {
    if (self->eq(item->key, key))
      return item;
    item = item->hash_next;
  }

  return NULL;
}

void LinkedHashmap_insert(struct LinkedHashmap* self, void* key, void* data) {
  size_t hashIndex = self->hashcode(key) & (self->bucket_size - 1);
  struct LinkedDataItem* item = malloc(sizeof(struct LinkedDataItem));
  item->key = key;
  item->data = data;
  item->hash_next = NULL;
  item->next = NULL;
  item->last = NULL;
  item->hashIndex = hashIndex;
  item->time = clock();
  struct LinkedDataItem* temp = self->data[hashIndex];
  if (!temp) {
    self->data[hashIndex] = item;
  } else {
    while (temp->hash_next) {
      temp = temp->hash_next;
    }
    temp->hash_next = item;
  }

  item->next = self->head;
  if (self->head) {
    self->head->last = item;
  }
  self->head = item;
  if (!self->tail) {
    self->tail = item;
  }
  self->size++;
  log_debug("insert: %d", hashIndex);
}

struct LinkedDataItem* LinkedHashmap_delete(struct LinkedHashmap* self,
                                            void* key) {
  size_t hashIndex = self->hashcode(key) & (self->bucket_size - 1);
  struct LinkedDataItem* item = self->data[hashIndex];
  if (self->eq(item->key, key)) {
    delete_item_from_link(self, item);
    self->data[hashIndex] = item->hash_next;
    self->size--;
    return item;
  }
  while (item->hash_next) {
    if (self->eq(item->hash_next->key, key)) {
      struct LinkedDataItem* temp = self->data[hashIndex];
      delete_item_from_link(self, temp);
      item->hash_next = temp->hash_next;
      self->size--;
      return temp;
    }
    item = item->hash_next;
  }
  return NULL;
}

void LinkedHashmap_clear(struct LinkedHashmap* self, clock_t time) {
  if (!self->tail)
    return;

  clock_t now = clock();
  struct LinkedDataItem* item = self->tail;
  while (item && now - item->time > time) {
    struct LinkedDataItem* temp = item;
    self->data[temp->hashIndex] = NULL;
    self->size--;

    item = temp->last;
    if (temp->key) {
      self->key_free(temp->key);
    }
    if (temp->data) {
      self->data_free(temp->data);
    }
    free(temp);
  }
  self->tail = item;
  if (item) {
    item->next = NULL;
  } else {
    self->head = NULL;
  }
}

size_t LinkedHashmap_size(struct LinkedHashmap* self) {
  return self->size;
}

void LinkedHashmap_free(struct LinkedHashmap* map) {
  struct LinkedDataItem *item, *temp;
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
