//
// Created by garri on 02.04.2022.
//

#include "file_functions.h"
#include <time.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>




struct article_type *database_creation(file_information *fileInformation){
    article_type *article;
    article = (article_type *) malloc(sizeof(article_type));
    if(article == NULL){
        printf("database creation is unsuccessful due to a memory error");
    }else{
        strcpy(article -> name, "<empty");
        article->price = 0;
        article->amount = 0;
        article->price_total = 0;
        article->price_c = none;
        article->refreshing = 0;
        article->filled = 0;
        fileInformation->size = 1;
    }
    return article;
}
struct article_type *database_extension(article_type *article, int extension_size, file_information *fileInformation){
    article = (article_type*)realloc(article, (extension_size + fileInformation->size)*sizeof(article_type));
    if(article == NULL){
        printf("memory allocation error\n");
    }
    fileInformation->size += extension_size;
    return article;
}
void file_creation(){}