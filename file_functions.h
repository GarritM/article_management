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

typedef struct file_information{
    int size;
    int sorting_mode;
}file_information;

struct article_type *database_creation();
struct article_type *database_extension();
#endif //UNTITLED3_FILE_FUNCTIONS_H
