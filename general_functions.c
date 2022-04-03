//
// Created by garri on 18.03.2022.
//
#include <string.h>
#include "general_functions.h"
int position_in_text(char *text_part, char *text){
    int pos = -1;
    int text_length = (int) strlen(text);
    int text_part_length = (int)strlen(text_part);
    if(text_part_length>text_length){
        return pos;
    }
    for(int i = 0; i<text_length; i++){
        if(text[i] == text_part[0]){
            pos = i;
            for(int k = 1; k<text_part_length;k++){
                if(text[i+k] != text_part[k]){
                    pos = -1;
                    break;
                }
            }
            if(pos != -1){
                break;
            }
        }
    }
    return pos;
}