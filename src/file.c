#include <stdio.h>
#include <stdlib.h>
#include "error.h"

// Reads file
char *fileRead(const char *fileName)
{
    FILE *file;
    char *buffer;
    long fileSize;
    size_t result;

    file = fopen(fileName, "r");
    if (file == NULL)
    {
        errorHandle(E_FILE_NOT_FOUND);
    }

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    buffer = (char *)malloc(sizeof(char) * fileSize);
    if (buffer == NULL)
    {
        errorHandle(E_NULL, "file buffer");
    }

    result = fread(buffer, 1, fileSize, file);
    if (result != fileSize)
    {
        errorHandle(E_FILE_READ);
    }

    return buffer;
}