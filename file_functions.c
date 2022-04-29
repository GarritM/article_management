//
// Created by garri on 02.04.2022.
//

#include "file_functions.h"
#include <time.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

struct article_type *create_article_array(int array_size){
    article_type* article = (article_type *) malloc(array_size * sizeof(article_type));
    if(article == NULL){
        printf("database creation is unsuccessful due to a memory error");
    }
    return article;
}
struct database_information_type *create_database_information(){
    database_information_type* database_information = (database_information_type *) malloc(sizeof(database_information_type));
    if(database_information == NULL){
        printf("creation of database information was unsuccessful due to a memory error");
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
    database.article_array = create_article_array(1);
    database.article_array->filled = 0;
    return database;
}
void extend_article_array(struct database_type *database, int extension_size){
    database->article_array = (article_type *) realloc(database->article_array, (extension_size + database->file_information->size) * sizeof(article_type));
    if(database->article_array == NULL){
        printf("memory allocation error\n");
    }
}
void reduce_article_array(struct database_type *database, int reduction_size){
    database->article_array = (article_type *) realloc(database->article_array, (database->file_information->size - reduction_size) * sizeof(article_type));
    if(database->article_array == NULL){
        printf("memory allocation error\n");
    }
}

void save_database(struct database_type database){
    FILE *db_save;
    db_save = fopen("C://Users/garri/CLionProjects/Gittesting/Database.csv", "wb");
    /*fehlerprüfung*/
    if(db_save == NULL){
        printf("file error occurred");
    }
        /*tatsächliches schreiben in datei*/
    else{
        //TODO: Implement FIlE-Header with sorting state, elements
        fprintf(db_save,"%i;%i\n",database.file_information->size,database.file_information->sorting_mode);
        for (int i = 0; i < database.file_information->size; i++) {
                fprintf(db_save, "%s;%i;%.2f;%.2f;%d;%i\n",
                        database.article_array[i].name,
                        database.article_array[i].amount,
                        database.article_array[i].price,
                        database.article_array[i].price_total,
                        database.article_array[i].price_c,
                        database.article_array[i].filled);

        }
    }
    fclose(db_save);
}
void load_database(struct database_type *database){
        FILE *db_read;
        char line_buffer[500];
        if((db_read=fopen("C://Users/garri/CLionProjects/Gittesting/Database.csv", "r")) == NULL){
            printf("loading unsuccessful\n");
        }else{
            fscanf(db_read, "%s", line_buffer);
            database->file_information->size = atoi(strtok(line_buffer, ";"));
            database->file_information->sorting_mode = atoi(strtok(NULL,";"));
            database->article_array = create_article_array(database->file_information->size);
            for(int i = 0; i<database->file_information->size;i++){
                fscanf(db_read, "%s", line_buffer); //wieso nicht "&line_buffer"?
                strcpy(database->article_array[i].name, strtok(line_buffer,";"));
                database->article_array[i].amount = atoi(strtok(NULL,";"));
                database->article_array[i].price = atof(strtok(NULL,";"));
                database->article_array[i].price_total = atof(strtok(NULL,";"));
                database->article_array[i].price_c = atof(strtok(NULL,";"));
                database->article_array[i].filled = atof(strtok(NULL,";"));
            }
            printf("loading successful\n");
        }
        fclose(db_read);
}

