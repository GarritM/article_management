#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "user_interface.h"
#include "file_functions.h"
#include "editing_functions.h"

int main(){
    //TODO: Programm muss wissen ob File geladen oder nicht

    /*user-interface*/
    int chosen_option = 0;
    article_type *article;
    file_information *fileInformation;
    while(chosen_option != -1){
        chosen_option = user_menu();
        switch(chosen_option){
            //TODO: alle funktionen müssen hinzugefügt werden
            case 11:
                print_complete_db(&article, fileInformation);
            case 21:
                new_entry(&article, &fileInformation);
            case 31:
                article = database_creation(&fileInformation);
        }
    }/*-1 is the return-value of "user_menu()" to close the programm*/

    free(article);
    return 0;
}
