#include <stdio.h>
#include <stdlib.h>
#include "json_validation  I.h"
#include "json_validation  II.h"
#include "file_operations.h"
#include "json_display.h"


int main(void){
    char inputFilename[1024] = { 0 };   /*�����������ַ����飬�ֱ����ڴ洢����JSON�ļ�·����
                                         ���JSON�ļ�·����ѹ��JSON�ļ�·����*/
    char outputFilename[1024] = { 0 };
    char compressedFilename[1024] = { 0 }; // ����ѹ���ļ���
    printf("�򵥵�JSON��ʽ�������ע��JSON�ļ��в��������ķ��ţ�\n");
    // ��ʾ�û������ļ�·��
    printf("������ JSON �ļ���·��: ");
    scanf("%1023s", inputFilename);
    printf("��������� JSON �ļ���·��: ");
    scanf("%1023s", outputFilename);           //%1023sȷ��������ַ��������������С���Է�ֹ�����������
    printf("������ѹ�� JSON �ļ���·��: ");
    scanf("%1023s", compressedFilename);
    // ���Դ򿪲���ȡ�ļ�
    char* json = readFile(inputFilename);   /*readFile�������Դ򿪲���ȡJSON�ļ������ļ����ݱ�����jsonָ��ָ����ڴ��С�
                                              ����ļ���ʧ�ܣ����򷵻�1*/
    if (json == NULL) {
        return 1; // �ļ���ʧ��
    }

    ErrorLocation errorLocation;
    if (!isValidJson(json, &errorLocation)) {
        printf("Invalid JSON at line %d, column %d.\n", errorLocation.line, errorLocation.column);
        free(json);
        return 1;
    }

    //printJson(json); // ��ʾ JSON
    saveJson(json, outputFilename); // ���� JSON
    saveCompressedJson(json, compressedFilename); // ����ѹ���� JSON

    free(json); // �ͷ��ڴ�
    return 0;
}