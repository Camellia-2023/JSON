#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "json_validation  I.h"

// �������λ�ýṹ��
typedef struct {
    int line;       //��ʾ����������
    int column;     //��ʾ�������������ַ�λ�ã�
} ErrorLocation;  //������һ���ṹ��ΪErrorLocation�Ľṹ��


bool isValidJson(const char* json, ErrorLocation* errorLocation) {
    // ��ʼ������λ��errorLocation����������Ϊ 1������Ϊ 1
    errorLocation->line = 1;  //��
    errorLocation->column = 1;  //��
    int key[1024];  //3��״̬
    // ջ���ڼ������ƥ��
    char stack[1024] = { 0 };
    int stackTop = -1;
    for (int i = 0;i < 1024;i++)
        key[i] = 1;
    // ѭ������ JSON �ַ���
    for (int i = 0; json[i] != '\0'; ++i) {

        char c = json[i];
        // ����������Ϣ
        if (c == '\n') {
            errorLocation->line++;
            errorLocation->column = 0;
        }
        else {
            errorLocation->column++;
        }

        // �������ƥ��
        if (c == '{' || c == '[') {
            stack[++stackTop] = c;
            if (c == '[')
                key[stackTop] = 2;
            continue;

        }
        else if (c == '}' || c == ']') {
            if (stackTop == -1) {
                return false; // û��ƥ��Ŀ�����
            }
            char openBracket = (c == '}') ? '{' : '[';
            if (stack[stackTop] != openBracket) {
                return false; // ���Ų�ƥ��
            }
            stackTop--;
            continue;
        }

        // ������ָ�ʽ
        if (isPartOfNumber(c)) {
            int num = 0;
            const char* nextChar = checkNumber(json + i, &num);
            errorLocation->column += num;
            if (!nextChar) {

                return false; // ���ָ�ʽ���Ϸ�
            }
            i = nextChar - json - 1; // ��������������
            continue;
        }

        // ��鲼��ֵ�� null
        if (strncmp(json + i, "true", 4) == 0) {
            i += 3; // ���� "true"����һ��ѭ�������� i
            errorLocation->column += 3;
        }
        else if (strncmp(json + i, "false", 5) == 0) {
            i += 4; // ���� "false"����һ��ѭ�������� i
            errorLocation->column += 4;
        }
        else if (strncmp(json + i, "null", 4) == 0) {
            i += 3; // ���� "null"����һ��ѭ�������� i
            errorLocation->column += 3;
        }
        // ����ַ�����ʽ
        if (c == '"') {
            bool isEscaped = false; // ���ת���ַ�
            i++; // ������ʼ��˫����
            while (json[i] != '\0' && (json[i] != '"' || isEscaped)) {
                if (isEscaped) {
                    isEscaped = false;
                    i--;
                }
                else if (json[i] == '\\') {
                    isEscaped = true; // ������б�ܣ�����ת��״̬Ϊ true
                    i++; // ����ת���ַ�����ַ�
                }
                i++;
            }
            if (json[i] != '"') {
                return false; // �ַ���û����ȷ����
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
                while (isWhitespace(json[++i]))  //����������
                    errorLocation->column++;

                if (json[i] == ':')
                    return false;
                i--;
            }

        }
        if (c == ',' && key[stackTop] == 0)
            key[stackTop] = 1;
        // ��������Ƿ��ַ�
        if (!isWhitespace(c) && !isPartOfNumber(c) && c != '{' && c != '}' && c != '[' && c != ']' && c != ',' && c != '"' && c != ':') {
            errorLocation->column++;

            return false; // �Ƿ��ַ�
        }

    }

    // ����Ƿ��������Ŷ���ƥ��
    if (stackTop != -1) {
        errorLocation->column++;
        return false; // ����δƥ��Ŀ�����
    }

    return true; // JSON �ַ����Ϸ�
}