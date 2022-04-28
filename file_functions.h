//
// Created by garri on 02.04.2022.
//

#ifndef UNTITLED3_FILE_FUNCTIONS_H
#define UNTITLED3_FILE_FUNCTIONS_H
#include <time.h>

typedef struct article_type {
    char name[100];
    double price, price_total;
    enum price_cs {
        none, gratis, cheap, normal, expensive, luxurious
    } price_c;
    int amount, refreshing, filled;
    time_t creation_date, last_edited;
} article_type;

typedef struct database_information_type{
    int size;
    int sorting_mode;
}database_information_type;

typedef struct database_type{
    database_information_type *file_information;
    article_type *article_array;
}database_type;

struct article_type *create_article_array();
struct database_type database_creation();
struct database_type database_extension();
void extend_article_array();
#endif //UNTITLED3_FILE_FUNCTIONS_H