#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ioReadFile(const char* path, char* buffer[], int* length);

#endif