//
// Created by garri on 01.04.2022.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "user_interface.h"
#include "file_functions.h"
#include "editing_functions.h"

const char* price_c_names[6] = { "none", "gratis", "cheap", "normal", "expensive", "luxurious"};
int ask_for_answer(){

    char answer = '0';
    while(answer != 'y'&& answer !='n'){
        scanf("%c",&answer);
    }
    if(answer == 'y'){
        return 1;
    }else if(answer == 'n'){
        return 0;
    }
}
void printing_configuration(database_type *database){
    database->file_information->print_conf = 0;
    printf("Do you want to output...\n");
    printf("...the name? (y/n)\n");
    database->file_information->print_conf = ask_for_answer()*1 | database->file_information->print_conf;
    printf("...the price? (y/n)\n");
    database->file_information->print_conf = ask_for_answer()*2 | database->file_information->print_conf;
    printf("...the total price? (y/n)\n");
    database->file_information->print_conf = ask_for_answer()*4 | database->file_information->print_conf;
    printf("...the category of the price e? (y/n)\n");
    database->file_information->print_conf = ask_for_answer()*8 | database->file_information->print_conf;
    printf("...the amount? (y/n)\n");
    database->file_information->print_conf = ask_for_answer()*16 | database->file_information->print_conf;
    printf("...the refreshing? (y/n)\n");
    database->file_information->print_conf = ask_for_answer()*32 | database->file_information->print_conf;
    printf("...the filling? (y/n)\n");
    database->file_information->print_conf = ask_for_answer()*64 | database->file_information->print_conf;
    printf("...the time of creation? (y/n)\n");
    database->file_information->print_conf = ask_for_answer()*128 | database->file_information->print_conf;
    printf("...the last time of editing? (y/n)\n");
    database->file_information->print_conf = ask_for_answer()*256 | database->file_information->print_conf;
    }
void print_article(struct article_type article, unsigned int p_conf) {
    struct tm time_edit = *gmtime(&article.last_edited),
            time_created = *gmtime(&article.creation_date);
    if (p_conf & 1) {
        printf("%-12s", article.name);
    }if (p_conf & 16) {
        printf("%-5i", article.amount);
    }if (p_conf & 2) {
        printf("%8.2f", article.price);
    }if (p_conf & 8){
        printf("%-9s", price_c_names[article.price_c]);
    }if (p_conf & 4){
        printf("%10.2f\t", article.price_total);
    }if (p_conf & 128){
        printf("%02d.%02d.%02d %d:%d:%d\t",time_edit.tm_mday,time_edit.tm_mon + 1,time_edit.tm_year+1900, time_edit.tm_hour, time_edit.tm_min, time_edit.tm_sec);
    }if (p_conf & 256) {
        printf("%02d.%02d.%02d %d:%d:%d\n", time_created.tm_mday, time_created.tm_mon + 1, time_created.tm_year + 1900,
               time_created.tm_hour, time_created.tm_min, time_created.tm_sec);
    }
}
void print_most_expensive_article(struct database_type *database){
    print_table_header();
    print_article(database->article_array[get_index_most_expensive_article(database)], database->file_information->print_conf);
} //TODO: solve with binary search
void print_cheapest_article(struct database_type *database){
    print_table_header();
    print_article(database->article_array[get_index_cheapest_article(database)], database->file_information->print_conf);
} //TODO: solve with binary search
void print_complete_db(struct database_type database){
    double running_total = 0.0;
    print_table_header(database.file_information->print_conf);
    for(int i=0; i < database.file_information->size; i++){
        if(database.article_array[i].filled == 1){
            printf("%i\t",i);
            print_article(database.article_array[i],database.file_information->print_conf);
            running_total += database.article_array[i].price_total;
        }
    }
    printf("\t\t\t\t\tTotal:%11.2f\n", running_total);
}
void print_table_header(unsigned int p_conf){ //TODO: formatting
    printf("\nNo.\t");
    if (p_conf & 1) {
        printf("article\t");
    }if (p_conf & 16) {
        printf("amount\t");
    }if (p_conf & 2) {
        printf("price\t");
    }if (p_conf & 8){
        printf("category\t");
    }if (p_conf & 4){
        printf("total\t");
    }if (p_conf & 128){
        printf("last time edited\t");
    }if (p_conf & 256) {
        printf("created\n");
    }
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
        quicksort_name(database,0,database->file_information->size-1);
        printf("Be aware, that the database has been sorted alphabetically.\n");
    }
    char searched_article[100];
    printf("Type in the name of the article you are searching:\n");
    scanf("%s", &searched_article);
    int found_result = binary_search_article_in_range(database, searched_article, 0, database->file_information->size-1);
    if(found_result >= 0) {
        print_table_header(database->file_information->print_conf);
        printf("%i\t", found_result);
        print_article(database->article_array[found_result],database->file_information->print_conf);
    }else if(found_result == -1) {
        printf("There is no article \"%s\"\n", searched_article);
    }else{
        printf("Something really bad happened. Maybe restarting the program could help.\n");
    }

}
int binary_search_article_in_range(struct database_type *database, char* searched_article, int left_boundary, int right_boundary){
    if(database->file_information->sorting_mode != name_a_z){
        return -1;
    }
    int fix_point = (int)ceil((right_boundary - left_boundary) / 2) + left_boundary;
    int compare_result = strcmp(searched_article, database->article_array[fix_point].name);
    if(right_boundary < left_boundary){
        return -1;
    }else if(compare_result > 0){
        return binary_search_article_in_range(database, searched_article, fix_point + 1, right_boundary);
    }else if(compare_result < 0){
        return binary_search_article_in_range(database, searched_article, left_boundary, fix_point-1);
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
                       "[4] search and print one article by name\n"
                       "[9] configure printed parameters\n"
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
                /*configure parameter-printing*/
                else if (option_number == 9) {
                    return 19;
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
                               "[4] sort the database by editing date\n"
                               "[5] sort the database by creation date\n"
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
                        /*quicksort_price via date of last edit*/
                        else if (option_number == 4) {
                            return 244;
                        }
                        /*quicksort_price via date of creation*/
                        else if (option_number == 5) {
                            return 245;
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
        /*invalid option*/
        else {
            printf("There is no option %i.\n", option_number);
            option_number = 0;
        }
    }
    return 0;
}/*Optionswahl durch user_innen, returned eine für jede option eindeutige nummer zurück, nämlich die aneinandergereihte Zahl der Optionen*/