#include "io.hpp"

void ioReadFile(const char* path, char* buffer[], int* length) {
    FILE* file = fopen(path, "r");

    fseek(file, 0L, SEEK_END);
    long __length = ftell(file) * sizeof(char);
    fseek(file, 0L, SEEK_SET);

    char* __buffer = (char*)malloc(__length);
    fread((void*)__buffer, 1, __length, file);

    *buffer = __buffer;
    *length = __length;
}