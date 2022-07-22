#include <stdio.h>
#include <pthread.h>

#include "user_interface.h"
#include "file_functions.h"
#include "editing_functions.h"
#include "network_functions.h"

#define ART_NAME_LENGTH 100


int main() {

    database_type database = database_creation();
    initialize();

    database.file_information->lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_t server_thread;

    pthread_create(&server_thread, NULL, init_server, (void*)&database);
    pthread_mutex_lock(&database.file_information->lock);

    int chosen_option = 0;
//TODO: create 1 extra thread for the server-loop

    while (chosen_option != -1) {
        chosen_option = user_menu(database);
        if (chosen_option == 11) {
            print_complete_db(database);
        }else if(chosen_option == 12){
            print_most_expensive_article(database);
        }else if(chosen_option == 13){
            print_cheapest_article(database);
        }else if(chosen_option == 14){
            get_article_by_name(&database);
        }else if(chosen_option == 19) {
            printing_configuration(&database);
        }else if (chosen_option == 21) {
            new_entry(&database);
        }else if (chosen_option == 22) {
            change_article(&database);
        }else if (chosen_option == 23) {
            delete_article(&database);
        }else if (chosen_option == 241) {
            turn_around(&database);
        }else if (chosen_option == 242) {
            quicksort_price(&database,0,database.file_information->size-1);
        }else if (chosen_option == 243) {
            quicksort_name(&database,0,database.file_information->size-1);
        }else if (chosen_option == 244) {
            quicksort_time_ledited(&database,0,database.file_information->size-1);
        }else if (chosen_option == 245) {
            quicksort_time_created(&database,0,database.file_information->size-1);
        }else if (chosen_option == 31) {
            database = database_creation();
        }else if (chosen_option == 32) {
            save_database(&database);
        }else if (chosen_option == 33) {
            load_database(&database);
        }else if (chosen_option == 41) {
//            init_server(&database);
        }else if (chosen_option == 42) {
            init_client(&database);
        }
    }/*-1 is the return-value of "user_menu()" to close the program*/
    pthread_join(server_thread, (void *) &database);
    close_database(database);
    printf("program closed at %s %s\n\n"
           "***********\n"
           "~Good bye!~\n"
           "***********\n", __TIME__, __DATE__);

    return 0;
}
