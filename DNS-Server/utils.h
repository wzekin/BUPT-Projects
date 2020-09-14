#include <stdio.h>
#include <stdbool.h>


#define $(return_type, function_body) \
({ \
      return_type __fn__ function_body; \
          __fn__; \
})

size_t hight_bit(size_t x);

size_t question_hashcode(const void* key);
size_t str_hashcode(const void* key);
size_t int_hashcode(const void* key);

bool question_eq(const void* self, const void* other);
bool str_eq(const void* self, const void* other);
bool int_eq(const void* self, const void* other);