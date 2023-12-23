#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "file_operations.h"
#include "json_validation  I.h"
#include "json_validation  II.h"


// �������������ڴ�ӡ�����ո�
void printIndent(int indentLevel)
{
    for (int i = 0; i < indentLevel; i++) 
    {
        printf("    "); // ÿ������ʹ��4���ո�
    }
}

// ���������ڰ�����״�����ĸ�ʽ��JSON�ַ�����ʾ����Ļ��
void printJson(const char* json) {     //�ú�����ָ�����ַ���ָ�루const char* json����Ϊ��������ʾ JSON �ַ�����
    if (json == NULL) return;
    int indentLevel = 0;  //���ٵ�ǰ��������
    int inString = 0; // ���ڸ����Ƿ����ַ����ڲ�

    while (*json) {
        switch (*json) {
        case '\"': // �����ַ����Ŀ�ʼ�ͽ���
            inString = !inString;   // �����������˫���ţ����л� inString ������ָʾ�Ƿ����ַ����ڲ���Ȼ���ӡ˫����
            putchar(*json);
            break;
        case '[':     /*�����ַ��ǿ����Ż��Ǳ����ţ��������� indentLevel ���ڴ�ӡ����֮ǰ����ʵ�������*/
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
        case ',':   //����ַ��Ƕ��ţ������ڴ�ӡ����֮ǰ��ӻ����ַ����ʵ�������
            if (!inString) {
                putchar(*json);
                putchar('\n');
                printIndent(indentLevel);
            }
            else {
                putchar(*json);
            }
            break;
        case ':':    // ����ַ���ð�ţ�������ӡð�ź��һ���ո�
            if (!inString) {
                putchar(*json);
                printf(" ");
            }
            else {
                putchar(*json);
            }
            break;
        case ' ':  //����ַ������������������ֱ�Ӵ�ӡ
        case '\n':
        case '\t':
        case '\r':
            // �����ַ����ⲿ�Ŀհ��ַ�
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
    putchar('\n'); // ȷ���������������һ������
}


// ���������ڰ�����״�����ĸ�ʽ��JSON�ַ���������ָ�����ı��ļ���
void saveJson(const char* json, const char* filename) {  //һ��ָ�����ַ���ָ�룬��ʾҪ����� JSON �ַ���;һ��ָ�����ַ���ָ�룬��ʾҪ���浽���ļ������ơ�
    if (json == NULL || filename == NULL) return;
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file '%s'\n", filename);
        return;
    }
    int indentLevel = 0;
    int inString = 0; // ���ڸ����Ƿ����ַ����ڲ�

    while (*json) {
        switch (*json) {
        case '\"': // �����ַ����Ŀ�ʼ�ͽ���
            inString = !inString;
            fputc(*json, file);
            break;
        case '[':     /*�ڴ򿪴����ţ�{���������ţ�[��ʱ���������������������������Ӧ�����Ŀո�*/
        case '{':
            if (!inString) {
                fputc(*json, file);//��������ļ�ʱʹ�� fputc ���������� putchar��
                fputc('\n', file);
                indentLevel++;
                for (int i = 0; i < indentLevel; i++) {
                    fprintf(file, "    "); // ÿ������ʹ��4���ո�
                }
            }
            else {
                fputc(*json, file);
            }
            break;
        case ']':     /*�ڹرմ����ţ�}���������ţ�]��ʱ���������������������������Ӧ�����Ŀո�*/
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
        case ',':     //�ڶ��ţ�,����������к���Ӧ������������
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
        case ':':       //��ð�ţ�:�������һ���ո�
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
            // �����ַ����ⲿ�Ŀհ��ַ�
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
    fputc('\n', file); // ȷ�����ļ���������һ������
    fclose(file);//������������в�����ȷ�����ļ�����ʱ��һ�����У����ر��ļ�
}

// ������������ѹ���ĸ�ʽ���� JSON �ַ�����ָ�����ı��ļ���
void saveCompressedJson(const char* json, const char* filename) {
    if (json == NULL || filename == NULL) return;
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file '%s'\n", filename);
        return;
    }
    bool inString = false; // ����Ƿ����ַ����ڲ�
    const char* ptr = json;
    while (*ptr) {
        if (*ptr == '\"') {
            inString = !inString; // ����˫���ţ��л��ַ�������״̬
            fputc(*ptr, file); // д��˫����
        }
        else if (inString || (*ptr != ' ' && *ptr != '\n' && *ptr != '\t' && *ptr != '\r')) {
            fputc(*ptr, file); // д���ַ����ڵ��ַ���ǿհ��ַ�
        }
        ptr++;
    }
    fclose(file);
}
