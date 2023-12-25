#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


// �������������ڼ���ַ��Ƿ�Ϊ�հ��ַ�
bool isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}   


// �������������ڼ���ַ��Ƿ�Ϊ���ֵ�һ����
bool isPartOfNumber(char c) {
    return isdigit(c) || c == '-' || c == '+' || c == '.' || c == 'e' || c == 'E';/*��ѧ�������е�ָ�����ֵ� 'e' �� 'E'*/
}  


// ���������ڼ��ӵ�ǰλ�ÿ�ʼ���ַ����Ƿ�Ϊ�Ϸ�������
const char* checkNumber(const char* str, int* num) {
    const char* ptr = str;
    //��������Ƿ��Ը��Ż����ſ�ͷ
    if (*ptr == '-' || *ptr == '+') {
        ptr++;
        (*num)++;
    }


    // ����������֣��������������ֵĳ���
    if (!isdigit(*ptr)) { // ���ֱ��������ֿ�ʼ
        return NULL;    
    }
    while (isdigit(*ptr)) { // �����������ֵ���������
        ptr++;       
        (*num)++;
    }

    // ���С�����֣�������С�����ֵĳ���
    if (*ptr == '.') {
        ptr++;
        if (!isdigit(*ptr)) { // С���������������
            return NULL;     
        }
        while (isdigit(*ptr)) { // ����С�����ֵ���������
            ptr++;
            (*num)++;   
        }
    }



    // ���ָ�����֣�������ָ�����ֵĳ���
    if (*ptr == 'e' || *ptr == 'E') {          
        (*num)++;                              //���� num ָ�����������ʾ����һ��������ַ���ָ���ַ���
        ptr++;                                 //��ָ������ƶ�һλ������ 'e' �� 'E'��
        if (*ptr == '+' || *ptr == '-') { // ָ��������������
            ptr++;
            (*num)++;
        }
        if (!isdigit(*ptr)) { // ָ�����ź�����������
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


    // �������ֺ�ֱ�Ӹ��Ž������Ż򶺺�
    if (*ptr == ',' || *ptr == ']' || *ptr == '}') {
        return ptr;     //�������Щ�ָ���֮һ����ʾ���ֽ�����ɣ���������ָ����ַ���ָ�롣
    }
    // ������ֺ����ǷǷ��ַ����򷵻� NULL
    if (*ptr != '\0' && !isPartOfNumber(*ptr) && *ptr != ',' && *ptr != ']' && *ptr != '}') {
        return NULL;
    }

    return ptr; 
}