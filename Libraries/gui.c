#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utilities.h"
#include "gui.h"

void setESC(int theme) {
    printf("\e[%dm", theme); 
}

void rgb(int r, int g, int b, bool background) {
    printf("\e[%d;2;%d;%d;%dm", background ? 48 : 38, r, g, b);
}

void rgbHelper(char *color) {
    printf("\e%s", color);
}

char* hyperLink(char *link, char *title) {
    char *str = CA(char);
    str = RA(char, str, 1000);
    sprintf(str, "\e]8;;%s\e\\%s\e]8;;\e\\", link, title);
    
    return str;
}

void design(char *word, int j) {
    if (strcmp(word, "normal") == 0) setESC(BASH_NORMAL);
    else if (strcmp(word, "bold") == 0) setESC(BASH_BOLD);
    else if (strcmp(word, "dim") == 0) setESC(BASH_DIM);
    else if (strcmp(word, "italic") == 0) setESC(BASH_ITALIC);
    else if (strcmp(word, "underline") == 0) setESC(BASH_UNDERLINE);
    else if (strcmp(word, "blink") == 0) setESC(BASH_BLINK);
    else if (strcmp(word, "invisible") == 0) setESC(BASH_INVISIBLE);
    else if (strcmp(word, "strikethrough") == 0) setESC(BASH_STRIKETHROUGH);
    else if (strcmp(word, "doubleunderline") == 0) setESC(BASH_DOUBLEUNDERLINE);
    else if (strcmp(word, "overline") == 0) setESC(BASH_OVERLINE);
    else if (strcmp(word, "newline") == 0) printf("\n");
    else if (word[0] == '[' && word[strlen(word) - 1] == 'm') rgbHelper(word);
    // else if (word[0] == '[' && word[strlen(word) - 1] == ']') {
    //     char *link = MA(char);
    //     char *title = MA(char);
    //     int titleFirstIndex = -1;

    //     for (int k = 1; k < j - 1; k++) {
    //         char _letter = *(word + k);

    //         if (_letter == ' ') {
    //             titleFirstIndex = k + 1;
    //         } else {
    //             if (titleFirstIndex != -1) {
    //                 *(title + k - titleFirstIndex) = _letter;
    //             } else {
    //                 *(link + k - 1) = _letter;
    //                 link = RA(char, link, k + 1);
    //             }
    //         }
    //     }

    //     hyperLink(link, title);
    // }
}

void applyDesign(char *_design) {
    int length = strlen(_design), j = 0;
    char *word = MA(char);

    for (int i = 0;i < length; i++) {
        char letter = *(_design + i);

        if (letter != ',') {
            *(word + j) = letter;
            word = RA(char, word, j + 2);
            j += 1;
        } else {
            design(word, j);
            
            *(word + 0) = '\0';
            j = 0;
        }
    }

    design(word, j);
}