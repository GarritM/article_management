//
// Created by garri on 19.02.2022.
//
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "store_functions.h"


const char* price_c_names[5] = { "gratis", "cheap", "normal", "expensive", "luxurious"};

char* ask_for_article(){
    char* article_name;
    printf("Whats the name of the article?\n");
    scanf("%s", &article_name);
    return article_name;
}
int ask_for_article_amount(){
    int amount_of_article;
    printf("What amount of this article does the store have?\n");
    scanf("%i",&amount_of_article);
    return amount_of_article;
}
double ask_for_article_price(){
    double price_of_article = -1;
    printf("What is the price of this article?\n");
    scanf("%lf",&price_of_article);
    while(price_of_article < 0){
        printf("The price of the article should be positive or zero.\n"
               "Check if ur input was correct and give a correct price:\n");
        scanf("%lf",&price_of_article);
    }
    return price_of_article;
}
double average_price(double price_1, double price_2){
    return (price_1+price_2)/2;
}
int biggest_entry(struct article_type *db){
    int i, biggest = 0;
    for(i=1; i<SIZE_DB-1;i++){
        if(strcmp(db[i+1].name,"<empty>") == 0){
            return i; //wenn das nächste element <empty> ist, dann wird i returnt, da es der letzte 'echte' eintrag
        }
    }
    return i;
}
void binary_search_article(struct article_type *db, char* searched_article, int left_boundary, int right_boundary){
    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
    /* WARNING: "db" has to be sorted, before this function can be used properly!*/
    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
    // TODO: Implementation of quicksort ONLY at the beginning of the function
    int left_end = left_boundary;
    int right_end = right_boundary;
    int fix_point = (right_end-left_end)/2 + left_end;
    int compare_result = strcmp(searched_article, db[fix_point].name);
    if(right_end == left_end && compare_result !=0){
        printf("There is no article \"%s\"\n",searched_article);
    }else if(compare_result > 0){
        binary_search_article(db, searched_article, fix_point + 1, right_end);
    }else if(compare_result < 0){
        binary_search_article(db, searched_article, left_end, fix_point);
    }else if(compare_result == 0){
        print_table_header();
        print_article(db[fix_point]);
    }else{
        printf("Something went wrong, dont bother with this article.\n");
    }
}
double calculate_total(int amount, ...){
    double total = 0.0;
    int i;
    va_list x;

    va_start(x,amount);
    for(i=1;i<=amount;i++){
        total += va_arg(x,double);
    }
    va_end(x);

    return total;
}
int compare(const void *p1, const void *p2){
    return *(double *)p1 - *(double *)p2;
}
void entry_article(struct article_type *article){
    char name[100], name_with_no_space[100], buffer[100000] = "", dump;
    double price;
    int amount;
    printf("name of the article:\n");
    //empty buffer, or else bad things might happen
    do{
        dump = getchar();
    }while(dump != '\n' && dump != EOF);
    //the real scanning; scans negation of subset [\n]
    if(scanf("%[^\n]", &buffer)){ //TODO: Stackoverflow said this is the worst possible way to implement this, learn gets(); vulnerable to bufferoverflow-shit
        buffer[99] = "\0";
        strcpy(name,buffer);
    }else{
        strcpy(name, "");
    }
    //empty buffer again just for safety
    do{
        dump = getchar();
    }while(dump != '\n' && dump != EOF);
    no_space_for_strings(name); //TODO: probably here, should be checked if a identical name already exists in this database. If so check if price is identical two... then just add the amount, if not, expand name by an identifier,
    strcpy(article->name, name);
    printf("price:\n");
    scanf("%lf",&price);
    article->price = price;
    get_price_category(article);
    printf("amount:\n");
    scanf("%i",&amount);
    article->amount = amount;
    printf("entry successful\n");
}
void delete_article(struct article_type *db, int item_to_delete){
    int i;
    for(i=item_to_delete;i<SIZE_DB-1;i++){
        db[i]=db[i+1];
    }
    strcpy(db[SIZE_DB].name, "<empty>");
    db[SIZE_DB].price = 0;
    get_price_category(&db[SIZE_DB]);
    db[SIZE_DB].amount = 0;
    printf("deletion successful\n");
}
void get_price_category(struct article_type *article){
    if(article -> price == 0) {
        article -> price_c = gratis;
    }else if(article -> price < 0.65){
        article -> price_c = cheap;
    }else if(article -> price  < 2.50){
        article -> price_c = normal;
    }else if(article -> price < 8.50) {
        article -> price_c = expensive;
    }else{
        article -> price_c = luxurious;
    }
}
struct article_type maximum(struct article_type *db){
    int i,j=0;
    double max_value = 0;
    struct article_type db_return[SIZE_DB];

    for(i=1; i<SIZE_DB-1;i++){
        if(db[i].price > max_value){
            max_value = db[i].price;
            db_return[j] = db[i];
        }else if(strcmp(db[i].name,"<empty>")==0){
            return db_return[j];
        }
    }
    return db_return[j];
}
struct article_type minimum(struct article_type *db){
    int i,j=0;
    struct article_type db_return = db[1];
    double min_value = db[1].price;

    for(i=1; i<SIZE_DB-1;i++){
        if(strcmp(db[i].name,"<empty>")==0){
            return db_return;
        }
        else if(db[i].price < min_value){
            min_value = db[i].price;
            db_return = db[i];
        }
    }
    return db_return;
}
void no_space_for_strings(char* some_random_string){
    char some_modified_string[100] = "", *address;
    address = strtok(some_random_string, " ");
    while(address != NULL){
        strcat(some_modified_string, address);
        address = strtok(NULL," ");
    }
    strcpy(some_random_string, some_modified_string);
}
void print_article(struct article_type article){
    printf("%-20s%-5i %8.2f  %-9s  %10.2f\n",article.name,article.amount,article.price,price_c_names[article.price_c],article.price*article.amount);
}
void print_complete_db(article_type *db){
    double running_total = 0.0;
    print_table_header();
    for(int i=0; i < SIZE_DB; i++){
        if(strcmp(db[i].name, "<empty>") != 0){
            printf("%i\t",i);
            print_article(db[i]);
            running_total += db[i].price*db[i].amount;
        }
    }
    printf("\t\t\t\t\t\tTotal:%11.2f\n", running_total);
}
void print_table_header(){
    printf("\nNo.\tarticle\t\t    amount   price  category\t    total\n");
}
double price_of_amount(int amount_of_article, double price_of_article){
    double price_of_amount;
    price_of_amount = amount_of_article*price_of_article;
    return price_of_amount;
}
void quicksort_price(struct article_type *db, int left_boundary, int right_boundary){
    int fix_point = (right_boundary+left_boundary)/2, i = left_boundary, j = right_boundary;
    double compare_price = db[fix_point].price;
    struct article_type dummy;
    do{
        while(db[i].price < compare_price){
            i++;
        }
        while(db[j].price > compare_price){
            j--;
        }
        if(i<=j){
            dummy = db[i];
            db[i] = db[j];
            db[j] = dummy;
            i++;
            j--;
        }
    }while(i<=j);
    if(left_boundary<j){
        quicksort_price(db, left_boundary, j);
    }
    if(right_boundary>i){
        quicksort_price(db, i, right_boundary);
    }
}
void quicksort_name(struct article_type *db, int left_boundary, int right_boundary){
    int fix_point = (right_boundary+left_boundary)/2, i = left_boundary, j = right_boundary;
    char* compare_name = db[fix_point].name;
    struct article_type dummy;
    do{
        while(strcmp(db[i].name, compare_name)<0){
            i++;
        }
        while(strcmp(db[j].name, compare_name)>0){
            j--;
        }
        if(i<=j){
            dummy = db[i];
            db[i] = db[j];
            db[j] = dummy;
            i++;
            j--;
        }
    }while(i<=j);
    if(left_boundary<j){
        quicksort_name(db, left_boundary, j);
    }
    if(right_boundary>i){
        quicksort_name(db, i, right_boundary);
    }
}
void turn_around(struct article_type *db) {
    int i, j;
    struct article_type dummy;
    for(i=1, j= biggest_entry(db); i<j;i++,j--){
        dummy = db[i];
        db[i] = db[j];
        db[j] = dummy;
    }
}
