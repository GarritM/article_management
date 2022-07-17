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
#include <dirent.h>
#include <unistd.h>

void initialize(){

    printf("\n******************************\n"
           "you opened the program:\n"
           "\"article_management\"\n"
           "\n"
           "Version: 1.2\n"
           "by Garrit Morrin\n"
           "%s\n"
           "currently running on: ", __TIMESTAMP__); //TODO TIMESTAMP is compilation time?
#ifdef __unix
    printf("Unix\n");
#elif _WIN32
    printf("Win32\n");
#endif
    printf("\n"
           "\n"
           "Welcome!\n"
           "*******************************\n\n");

    DIR *directory_pointer;
    struct dirent *entry_pointer;
    int dir_existence_check = 0;
    if((directory_pointer = opendir(".\\"))){ //TODO: change it to a fixed folder in "Documents" (for UNIX and Windows)
        while((entry_pointer = readdir(directory_pointer))){
            if(strcmp(entry_pointer->d_name, "Databases") == 0){
                printf("folder Databases found\n");
                dir_existence_check = 1;
                break;
            }
        }
        if(dir_existence_check == 0){
#ifdef _WIN32
            mkdir("Databases");
#else
            mkdir("Databases", 7);
#endif
            printf("folder Databases has been created\n");
        }
    }
}

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
        char file_rights[10] = "----------";
        if(attr.st_mode & S_IRUSR) file_rights[0] = 'r';
        if(attr.st_mode & S_IWUSR) file_rights[1] = 'w';
        if(attr.st_mode & S_IXUSR) file_rights[2] = 'x';
        if(attr.st_mode & S_IRGRP) file_rights[3] = 'r';
        if(attr.st_mode & S_IWGRP) file_rights[4] = 'w';
        if(attr.st_mode & S_IXGRP) file_rights[5] = 'x';
        if(attr.st_mode & S_IROTH) file_rights[6] = 'r';
        if(attr.st_mode & S_IWOTH) file_rights[7] = 'w';
        if(attr.st_mode & S_IXOTH) file_rights[8] = 'x';
        printf("%s access rights\n", file_rights);

        printf("%d UID\n", attr.st_uid);
        printf("%d GID\n", attr.st_gid);
        printf("last change: %s", ctime(&attr.st_atime));
#endif

        printf("%ld Byte\n", attr.st_size);
        printf("last time accessed: %s", ctime(&attr.st_atime));

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

int decode_file_info(char *data, database_type *database){
    database->file_information->size = atoi(strtok(data,";"));
    database->file_information->sorting_mode = atoi(strtok(NULL,";"));
    database->file_information->print_conf = atoi(strtok(NULL,";"));

    return 0;
}
int decode_article_data(char* data, article_type* article){
    strcpy(article->name, strtok(data,";"));
    article->amount = atoi(strtok(NULL,";"));
    article->price = atof(strtok(NULL,";"));
    article->price_total = atof(strtok(NULL,";"));
    article->price_c = atof(strtok(NULL,";"));
    article->filled = atof(strtok(NULL,";"));
    article->creation_date = (time_t)atol(strtok(NULL,";"));
    article->last_edited =(time_t)atol(strtok(NULL,";"));
    return 0;
}
int encode_database_info(char* data,database_information_type db_info){
    sprintf(data,"%i;%i;%i\n",
            db_info.size,
            db_info.sorting_mode,
            db_info.print_conf);
    return 0;
}
int encode_article_data(char* data, article_type* article){
    sprintf(data, "%s;%i;%.2f;%.2f;%d;%i;%ld;%ld\n",
            article->name,
            article->amount,
            article->price,
            article->price_total,
            article->price_c,
            article->filled,
            article->creation_date,
            article->last_edited);
    return 0;
}


void load_database(struct database_type *database){
    FILE *db_read;
    DIR *directory_pointer;
    char file_name_buffer[1000], line_buffer[500], local_file_name[1000], directory_name[1000]; //TODO: Replace by constants
    struct dirent *entry_pointer;
    if(!(directory_pointer = opendir(".\\Databases"))){
        printf("unable to read the directory\n");
        printf("Give an absolute path of file to open:");
        fflush(stdout);
        memset(file_name_buffer, 0, strlen(file_name_buffer)); //TODO: replace strlen() with const
        fgets(file_name_buffer, 1000, stdin);
        file_name_buffer[strcspn(file_name_buffer,"\n")] = '\0';
        strcpy(&database->file_information->file_name, file_name_buffer);
    }
    printf("Which of the following files do u want to open:\n"); //TODO: exclude possibility of empty folder
    while((entry_pointer = readdir(directory_pointer))){
        if(strcmp(entry_pointer->d_name,".")!=0 && strcmp(entry_pointer->d_name, ".."))
        printf("%s\n",entry_pointer->d_name);
    }
    getcwd(directory_name, 1000);
    strcpy(&database->file_information->file_name, directory_name);
#ifdef _WIN32
    strcat(&database->file_information->file_name, "\\Databases\\");
#endif
#ifdef __unix
    strcat(&database->file_information->file_name, "/Database/");
#endif
    memset(file_name_buffer, 0, strlen(file_name_buffer)); //TODO: replace strlen() with const
    fgets(file_name_buffer, 1000, stdin);
    file_name_buffer[strcspn(file_name_buffer,"\n")] = '\0';
    strcpy(local_file_name, file_name_buffer);
    strcat(&database->file_information->file_name,local_file_name);

    //writing rights for the loaded files are changed, denied till this file is closed
    revoke_writing_rights(database->file_information);
    file_stat(database->file_information->file_name);
    if((db_read=fopen(database->file_information->file_name, "r")) == NULL){
        printf("loading unsuccessful\n");
    }else{
        fscanf(db_read, "%s", line_buffer);
        decode_file_info(line_buffer, database);
        database->article_array = create_article_array(database->file_information->size);
        for(int i = 0; i<database->file_information->size;i++){
            fscanf(db_read, "%s", line_buffer); //wieso nicht "&line_buffer"?
            decode_article_data(line_buffer, &database->article_array[i]);
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
        scanf("%s",database.file_information->file_name); //TODO: filename, should be assembled so that the file is saved in the same place always;
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
        printf("Do u want to save your changes before closing the file? (y/n)\n"); //TODO implement: indicator for changes of the file in Database.file_information
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
#ifdef _WIN32
    chmod(file_information->file_name, S_IWRITE | S_IREAD);
#endif
#ifdef __unix
    chmod(file_information->file_name, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH);
#endif
    printf("writing access granted for %s\n", file_information->file_name);
    return 1;
}
int revoke_writing_rights(struct database_information_type *file_information) {
    struct stat file_attr;
    stat(file_information->file_name, &file_attr);
#ifdef _WIN32
    chmod(file_information->file_name, S_IREAD);
#endif
#ifdef __unix
    chmod(file_information->file_name, S_IRUSR | S_IRGRP | S_IROTH);
#endif
    printf("write access revoked for %s\n", file_information->file_name);
    return 1;
}
