//
// Created by garri on 01.04.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "user_interface.h"
#include "file_functions.h"

const char* price_c_names[6] = { "none", "gratis", "cheap", "normal", "expensive", "luxurious"};

void print_article(struct article_type article){
    printf("%-20s%-5i %8.2f  %-9s  %10.2f\n",
           article.name,article.amount,article.price,price_c_names[article.price_c],article.price_total);
}
void print_most_expensive_article(struct database_type *database){
    print_article(database->article_array[get_index_expensive_article(database)]);
}
void print_complete_db(struct database_type database){
    double running_total = 0.0;
    print_table_header();
    for(int i=0; i < database.file_information->size; i++){
        if(database.article_array[i].filled == 1){
            printf("%i\t",i);
            print_article(database.article_array[i]);
            running_total += database.article_array[i].price_total;
        }
    }
    printf("\t\t\t\t\t\tTotal:%11.2f\n", running_total);
}
void print_table_header(){
    printf("\nNo.\tarticle_type\t\t    amount   price  category\t    total\n");
}

int get_index_expensive_article(struct database_type *database) {
    int article_index = -1;
    double max_value = 0;
    for (int i = 0; i < database->file_information->size; i++) {
        if (database->article_array[i].price > max_value) {
            max_value = database->article_array[i].price;
            article_index = i;
        }
        return article_index;
    }
}

int user_menu(){
    int option_number = 0; //TODO: option_number sollte vielleicht durch navigator ersetzt werden, wahrscheinlich bessere darstellungsmöglichkeiten, etwa wenn teile des screens gelöscht werden sollen
    while (option_number == 0) {
        printf("Choose one of the following options:\n"
               "[1] output\n"
               "[2] edit\n"
               "[3] file\n"
               "[0] close program\n");
        scanf("%i", &option_number);

        /*option output*/
        if (option_number == 1) {
            option_number = 0;
            while (option_number == 0) {
                printf("Choose one of the following options:\n"
                       "[1] print the whole database\n"
                       "[2] print most expensive article_type\n"
                       "[3] print cheapest article_type\n"
                       "[4] search and print an article_type by name\n"
                       "[0] back\n");
                scanf("%i", &option_number);

                /*unteroptionen für [1]output*/

                /*print whole database*/
                if (option_number == 1) {
                    return 11;
                }
                /*print most expensive article_type*/
                else if (option_number == 2) {
                    return 12;
                }
                /*print cheapest article_type*/
                else if (option_number == 3) {
                    return 13;
                }
                /*search an article_type with binary search and print it*/
                else if (option_number == 4) {
                    return 14;
                }
                /*back*/
                else if(option_number == 0){
                    break;
                }
                /*invalid option*/
                else {
                    printf("There is no option %i.\n", option_number);
                    option_number = 0;
                }
            }
        }
        /*editing option*/
        else if (option_number == 2) {
            option_number = 0;
            while (option_number == 0) {
                printf("Choose one of the following options:\n"
                       "[1] make a new entry in the database\n"
                       "[2] change an entry\n"
                       "[3] delete one entry\n"
                       "[4] sorting option\n"
                       "[0] back\n");
                scanf("%i", &option_number);

                /*unteroptionen für edit [2]*/

                /*entry insert/change*/
                if (option_number == 1) {
                    return 21;
                }
                /*change a specific entry*/
                else if (option_number == 2) {
                    return 22;
                }
                /*entry delete*/
                else if (option_number == 3) {
                    return 23;
                }
                /*sorting options*/
                else if (option_number == 4) {
                    option_number = 0;
                    while (option_number == 0) {
                        printf("Choose one of the following options:\n"
                               "[1] turn around the database\n"
                               "[2] sort the database from lowest to highest price\n"
                               "[3] sort the database by name (a-z)\n"
                               "[0] back\n");
                        scanf("%i", &option_number);

                        /*submenu "[2]edit[4]sorting options"*/

                        /*turn around the database*/
                        if (option_number == 1) {
                            return 241;
                        }
                        /*quicksort_price via price*/
                        else if (option_number == 2) {
                            return 242;
                        }
                        /*quicksort_price via name*/
                        else if (option_number == 3) {
                            return 243;
                        }
                        /*back*/
                        else if (option_number == 0) {
                            break;
                        }
                        /*invalid option*/
                        else {
                            printf("There is no option %i.\n", option_number);
                            option_number = 0;
                        }
                    }
                }
                /*back*/
                else if (option_number == 0) {
                    break;
                }
                /*invalid option*/
                else {
                    printf("There is no option %i.\n", option_number);
                    option_number = 0;
                }
            }
        }
        /*file options*/
        else if (option_number == 3) {
            option_number = 0;
            while (option_number == 0){
                printf("[1] create a new database\n"
                       "[2] save this file\n"
                       "[3] load a file\n"
                       "[0] back\n");
                scanf("%i", &option_number);

                /*create database*/
                if(option_number == 1){
                    return 31;
                }
                /*save*/
                else if(option_number == 2){
                    return 32;
                }
                /*load*/
                else if(option_number == 3){
                    return 33;
                }
                /*back*/
                else if(option_number == 0){
                    break;
                }
                /*invalid option*/
                else {
                    printf("There is no option %i.\n", option_number);
                    option_number = 0;
                }
            }
        }
        /*option to close the program*/
        else if (option_number == 0) {
            return -1;
        }
        /*ivalid option*/
        else {
            printf("There is no option %i.\n", option_number);
            option_number = 0;
        }
    }
    return 0;
}/*Optionswahl durch user_innen, returned eine für jede option eindeutige nummer zurück, nämlich die aneinandergereihte Zahl der Optionen*/