#include "hashmap.h"
#ifndef  FILE_READER_H
#define  FILE_READER_H
//将host文件中的数据加载到hashmap中
void read_hosts(char *filename, struct Hashmap *hosts,
                struct Hashmap *ban_hosts);
#endif
