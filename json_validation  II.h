#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "json_validation  I.h"

// 定义错误位置结构体
typedef struct {
    int line;       //表示错误发生行数
    int column;     //表示错误发生列数（字符位置）
} ErrorLocation;  //定义了一个结构名为ErrorLocation的结构体


bool isValidJson(const char* json, ErrorLocation* errorLocation) {
    // 初始化错误位置errorLocation，设置行数为 1，列数为 1
    errorLocation->line = 1;  //行
    errorLocation->column = 1;  //列
    int key[1024];  //3种状态
    // 栈用于检查括号匹配
    char stack[1024] = { 0 };
    int stackTop = -1;
    for (int i = 0;i < 1024;i++)
        key[i] = 1;
    // 循环遍历 JSON 字符串
    for (int i = 0; json[i] != '\0'; ++i) {

        char c = json[i];
        // 更新行列信息
        if (c == '\n') {
            errorLocation->line++;
            errorLocation->column = 0;
        }
        else {
            errorLocation->column++;
        }

        // 检查括号匹配
        if (c == '{' || c == '[') {
            stack[++stackTop] = c;
            if (c == '[')
                key[stackTop] = 2;
            continue;

        }
        else if (c == '}' || c == ']') {
            if (stackTop == -1) {
                return false; // 没有匹配的开括号
            }
            char openBracket = (c == '}') ? '{' : '[';
            if (stack[stackTop] != openBracket) {
                return false; // 括号不匹配
            }
            stackTop--;
            continue;
        }

        // 检查数字格式
        if (isPartOfNumber(c)) {
            int num = 0;
            const char* nextChar = checkNumber(json + i, &num);
            errorLocation->column += num;
            if (!nextChar) {

                return false; // 数字格式不合法
            }
            i = nextChar - json - 1; // 跳过检查过的数字
            continue;
        }

        // 检查布尔值和 null
        if (strncmp(json + i, "true", 4) == 0) {
            i += 3; // 跳过 "true"，下一次循环会自增 i
            errorLocation->column += 3;
        }
        else if (strncmp(json + i, "false", 5) == 0) {
            i += 4; // 跳过 "false"，下一次循环会自增 i
            errorLocation->column += 4;
        }
        else if (strncmp(json + i, "null", 4) == 0) {
            i += 3; // 跳过 "null"，下一次循环会自增 i
            errorLocation->column += 3;
        }
        // 检查字符串格式
        if (c == '"') {
            bool isEscaped = false; // 标记转义字符
            i++; // 跳过开始的双引号
            while (json[i] != '\0' && (json[i] != '"' || isEscaped)) {
                if (isEscaped) {
                    isEscaped = false;
                    i--;
                }
                else if (json[i] == '\\') {
                    isEscaped = true; // 遇到反斜杠，设置转义状态为 true
                    i++; // 跳过转义字符后的字符
                }
                i++;
            }
            if (json[i] != '"') {
                return false; // 字符串没有正确结束
            }

            if (key[stackTop] == 1)
            {
                i++;
                errorLocation->column++;
                while (isWhitespace(json[i]))
                {
                    errorLocation->column++;
                    i++;
                }

                if (json[i] != ':')
                    return false;
                key[stackTop] = 0;
                while (isWhitespace(json[++i]))  //：不能连续
                    errorLocation->column++;

                if (json[i] == ':')
                    return false;
                i--;
            }

        }
        if (c == ',' && key[stackTop] == 0)
            key[stackTop] = 1;
        // 检查其他非法字符
        if (!isWhitespace(c) && !isPartOfNumber(c) && c != '{' && c != '}' && c != '[' && c != ']' && c != ',' && c != '"' && c != ':') {
            errorLocation->column++;

            return false; // 非法字符
        }

    }

    // 检查是否所有括号都已匹配
    if (stackTop != -1) {
        errorLocation->column++;
        return false; // 还有未匹配的开括号
    }

    return true; // JSON 字符串合法
}