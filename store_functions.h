//
// Created by garri on 19.02.2022.
//

#ifndef UEBUNG_PRAKTISCHE_INFORMATIK_STORE_FUNCTIONS_H
#define UEBUNG_PRAKTISCHE_INFORMATIK_STORE_FUNCTIONS_H
#define SIZE_DB 100
typedef struct article_type{
    //TODO: add date, editing-date, refreshing y/n, total, product category
    char name[100]/*, price_c[10]*/;
    double price;
    enum price_cs{ gratis, cheap, normal, expensive, luxurious} price_c;
    int amount;
}article_type;

void get_price_category();
int ask_size_db();
double average_price();
int* article_num_array();
char* ask_for_article();
int ask_for_article_amount();
double ask_for_article_price();
int biggest_entry();
void binary_search_article();
double calculate_total(int, ...);
void delete_article();
void entry_article();
struct article_type maximum();
struct article_type minimum();
void no_space_for_strings();
void overwrite_article();
double price_of_amount();
void print_article();
void print_table_header();
void print_complete_db();
void price_table_printing();
void quicksort_name();
void quicksort_price();
void turn_around();

#endif //UEBUNG_PRAKTISCHE_INFORMATIK_STORE_FUNCTIONS_H
