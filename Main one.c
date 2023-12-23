#include <stdio.h>
#include <stdlib.h>
#include "json_validation  I.h"
#include "json_validation  II.h"
#include "file_operations.h"
#include "json_display.h"


int main(void){
    char inputFilename[1024] = { 0 };   /*定义了三个字符数组，分别用于存储输入JSON文件路径、
                                         输出JSON文件路径和压缩JSON文件路径。*/
    char outputFilename[1024] = { 0 };
    char compressedFilename[1024] = { 0 }; // 定义压缩文件名
    printf("简单的JSON格式检查器，注意JSON文件中不能有中文符号！\n");
    // 提示用户输入文件路径
    printf("请输入 JSON 文件的路径: ");
    scanf("%1023s", inputFilename);
    printf("请输入输出 JSON 文件的路径: ");
    scanf("%1023s", outputFilename);           //%1023s确保输入的字符串不超过数组大小，以防止缓冲区溢出。
    printf("请输入压缩 JSON 文件的路径: ");
    scanf("%1023s", compressedFilename);
    // 尝试打开并读取文件
    char* json = readFile(inputFilename);   /*readFile函数尝试打开并读取JSON文件，将文件内容保存在json指针指向的内存中。
                                              如果文件打开失败，程序返回1*/
    if (json == NULL) {
        return 1; // 文件打开失败
    }

    ErrorLocation errorLocation;
    if (!isValidJson(json, &errorLocation)) {
        printf("Invalid JSON at line %d, column %d.\n", errorLocation.line, errorLocation.column);
        free(json);
        return 1;
    }

    //printJson(json); // 显示 JSON
    saveJson(json, outputFilename); // 保存 JSON
    saveCompressedJson(json, compressedFilename); // 保存压缩的 JSON

    free(json); // 释放内存
    return 0;
}