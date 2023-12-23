#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

char* readFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file '%s'\n", filename);
        return NULL;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fprintf(stderr, "Error seeking to end of file '%s'\n", filename);
        fclose(file);
        return NULL;
    }

    long fileSize = 0;
    fileSize = ftell(file);

    if (fileSize == -1L) {
        fprintf(stderr, "Error getting file size for '%s'\n", filename);
        fclose(file);
        return NULL;
    }

    if (fileSize == 0) {
        fclose(file);
        char* emptyString = (char*)malloc(1);
        if (emptyString == NULL) {
            fprintf(stderr, "Error allocating memory for empty string\n");
            return NULL;
        }
        emptyString[0] = '\0';
        return emptyString;
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Error seeking to start of file '%s'\n", filename);
        fclose(file);
        return NULL;
    }

    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Error allocating memory for file '%s'\n", filename);
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(buffer, 1, fileSize, file);
    buffer[bytesRead] = '\0';// 确保字符串正确终止

    fclose(file);
    return buffer;
}



