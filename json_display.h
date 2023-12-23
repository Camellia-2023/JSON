#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "file_operations.h"
#include "json_validation  I.h"
#include "json_validation  II.h"


// 辅助函数，用于打印缩进空格
void printIndent(int indentLevel)
{
    for (int i = 0; i < indentLevel; i++) 
    {
        printf("    "); // 每级缩进使用4个空格
    }
}

// 函数，用于按照树状缩进的格式将JSON字符串显示在屏幕上
void printJson(const char* json) {     //该函数以指向常量字符的指针（const char* json）作为参数，表示 JSON 字符串。
    if (json == NULL) return;
    int indentLevel = 0;  //跟踪当前缩进级别
    int inString = 0; // 用于跟踪是否在字符串内部

    while (*json) {
        switch (*json) {
        case '\"': // 处理字符串的开始和结束
            inString = !inString;   // 如果函数遇到双引号，则切换 inString 变量以指示是否在字符串内部。然后打印双引号
            putchar(*json);
            break;
        case '[':     /*根据字符是开括号还是闭括号，函数调整 indentLevel 并在打印括号之前添加适当的缩进*/
        case '{':
            if (!inString) {
                putchar(*json);
                putchar('\n');
                indentLevel++;
                printIndent(indentLevel);
            }
            else {
                putchar(*json);
            }
            break;
        case ']':
        case '}':
            if (!inString) {
                putchar('\n');
                indentLevel--;
                printIndent(indentLevel);
                putchar(*json);
            }
            else {
                putchar(*json);
            }
            break;
        case ',':   //如果字符是逗号，则函数在打印逗号之前添加换行字符和适当的缩进
            if (!inString) {
                putchar(*json);
                putchar('\n');
                printIndent(indentLevel);
            }
            else {
                putchar(*json);
            }
            break;
        case ':':    // 如果字符是冒号，则函数打印冒号后跟一个空格。
            if (!inString) {
                putchar(*json);
                printf(" ");
            }
            else {
                putchar(*json);
            }
            break;
        case ' ':  //如果字符不属于上述情况，就直接打印
        case '\n':
        case '\t':
        case '\r':
            // 忽略字符串外部的空白字符
            if (inString) {
                putchar(*json);
            }
            break;
        default:
            putchar(*json);
            break;
        }
        json++;
    }
    putchar('\n'); // 确保在输出结束后有一个新行
}


// 函数，用于按照树状缩进的格式将JSON字符串保存在指定的文本文件中
void saveJson(const char* json, const char* filename) {  //一个指向常量字符的指针，表示要保存的 JSON 字符串;一个指向常量字符的指针，表示要保存到的文件的名称。
    if (json == NULL || filename == NULL) return;
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file '%s'\n", filename);
        return;
    }
    int indentLevel = 0;
    int inString = 0; // 用于跟踪是否在字符串内部

    while (*json) {
        switch (*json) {
        case '\"': // 处理字符串的开始和结束
            inString = !inString;
            fputc(*json, file);
            break;
        case '[':     /*在打开大括号（{）或中括号（[）时，增加缩进级别并在新行上添加相应数量的空格。*/
        case '{':
            if (!inString) {
                fputc(*json, file);//在输出到文件时使用 fputc 函数而不是 putchar。
                fputc('\n', file);
                indentLevel++;
                for (int i = 0; i < indentLevel; i++) {
                    fprintf(file, "    "); // 每级缩进使用4个空格
                }
            }
            else {
                fputc(*json, file);
            }
            break;
        case ']':     /*在关闭大括号（}）或中括号（]）时，减少缩进级别并在新行上添加相应数量的空格。*/
        case '}':
            if (!inString) {
                fputc('\n', file);
                indentLevel--;
                for (int i = 0; i < indentLevel; i++) {
                    fprintf(file, "    ");
                }
                fputc(*json, file);
            }
            else {
                fputc(*json, file);
            }
            break;
        case ',':     //在逗号（,）后添加新行和相应数量的缩进。
            if (!inString) {
                fputc(*json, file);
                fputc('\n', file);
                for (int i = 0; i < indentLevel; i++) {
                    fprintf(file, "    ");
                }
            }
            else {
                fputc(*json, file);
            }
            break;
        case ':':       //在冒号（:）后添加一个空格。
            if (!inString) {
                fputc(*json, file);
                fprintf(file, " ");
            }
            else {
                fputc(*json, file);
            }
            break;
        case ' ':
        case '\n':
        case '\t':
        case '\r':
            // 忽略字符串外部的空白字符
            if (inString) {
                fputc(*json, file);
            }
            break;
        default:
            fputc(*json, file);
            break;
        }
        json++;
    }
    fputc('\n', file); // 确保在文件结束后有一个新行
    fclose(file);//函数在完成所有操作后，确保在文件结束时有一个新行，并关闭文件
}

// 函数，用于以压缩的格式保存 JSON 字符串在指定的文本文件中
void saveCompressedJson(const char* json, const char* filename) {
    if (json == NULL || filename == NULL) return;
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file '%s'\n", filename);
        return;
    }
    bool inString = false; // 标记是否处于字符串内部
    const char* ptr = json;
    while (*ptr) {
        if (*ptr == '\"') {
            inString = !inString; // 遇到双引号，切换字符串内外状态
            fputc(*ptr, file); // 写入双引号
        }
        else if (inString || (*ptr != ' ' && *ptr != '\n' && *ptr != '\t' && *ptr != '\r')) {
            fputc(*ptr, file); // 写入字符串内的字符或非空白字符
        }
        ptr++;
    }
    fclose(file);
}
