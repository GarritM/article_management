#include <stdio.h>
#include <stdlib.h>
#include "user_interface.h"
#include "file_functions.h"
#include "editing_functions.h"


int main() {
    //TODO: Programm muss wissen ob File geladen oder nicht

    /*user-interface*/
    int chosen_option = 0;
    database_type database;

    while (chosen_option != -1) {
        chosen_option = user_menu();
        //TODO: alle funktionen müssen hinzugefügt werden
        if (chosen_option == 11) {
            print_complete_db(database);
        } else if(chosen_option == 12){
            print_most_expensive_article(database);
        }else if (chosen_option == 21) {
            new_entry(database);
        } else if (chosen_option == 31) {
            database = database_creation();
        }else if (chosen_option == 32) {
            save_database(database);
        }else if (chosen_option == 33) {
            load_database(&database);
        }
    }/*-1 is the return-value of "user_menu()" to close the programm*/
    if (database.article_array != NULL) {
        free(database.article_array);
        free(database.file_information);
    }
    return 0;
}
