#include <stdbool.h>
#include <stdlib.h>
#ifndef HASHMAP_H
#define HASHMAP_H

// 一个带多态的HASHMAP,用于存储数据

struct DataItem {
  void* key;
  void* data;

  struct DataItem* hash_next;
};

struct Hashmap {
  struct DataItem** data;
  size_t bucket_size;

  size_t size;
  size_t (*hashcode)(const void* key);
  bool (*eq)(const void* self, const void* other);
  void (*key_free)(void* key);
  void (*data_free)(void* data);
};

//创建hashmap
struct Hashmap* Hashmap_create(size_t size,
                               size_t (*hashcode)(const void* key),
                               bool (*eq)(const void* self, const void* other),
                               void (*key_free)(void* key),
                               void (*data_free)(void* data));

// 搜索
struct DataItem* Hashmap_search(const struct Hashmap* self, const void* key);

// 删除
struct DataItem* Hashmap_delete(struct Hashmap* self, void* key);

// 插入
void Hashmap_insert(struct Hashmap* self, void* key, void* data);

// 释放资源
void Hashmap_free(struct Hashmap* self);

// 返回hashmap的大小
size_t Hashmap_size(struct Hashmap* self);
#endif
