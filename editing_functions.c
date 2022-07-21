//
// Created by garri on 02.04.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "editing_functions.h"
#include "file_functions.h"
#include "user_interface.h"


void change_article(database_type *database){
    int change_index = -1;
    char buffer[5];
    while(change_index == -1){
        printf("Type in the index of the entry you want to change: ");
        fflush(stdout);
        fgets(buffer, 5, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        change_index = atoi(&buffer);}
    entry_article(*database, change_index);
    database->file_information->sorting_mode = unsorted;
    database->file_information->change_mark = 1;
}
void entry_article(struct database_type *database, int article_index) {
    database->article_array[article_index].filled -= 2; // filled = 0 means, memory was allocated, filled 1 there is actually an entry; idk why i did this in the first place since there are better solutions
    if (entry_article_name(database, article_index) == 0) {
        entry_article_price(&database->article_array[article_index]);
        entry_article_amount(&database->article_array[article_index]);
        entry_article_price_total(&database->article_array[article_index]);
        entry_article_price_category(&database->article_array[article_index]);
        if(database->article_array[article_index].filled == -2){
            entry_article_time(&database->article_array[article_index]);
        }else if(database->article_array[article_index].filled == -1){
            entry_article_editing_time(&database->article_array[article_index]);
        }
        entry_article_filled(1,&database->article_array[article_index]);
        printf("entry successful\n");
        fflush(stdout);
    }
}
void delete_article(database_type *database){
    int delete_index = -1;
    char buffer[5];
    while(delete_index == -1){
        printf("Type in the index of the entry you want to delete: ");
        fflush(stdout);
        fgets(buffer, 5, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        delete_index = atoi(buffer);
    }
    for(int i=delete_index;i<database->file_information->size;i++){
        database->article_array[i]=database->article_array[i+1]; // this is a problem when deleting the last item
    }
    reduce_article_array(database, 1);
    database->file_information->size -= 1;
    database->file_information->change_mark = 1;
    printf("deletion successful\n");
}

void entry_article_amount(struct article_type *article) {
    int amount = -1;
    char buffer[5];
    while(amount == -1){
        printf("amount: ");
        fflush(stdout);
        fgets(buffer, 5, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        amount = atoi(&buffer);
    article->amount = amount;}
}
void entry_article_filled(int mode_of_filling, struct article_type *article) {
    article->filled = mode_of_filling;
}
int entry_article_name(database_type *database, int article_index) {
    char name[ART_NAME_LENGTH], buffer[100] = "";
    int dump;
    printf("name of the article: ");
    fflush(stdout);//TODO: for some reason the execution in cLion wont output the whitespace until another non whitespace is printed, which causes consistent bug when returning to the menu loop
    if(fgets(buffer, ART_NAME_LENGTH, stdin) != 0){
        buffer[strcspn(buffer, "\n")] = '\0'; //"fgets()" also copys the '\n' (which we dont like) strcspn counts number of char until it hits "/n" or "/0" (latter by default)
        strcpy(name, buffer);
    }
    else {
        strcpy(name, "corrupted_article_name");
        printf("error: name couldn't be assigned to article_type\n");
        return -1;
    }
    no_space_for_strings(name);
    int search_result = binary_search_article_in_range(database, name, 0, database->file_information->size);
    if(search_result == -1) {
        strcpy(database->article_array[article_index].name, name);
        return 0;
    }else if(search_result >= 0){
        printf("The name you entered already exists:\n");
        print_table_header();
        printf("%i\t", search_result);
        print_article(database->article_array[search_result]);
        printf("\n");
        return -1;

    }
    return -1;
}
void entry_article_price(struct article_type *article) {
    double price;
    char buffer[10];
    char* dump_i[10];
    memset(&article->price, 0, sizeof(article->price));
    printf("price: ");
    fflush(stdout);
    fgets(buffer, 10, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    article->price = strtod(buffer, dump_i);
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
void entry_article_editing_time(struct article_type*article){
    time(&article->last_edited);
}

void new_entry(struct database_type *database) {
    if (database->file_information->size == 0) { // für den ersten eintrags
        entry_article(database, database->file_information->size);
        database->file_information->size = 1;
    } else {
        extend_article_array(database, 1);
        entry_article(database, database->file_information->size);
        database->file_information->size += 1;
    }
    database->file_information->sorting_mode = unsorted;
    database->file_information->change_mark = 1;
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
void quicksort_price_algorithm(database_type *database, int left_boundary, int right_boundary) {
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
            dummy = database->article_array[i];
            database->article_array[i] = database->article_array[j];
            database->article_array[j] = dummy;
            i++;
            j--;
        }
    } while (i <= j);
    if (left_boundary < j) {
        quicksort_price_algorithm(database, left_boundary, j);
    }
    if (right_boundary > i) {
        quicksort_price_algorithm(database, i, right_boundary);
    }
}
void quicksort_price(struct database_type *database, int left_boundary, int right_boundary) {
    if (database->file_information->sorting_mode == price_low_to_high) {
        turn_around(database);
    } else if (database->file_information->sorting_mode == price_high_to_low) {
        turn_around(database);
    } else {
        quicksort_price_algorithm(database, left_boundary, right_boundary);
        database->file_information->sorting_mode = price_low_to_high;
        database->file_information->change_mark = 1;
    }
}
void quicksort_name_algorithm(struct database_type *database, int left_boundary, int right_boundary){
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
        quicksort_name_algorithm(database, left_boundary, j);
    }
    if(right_boundary>i){
        quicksort_name_algorithm(database, i, right_boundary);
    }
}
void quicksort_name(struct database_type *database, int left_boundary, int right_boundary){
    if(database->file_information->sorting_mode == name_a_z){
        turn_around(database);
    }else if(database->file_information->sorting_mode == name_z_a){
        turn_around(database);
    }else{
        quicksort_name_algorithm(database, left_boundary, right_boundary);
        database->file_information->sorting_mode = name_a_z;
        database->file_information->change_mark = 1;
    }
}
void quicksort_time_ledited(struct database_type *database, int left_boundary, int right_boundary){
    if(database->file_information->sorting_mode == tm_create_recent_old){
        turn_around(database);
    }else if(database->file_information->sorting_mode == tm_edit_old_recent){
        turn_around(database);
    }else{
        quicksort_time_algorithm_ledited(database, left_boundary, right_boundary);
        database->file_information->sorting_mode = tm_edit_recent_old;
        database->file_information->change_mark = 1;
    }

}
void quicksort_time_algorithm_ledited(struct database_type *database, int left_boundary, int right_boundary) {
    int fix_point = (right_boundary + left_boundary) / 2, i = left_boundary, j = right_boundary;
    double compare_time = database->article_array[fix_point].last_edited;
    struct article_type dummy;
    do {
        while (database->article_array[i].last_edited > compare_time) {
            i++;
        }
        while (database->article_array[j].last_edited < compare_time) {
            j--;
        }
        if (i <= j) {
            dummy = database->article_array[i];
            database->article_array[i] = database->article_array[j];
            database->article_array[j] = dummy;
            i++;
            j--;
        }
    } while (i <= j);
    if (left_boundary < j) {
        quicksort_time_algorithm_ledited(database, left_boundary, j);
    }
    if (right_boundary > i) {
        quicksort_time_algorithm_ledited(database, i, right_boundary);
    }
}

void quicksort_time_created(struct database_type *database, int left_boundary, int right_boundary){
    if(database->file_information->sorting_mode == tm_create_recent_old){
        turn_around(database);
    }else if(database->file_information->sorting_mode == tm_create_old_recent){
        turn_around(database);
    }else{
        quicksort_time_algorithm_ledited(database, left_boundary, right_boundary);
        database->file_information->sorting_mode = tm_create_recent_old;
        database->file_information->change_mark = 1;
    }
}
void quicksort_time_algorithm_created(struct database_type *database, int left_boundary, int right_boundary){
        int fix_point = (right_boundary + left_boundary) / 2, i = left_boundary, j = right_boundary;
        double comparetime = database->article_array[fix_point].creation_date;
        struct article_type dummy;
        do {
            while (database->article_array[i].creation_date > comparetime) {
                i++;
            }
            while (database->article_array[j].creation_date < comparetime) {
                j--;
            }
            if (i <= j) {
                dummy = database->article_array[i];
                database->article_array[i] = database->article_array[j];
                database->article_array[j] = dummy;
                i++;
                j--;
            }
        } while (i <= j);
        if (left_boundary < j) {
            quicksort_time_algorithm_created(database, left_boundary, j);
        }
        if (right_boundary > i) {
            quicksort_time_algorithm_created(database, i, right_boundary);
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
    if(database->file_information->sorting_mode == name_a_z){
        database->file_information->sorting_mode = name_z_a;
    }else if(database->file_information->sorting_mode == name_z_a){
        database->file_information->sorting_mode = name_a_z;
    }else if(database->file_information->sorting_mode == price_high_to_low){
        database->file_information->sorting_mode = price_low_to_high;
    }else if(database->file_information->sorting_mode == price_low_to_high){
        database->file_information->sorting_mode = price_high_to_low;
    }
    database->file_information->change_mark = 1;
}