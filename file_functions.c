//
// Created by garri on 02.04.2022.
//

#include "file_functions.h"
#include "user_interface.h"
#include <time.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>


void file_stat(char *filepath) {
    struct stat attr;
    if (stat(filepath, &attr)) {
        printf("Access to file attributes denied.\n");
    }


    else {
        if (attr.st_mode & S_IFREG) {
            printf("regular file\n");
        } else if (attr.st_mode & S_IFDIR) {
            printf("directory file\n");
        }
#ifdef _WIN32
        if (attr.st_mode & S_IREAD) {
            printf("right to read\n");
        }
        if (attr.st_mode & S_IWRITE) {
            printf("right to write\n");
        }
#elif __unix
        char vector[10] = "----------";
        if(attr.st_mode & S_IRUSR) vector[0] = 'r';
        if(attr.st_mode & S_IWUSR) vector[1] = 'w';
        if(attr.st_mode & S_IXUSR) vector[2] = 'x';
        if(attr.st_mode & S_IRGRP) vector[3] = 'r';
        if(attr.st_mode & S_IWGRP) vector[4] = 'w';
        if(attr.st_mode & S_IXGRP) vector[5] = 'x';
        if(attr.st_mode & S_IROTH) vector[6] = 'r';
        if(attr.st_mode & S_IWOTH) vector[7] = 'w';
        if(attr.st_mode & S_IXOTH) vector[8] = 'x';
        printf("%s access rights\n");

        printf("%d UID\n", attr.st_uid);
        printf("%d GID\n", attr.st_gid);
#endif
        printf("%ld Byte\n", attr.st_size);
        printf("last time accessed: %s", ctime(&attr.st_atime));
#ifdef __unix
        printf("last change: %s", ctime(&attr.st_atime));
#endif

    }
}
struct article_type *create_article_array(int array_size){
    article_type* article = (article_type *) malloc(array_size * sizeof(article_type));
    if(article == NULL){
        printf("database creation is unsuccessful due to a memory error\n");
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
    strcpy(database.file_information->file_name, "<empty>");
    database.article_array = create_article_array(1);
    database.article_array->filled = 0;
    database.file_information->print_conf = 511;

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

void load_database(struct database_type *database){
    FILE *db_read;
    char line_buffer[500], file_name[1000];
    printf("Path of file to open:\n");
    scanf("%s", &database->file_information->file_name);

    //writing rights for the loaded files are changed, denied till this file is closed
    //TODO implement: close-function
    revoke_writing_rights(database->file_information);
    file_stat(database->file_information->file_name);
    if((db_read=fopen(database->file_information->file_name, "r")) == NULL){
        printf("loading unsuccessful\n");
    }else{
        fscanf(db_read, "%s", line_buffer);
        database->file_information->size = atoi(strtok(line_buffer,";"));
        database->file_information->sorting_mode = atoi(strtok(NULL,";"));
        database->file_information->print_conf = atoi(strtok(NULL,";"));
        database->article_array = create_article_array(database->file_information->size);
        for(int i = 0; i<database->file_information->size;i++){
            fscanf(db_read, "%s", line_buffer); //wieso nicht "&line_buffer"?
            strcpy(database->article_array[i].name, strtok(line_buffer,";"));
            database->article_array[i].amount = atoi(strtok(NULL,";"));
            database->article_array[i].price = atof(strtok(NULL,";"));
            database->article_array[i].price_total = atof(strtok(NULL,";"));
            database->article_array[i].price_c = atof(strtok(NULL,";"));
            database->article_array[i].filled = atof(strtok(NULL,";"));
            database->article_array[i].creation_date = (time_t)atol(strtok(NULL,";"));
            database->article_array[i].last_edited =(time_t)atol(strtok(NULL,";"));
        }
        printf("loading successful\n");
        printf("\n");

    }
    fclose(db_read);
}
void save_database(struct database_type database){
    FILE *db_save;
    if(strcmp(database.file_information->file_name, "<empty>") == 0){
        printf("Where do u want to save this file? Type in a path:\n");
        scanf("%s",database.file_information->file_name); //TODO: filename, should be assembled so that the file is saved in the same place always
                                                          //      this makes it possible to display the files from which u can load
    }
    printf("saving Database in %s\n", database.file_information->file_name);
    grant_writing_rights(database.file_information);
    db_save = fopen(database.file_information->file_name, "wb");
    /*fehlerprüfung*/
    if(db_save == NULL){
        printf("file error occurred\n");
    }
        /*tatsächliches schreiben in datei*/
    else{
        fprintf(db_save,"%i;%i;%i\n",database.file_information->size,database.file_information->sorting_mode,database.file_information->print_conf);
        for (int i = 0; i < database.file_information->size; i++) {
                fprintf(db_save, "%s;%i;%.2f;%.2f;%d;%i;%ld;%ld\n",
                        database.article_array[i].name,
                        database.article_array[i].amount,
                        database.article_array[i].price,
                        database.article_array[i].price_total,
                        database.article_array[i].price_c,
                        database.article_array[i].filled,
                        database.article_array[i].creation_date,
                        database.article_array[i].last_edited);
        }
        printf("saving process successful\n");
    }

    revoke_writing_rights(database.file_information);
    printf("\n");
}
void close_database(struct database_type *database) {
    if (strcmp(database->file_information->file_name, "<empty>") != 0) {
        printf("Do u want to save your changes before closing the file? (y/n)\n");
        if (ask_for_answer() == 1) {
            save_database(*database);
        }
        grant_writing_rights(database->file_information);
        free(database->article_array);
        free(database->file_information);
        printf("file closed\n");

    }
}

int grant_writing_rights(struct database_information_type *file_information) {
    struct stat file_attr;
    stat(file_information->file_name, &file_attr);
#ifdef _WIN32 | _WIN64
    chmod(file_information->file_name, S_IWRITE | S_IREAD);
    printf("write access granted for %s\n",file_information->file_name);
    return 1;
#endif
}

int revoke_writing_rights(struct database_information_type *file_information) {
    struct stat file_attr;
    stat(file_information->file_name, &file_attr);
#ifdef _WIN32 | _WIN64
    chmod(file_information->file_name, S_IREAD);
    printf("write access revoked for %s\n", file_information->file_name);
    return 1;
#endif
}

