#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "store_functions.h"
#include "general_functions.h"

int main(int argv, char* argc[]){ //TODO: main should be able to take paths as arguments i guess

    struct article_type db[SIZE_DB];//TODO: dynamic length should be possible for this DB
    int i;
    double trash;

    /*fill new database with nothingness*/
    for(i=0; i < SIZE_DB; i++){
        db[i].amount = 0;
        db[i].price = 0;
        get_price_category(db[i]);
        strcpy(db[i].name, "<empty>");
    }

    //TODO: implement initializing-File with last opened File, and maybe all Database-file existing? But only when no file is already open (via argc)
    //Benutzeroberfläche
    //TODO: there should be no more code in the menu structure, every option should only contain functions, which are declared in modules.

    int option_number = 0;
    while(option_number == 0){
        printf("Choose one of the following options:\n"
               "[1] printing\n"
               "[2] edit\n"
               "[3] save the database\n"
               "[4] load the database\n"
               "[5] close program\n");
        scanf("%i",&option_number); //optionswahl durch user

        /* option print*/
        if(option_number == 1){
            option_number = 0;
            printf("Choose one of the following options:\n"
                   "[1] print the whole database\n"
                   //TODO: print_from_to
                   "[2] print most expensive article\n"
                   "[3] print cheapest article\n"
                   "[4] search and print an article by name\n"
                   "[0] back\n");
            scanf("%i",&option_number);

            /*unteroptionen für printing*/

            /*print whole database*/
            if(option_number == 1){
                print_complete_db(db);
                option_number = 0;
            }

            /*print most expensive article*/
            else if(option_number == 2){
                printf("The most expensive article is:\n");
                print_table_header();
                print_article(maximum(db));
                option_number = 0;
            }

            /*print cheapest article*/
            else if(option_number == 3){
                printf("The cheapest article is:\n");
                print_table_header();
                print_article(minimum(db));
                option_number = 0;
            }

            /*search an article with binary search and print it*/
            else if(option_number == 4){
                printf("Which article do u search?\n");
                char searched_article[100];
                scanf("%s", &searched_article);
                quicksort_name(db,1,biggest_entry(db));
                binary_search_article(db,searched_article,1,biggest_entry(db));
                option_number = 0;
            }
        }

        /* show options to edit the database*/
        else if(option_number == 2){
            option_number = 0;
            printf("Choose one of the following options:\n"
                   "[1] make a new entry in the database\n"
                   "[2] change an entry\n"
                   "[3] delete one entry\n"
                   "[4] sorting option\n"
                   "[0] back\n");
            scanf("%i",&option_number);

            /*unteroptionen für edit*/

                /*entry insert/change*/
            if(option_number == 1){
                i=1;
                while(1){
                    if(strcmp(db[i].name, "<empty>") == 0) {
                        entry_article(&db[i]);
                        break;
                    }else if(i == SIZE_DB){
                        printf("Your database is full");
                        break;
                    }
                    i++;
                }
                option_number = 0;
            }
                /*change a specific entry*/
            else if(option_number == 2){
                printf("Which entry do you want to change?\n"
                       "Type in the entry number:\n");
                scanf("%i",&i);
                entry_article(&db[i]);
                option_number = 0;
            }
                /*entry delete*/
            else if(option_number == 3){
                int index_of_delete;
                printf("Which article do u want to delete:\n");
                scanf("%i", &index_of_delete);
                delete_article(db,index_of_delete);
                option_number = 0;
            }
                /*sorting options*/
            else if(option_number == 4){
                option_number = 0;
                printf("Choose one of the following options:\n"
                       "[1] turn around the database\n"
                       "[2] sort the database from lowest to highest price\n"
                       "[3] sort the database by name (a-z)\n"
                       "[0] back\n");
                scanf("%i",&option_number);

                /*turn around the database*/
                if(option_number == 1){
                    turn_around(db);
                    option_number = 0;
                }
                    /*quicksort_price via price*/
                else if(option_number == 2){
                    quicksort_price(db,1,biggest_entry(db));
                    option_number = 0;
                }
                    /*quicksort_price via name*/
                else if(option_number == 3){
                    quicksort_name(db,1,biggest_entry(db));
                    option_number = 0;
                }
                /*back*/
                else if(option_number == 0){
                    continue;
                }

                else{
                    option_number = 0;
                }
            }
                /*back*/
            else if(option_number == 0){
                continue;
            }
            else{
                option_number = 0;
            }
        }

        /*option to save the database*/
        else if(option_number == 3){

            FILE *db_save;

            db_save = fopen("C:/Users/garri/CLionProjects/Uebung Praktische Informatik/Database.csv", "wb");
            /*fehlerprüfung*/
            if(db_save == NULL){
                printf("file error occurred");
                return 1;
            }
            /*tatsächliches schreiben in datei*/
            else{
                //TODO: Implement FIlE-Header with sorting state, elements
                for (i = 0; i < SIZE_DB; i++) {
                    if (strcmp(db[i].name, "<empty>") != 0) {
                        fprintf(db_save, "%s;%i;%.2f;%.2f;%d\n",
                                db[i].name, db[i].amount, db[i].price, db[i].price * db[i].amount, db[i].price_c);

                    }
                }
                fprintf(db_save,"<empty>");
            }
            fclose(db_save);
            option_number = 0;
        }

        //TODO: load and save should be possible in and from different files
        //TODO: the saving/loading-files shouldnt be .txt, try CSV or what is SQL?
        //TODO: add refreshing of price from a server?
        /*option to load the database*/
        else if(option_number == 4){
            FILE *db_read;
            char line_buffer[1000];
            if((db_read=fopen("C:/Users/garri/CLionProjects/Uebung Praktische Informatik/Database.csv", "r")) == NULL){
                printf("loading unsuccessful\n");
            }else{
                for(i = 1; i<SIZE_DB;i++){
                    fscanf(db_read, "%s", line_buffer); //wieso nicht "&line_buffer"?
                    strcpy(db[i].name, strtok(line_buffer, ";"));
                    if(strcmp(db[i].name, "<empty>") == 0){
                        break;
                    }
                    db[i].amount = atoi(strtok(NULL,";"));
                    db[i].price = atof(strtok(NULL,";"));
                    get_price_category(&db[i]);
                    trash = atof(strtok(NULL,";"));
                    trash = atof(strtok(NULL,";"));
                }
                fclose(db_read);
                printf("loading successful\n");
            }
            option_number = 0;
            fclose(db_read);
        }

        /*option to close the program*/
        else if(option_number == 5){
            break;
        }

        /*meldung bei ungültiger optionswahl*/
        else{
            printf("There is no option %i.\n", option_number); //TODO: This should be in all submenus.
            option_number = 0;
        }
    }
    return 0;
}