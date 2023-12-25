#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


// 辅助函数，用于检查字符是否为空白字符
bool isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}   


// 辅助函数，用于检查字符是否为数字的一部分
bool isPartOfNumber(char c) {
    return isdigit(c) || c == '-' || c == '+' || c == '.' || c == 'e' || c == 'E';/*科学计数法中的指数部分的 'e' 或 'E'*/
}  


// 函数，用于检查从当前位置开始的字符串是否为合法的数字
const char* checkNumber(const char* str, int* num) {
    const char* ptr = str;
    //检查数字是否以负号或正号开头
    if (*ptr == '-' || *ptr == '+') {
        ptr++;
        (*num)++;
    }


    // 检查整数部分，并计算整数部分的长度
    if (!isdigit(*ptr)) { // 数字必须以数字开始
        return NULL;    
    }
    while (isdigit(*ptr)) { // 跳过整数部分的所有数字
        ptr++;       
        (*num)++;
    }

    // 检查小数部分，并计算小数部分的长度
    if (*ptr == '.') {
        ptr++;
        if (!isdigit(*ptr)) { // 小数点后必须跟着数字
            return NULL;     
        }
        while (isdigit(*ptr)) { // 跳过小数部分的所有数字
            ptr++;
            (*num)++;   
        }
    }



    // 检查指数部分，并计算指数部分的长度
    if (*ptr == 'e' || *ptr == 'E') {          
        (*num)++;                              //递增 num 指向的整数，表示遇到一个额外的字符（指数字符）
        ptr++;                                 //将指针向后移动一位，跳过 'e' 或 'E'。
        if (*ptr == '+' || *ptr == '-') { // 指数可以有正负号
            ptr++;
            (*num)++;
        }
        if (!isdigit(*ptr)) { // 指数符号后必须跟着数字
            return NULL;         
        }
        while (isdigit(*ptr)) { 
            ptr++;
            (*num)++;
        }
    }
    if (*ptr == ' ' || *ptr == '\n')     
    {
        while (*ptr == ' ' || *ptr == '\n') {      
            (*num)++;
            ptr++;
        }
        if (*ptr == ',' || *ptr == ']' || *ptr == '}') {
            return ptr;
        }                   
        else
            return NULL;

    }


    // 允许数字后直接跟着结束括号或逗号
    if (*ptr == ',' || *ptr == ']' || *ptr == '}') {
        return ptr;     //如果是这些分隔符之一，表示数字解析完成，函数返回指向该字符的指针。
    }
    // 如果数字后面是非法字符，则返回 NULL
    if (*ptr != '\0' && !isPartOfNumber(*ptr) && *ptr != ',' && *ptr != ']' && *ptr != '}') {
        return NULL;
    }

    return ptr; 
}