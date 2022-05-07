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
    enum sorting_mode {
        unsorted,
        price_low_to_high, price_high_to_low,
        name_a_z, name_z_a,
        tm_edit_recent_old, tm_edit_old_recent,
        tm_create_recent_old, tm_create_old_recent,
    }sorting_mode;
    char file_name[1000];
    unsigned short print_conf; //for configuring parameter of article output
    // 0000 0000 0000 0001 = name
    // 0000 0000 0000 0010 = price
    // 0000 0000 0000 0100 = price_total
    // 0000 0000 0000 1000 = price_c
    // 0000 0000 0001 0000 = amount
    // 0000 0000 0010 0000 = refreshing
    // 0000 0000 0100 0000 = filling
    // 0000 0000 1000 0000 = creation
    // 0000 0001 0000 0000 = last edited
}database_information_type;
typedef struct database_type{
    database_information_type *file_information;
    article_type *article_array;
}database_type;

struct article_type *create_article_array();
struct database_type database_creation();
struct database_type database_extension();
void extend_article_array();
void reduce_article_array();

void load_database();
void save_database();
void file_stat();

#endif //UNTITLED3_FILE_FUNCTIONS_H
