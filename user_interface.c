//
// Created by garri on 01.04.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "user_interface.h"
#include "file_functions.h"
#include "editing_functions.h"

const char* price_c_names[6] = { "none", "gratis", "cheap", "normal", "expensive", "luxurious"};

void print_article(struct article_type article){
    printf("%-12s%-5i %8.2f  %-9s  %10.2f\n",
           article.name,article.amount,article.price,price_c_names[article.price_c],article.price_total);
}
void print_most_expensive_article(struct database_type *database){
    print_table_header();
    print_article(database->article_array[get_index_most_expensive_article(database)]);
} //TODO: solve with binary search
void print_cheapest_article(struct database_type *database){
    print_table_header();
    print_article(database->article_array[get_index_cheapest_article(database)]);
} //TODO: solve with binary search
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
    printf("\t\t\t\t\tTotal:%11.2f\n", running_total);
}
void print_table_header(){
    printf("\nNo.\tarticle\t    amount   price  category\t    total\n");
}

int get_index_most_expensive_article(struct database_type *database) {
    int article_index = 0;
    double max_value = database->article_array[0].price;
    for (int i = 1; i < database->file_information->size; i++) {
        if (database->article_array[i].price > max_value) {
            max_value = database->article_array[i].price;
            article_index = i;
        }
    }
    return article_index;
}
int get_index_cheapest_article(struct database_type *database) {
    int article_index = 0;
    double min_value = database->article_array[0].price;
    for (int i = 1; i < database->file_information->size; i++) {
        if (database->article_array[i].price < min_value) {
            min_value = database->article_array[i].price;
            article_index = i;
        }
    }
    return article_index;
}
void get_article_by_name(struct database_type *database){
    if(database->file_information->sorting_mode != name_a_z){
        quicksort_name(database,0,database->file_information->size);
        printf("Be aware, that the database has been resorted alphabetically.\n");
    }
    char searched_article[100];
    printf("Type in the name of the article you are searching:\n");
    scanf("%s", &searched_article);
    int found_result = binary_search_article_in_range(database, searched_article, 0, database->file_information->size);
    if(found_result >= 0) {
        print_table_header();
        printf("%i\t", found_result);
        print_article(database->article_array[found_result]);
    }else if(found_result == -1) {
        printf("There is no article \"%s\"\n", searched_article);
    }else{
        printf("Something really bad happened. Maybe restarting the program could help.\n");
    }

}
int binary_search_article_in_range(struct database_type database, char* searched_article, int left_boundary, int right_boundary){
    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
    /* WARNING: "database" has to be sorted by name, before this function can be used properly!*/
    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
    if(database.file_information->sorting_mode != name_a_z){
        return -1;
    }
    int fix_point = (right_boundary - left_boundary) / 2 + left_boundary;
    int compare_result = strcmp(searched_article, database.article_array[fix_point].name);
    if(right_boundary == left_boundary && compare_result != 0){
        return -1;
    }else if(compare_result > 0){
        return binary_search_article_in_range(database, searched_article, fix_point + 1, right_boundary);
    }else if(compare_result < 0){
        return binary_search_article_in_range(database, searched_article, left_boundary, fix_point);
    }else if(compare_result == 0){
        return fix_point;
    }else{
        printf("Something went very wrong, dont bother with this article.\n");
        return -1;
    }
}

int user_menu(struct database_type *database){
    int option_number = 0;
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
                       "[2] print most expensive article\n"
                       "[3] print cheapest article\n"
                       "[4] search and print an article by name\n"
                       "[0] back\n");
                scanf("%i", &option_number);

                /*unteroptionen für [1]output*/

                /*print whole database*/
                if (option_number == 1) {
                    return 11;
                }
                /*print most expensive article*/
                else if (option_number == 2) {
                    return 12;
                }
                /*print cheapest article*/
                else if (option_number == 3) {
                    return 13;
                }
                /*search an article with binary search and print it*/
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
                               "[2] sort the database by price\n"
                               "[3] sort the database alphabetically\n"
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
                       "[4] show file attributes\n"
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
                /*show file attributes*/
                else if(option_number == 4){
                    return 34;
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
        /*invalid option*/
        else {
            printf("There is no option %i.\n", option_number);
            option_number = 0;
        }
    }
    return 0;
}/*Optionswahl durch user_innen, returned eine für jede option eindeutige nummer zurück, nämlich die aneinandergereihte Zahl der Optionen*/