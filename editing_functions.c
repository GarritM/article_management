//
// Created by garri on 02.04.2022.
//

#include <stdio.h>
#include <string.h>
#include "editing_functions.h"
#include "file_functions.h"

void entry_article(struct database_type *database, int article_index) {
    entry_article_filled(-1, &database->article_array[article_index]); //if process is interrupted it shows database.article_array.filling = -1
    entry_article_name(&database->article_array[article_index]);
    entry_article_price(&database->article_array[article_index]);
    entry_article_amount(&database->article_array[article_index]);
    entry_article_price_total(&database->article_array[article_index]);
    entry_article_price_category(&database->article_array[article_index]);
    entry_article_filled(1, &database->article_array[article_index]);
    printf("entry successful\n");
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

void entry_article_name(struct article_type *article) {
    char name[100], buffer[100000] = "", dump;
    printf("name of the article:\n");
    //empty buffer, or else bad things might happen
    do {
        dump = getchar();
    } while (dump != '\n' && dump != EOF);
    //the real scanning; scans negation of subset [\n]
    if (scanf("%[^\n]",
              &buffer)) { //TODO: Stackoverflow said this is the worst possible way to implement this, learn gets(); vulnerable to buffer overflow-shit
        buffer[99] = "\0";
        strcpy(name, buffer);
    } else {
        strcpy(name, "<unnamed>");
        printf("error: name couldn't be assigned to article_type\n");
    }
    //empty buffer again just for safety
    do {
        dump = getchar();
    } while (dump != '\n' && dump != EOF);
    no_space_for_strings(
            name); //TODO: probably here, should be checked if a identical name already exists in this database. If so check if price is identical two... then just add the amount, if not, expand name by an identifier,
    strcpy(article->name, name);

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
