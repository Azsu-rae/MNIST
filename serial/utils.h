#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "mnist.h"

FILE* open_or_die(const char* path);
uint32_t read4bytes(FILE* f);
uint8_t readbyte(FILE* f);
Dataset* Dataset_train();

#endif
