//
// Created by garri on 02.04.2022.
//

#include <stdio.h>
#include <string.h>
#include "editing_functions.h"
#include "file_functions.h"
#include "user_interface.h"

void change_article(database_type *database){
    int change_index;
    printf("Type in the index of the entry you want to change:\n");
    scanf("%i", &change_index);
    entry_article(*database, change_index);
}
void entry_article(struct database_type *database, int article_index) {
    entry_article_filled(-1,&database->article_array[article_index]); //if process is interrupted it shows database.article_array.filling = -1
    if (entry_article_name(database, article_index) == 0) {
        entry_article_price(&database->article_array[article_index]);
        entry_article_amount(&database->article_array[article_index]);
        entry_article_price_total(&database->article_array[article_index]);
        entry_article_price_category(&database->article_array[article_index]);
        entry_article_time(&database->article_array[article_index]);
        entry_article_filled(1, &database->article_array[article_index]);
        printf("entry successful\n");
    }
}
void delete_article(database_type *database){
    int delete_index;
    printf("Type in the index of the entry you want to delete:\n");
    scanf("%i", &delete_index);
    for(int i=delete_index;i<database->file_information->size;i++){
        database->article_array[i]=database->article_array[i+1];
    }
    reduce_article_array(database, 1);
    database->file_information->size -= 1;
    printf("deletion successful\n");
}

void entry_article_amount(struct article_type *article) {
    int amount;
    printf("amount:\n");
    scanf("%d", &amount);
    article->amount = amount;
}

void entry_article_filled(int mode_of_filling, struct article_type *article) {
    article->filled = mode_of_filling;
}

int entry_article_name(database_type *database, int article_index) {
    char name[100], buffer[100000] = "", dump;
    printf("name of the article:\n");
    //empty buffer, or else bad things might happen
    do {
        dump = getchar();
    } while (dump != '\n' && dump != EOF);
    //the real scanning; scans negation of subset [\n]
    if (scanf("%[^\n]", buffer)) { //TODO: Stackoverflow said this is the worst possible way to implement this, learn gets(); vulnerable to buffer overflow-shit
        buffer[99] = "\0";
        strcpy(name, buffer);
    } else {
        strcpy(name, "corrupted_article_name");
        printf("error: name couldn't be assigned to article_type\n");
        return -1;
    }
    //empty buffer again just for safety
    do {
        dump = getchar();
    } while (dump != '\n' && dump != EOF);
    no_space_for_strings(name);
    int found_result = binary_search_article_in_range(database, name, 0, database->file_information->size);
    if(found_result == -1) {
        strcpy(database->article_array[article_index].name, name);
        return 0;
    }else if(found_result >= 0){
        printf("The name you entered already exists:\n");
        print_table_header();
        printf("%i\t",found_result);
        print_article(database->article_array[found_result]);
        printf("\n");
        return -1;

    }
    return -1;
}

void entry_article_price(struct article_type *article) {
    double price;
    printf("price:\n");
    scanf("%lf", &price);
    article->price = price;
}

void entry_article_price_category(struct article_type *article) {
    if (article->filled == 0) {
        article->price_c = none;
    } else if (article->price == 0) {
        article->price_c = gratis;
    } else if (article->price < 0.65) {
        article->price_c = cheap;
    } else if (article->price < 2.50) {
        article->price_c = normal;
    } else if (article->price < 8.50) {
        article->price_c = expensive;
    } else if (article->price >= 8.50) {
        article->price_c = luxurious;
    } else {
        article->price_c = none;
        printf("error: unable to assign price category properly\n category of the price is assigned with \"none\"\n");
    }
}

void entry_article_price_total(struct article_type *article) {
    article->price_total = article->price * article->amount;
}

void entry_article_time(struct article_type *article){
    time(&article->creation_date);
    time(&article->last_edited);
}

void new_entry(struct database_type *database) {
    if(database) {
        if (database->file_information->size == 0) { // für den ersten eintrags
            entry_article(database, database->file_information->size);
            database->file_information->size = 1;
        } else {
            extend_article_array(database, 1);
            entry_article(database,database->file_information->size);
            database->file_information->size +=1;
        }
    }
    else{
        printf("There is no existing database.\n");
    }
}

void no_space_for_strings(char *some_random_string) {
        char some_modified_string[100] = "", *address;
        address = strtok(some_random_string, " ");
        while (address != NULL) {
            strcat(some_modified_string, address);
            address = strtok(NULL, " ");
        }
        strcpy(some_random_string, some_modified_string);
    }
void quicksort_price(database_type *database, int left_boundary, int right_boundary) {
    int fix_point = (right_boundary + left_boundary) / 2, i = left_boundary, j = right_boundary;
    double compare_price = database->article_array[fix_point].price;
    struct article_type dummy;
    do {
        while (database->article_array[i].price < compare_price) {
            i++;
        }
        while (database->article_array[j].price > compare_price) {
            j--;
        }
        if (i <= j) {
            article_type dummy;
            dummy = database->article_array[i];
            database->article_array[i] = database->article_array[j];
            database->article_array[j] = dummy;
            i++;
            j--;
        }
    } while (i <= j);
    if (left_boundary < j) {
        quicksort_price(database, left_boundary, j);
    }
    if (right_boundary > i) {
        quicksort_price(database, i, right_boundary);
    }
}
void quicksort_name(struct database_type *database, int left_boundary, int right_boundary){
    int fix_point = (right_boundary+left_boundary)/2, i = left_boundary, j = right_boundary;
    char* compare_name = database->article_array[fix_point].name;
    struct article_type dummy;
    do{
        while(strcmp(database->article_array[i].name, compare_name)<0){
            i++;
        }
        while(strcmp(database->article_array[j].name, compare_name)>0){
            j--;
        }
        if(i<=j){
            dummy = database->article_array[i];
            database->article_array[i] = database->article_array[j];
            database->article_array[j] = dummy;
            i++;
            j--;
        }
    }while(i<=j);
    if(left_boundary<j){
        quicksort_name(database, left_boundary, j);
    }
    if(right_boundary>i){
        quicksort_name(database, i, right_boundary);
    }
}
void swap(database_type *database, int article_index_a, int article_index_b){
    article_type dummy;
    dummy = database->article_array[article_index_a];
    database->article_array[article_index_a] = database->article_array[article_index_b];
    database->article_array[article_index_b] = dummy;
}
void turn_around(database_type *database) {
    int i, j;
    struct article_type dummy;
    for(i=0, j= database->file_information->size-1; i<j;i++,j--){
        dummy = database->article_array[i];
        database->article_array[i] = database->article_array[j];
        database->article_array[j] = dummy;
    }
}

