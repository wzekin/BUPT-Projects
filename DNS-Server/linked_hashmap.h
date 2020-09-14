#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#ifndef LINKEDHASHMAP_H
#define LINKEDHASHMAP_H
// 一个带多态的LINKEDHASHMAP,充当LRU

struct LinkedDataItem {
  void* key;
  void* data;

  struct LinkedDataItem* hash_next;

  struct LinkedDataItem* next;
  struct LinkedDataItem* last;

  int hashIndex;
  clock_t time;
};

struct LinkedHashmap {
  struct LinkedDataItem** data;
  size_t bucket_size;

  size_t size;

  struct LinkedDataItem* head;
  struct LinkedDataItem* tail;

  bool (*eq)(const void* self, const void* other);
  size_t (*hashcode)(const void* key);
  void (*key_free)(void* key);
  void (*data_free)(void* data);
};

//创建
struct LinkedHashmap* LinkedHashmap_create(size_t size,
                                           size_t (*hashcode)(const void* key),
                                           bool (*eq)(const void* self,
                                                      const void* other),
                                           void (*key_free)(void* key),
                                           void (*data_free)(void* data));

//搜索
struct LinkedDataItem* LinkedHashmap_search(const struct LinkedHashmap* self,
                                            const void* key);

//删除
struct LinkedDataItem* LinkedHashmap_delete(struct LinkedHashmap* self,
                                            void* key);

//插入
void LinkedHashmap_insert(struct LinkedHashmap* self, void* key, void* data);

//清理过期缓存
void LinkedHashmap_clear(struct LinkedHashmap* self, clock_t time);

//释放资源
void LinkedHashmap_free(struct LinkedHashmap* self);

//返回LRU大小
size_t LinkedHashmap_size(struct LinkedHashmap* self);
#endif
