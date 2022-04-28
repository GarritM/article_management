//
// Created by garri on 02.04.2022.
//

#include "file_functions.h"
#include <time.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

struct article_type *create_article_array(){
    article_type* article = (article_type *) malloc(sizeof(article_type));
    if(article == NULL){
        printf("database creation is unsuccessful due to a memory error");
    }
    return article;
}
struct database_information_type *create_database_information(){
    database_information_type* database_information = (database_information_type *) malloc(sizeof(database_information_type));
    if(database_information == NULL){
        printf("database creation is unsuccessful due to a memory error");
    }
    else {
        database_information->size = 0;
        database_information->sorting_mode = 0;
    }
    return database_information;
}
struct database_type database_creation(){
    struct database_type database;
    database.file_information = create_database_information();
    database.article_array = create_article_array();
    database.article_array->filled = 0;
    return database;
}
void extend_article_array(database_type *database, int extension_size){
    database->article_array = (article_type *) realloc(database->article_array, (extension_size + database->file_information->size) * sizeof(article_type));
    if(database->article_array == NULL){
        printf("memory allocation error\n");
    }
}

