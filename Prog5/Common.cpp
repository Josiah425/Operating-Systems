#ifndef COMMON_CPP
#define COMMON_CPP
#include "Common.h"
#include <string>
#include <fstream>
#include <iostream>

SB sb1;
pthread_mutex_t mutex, command;
pthread_cond_t empty, filled;
Inode map[256];
Block Bmap[256];
std::string buffer[100];
bool * freeList;
int start = 0;
int tail = 0;

#endif
