#include "utilities.h"
#include <regex.h>
#include <string.h>

cJSON* JSON(char *path) {
    char *buffer = (char *) malloc(MAX * sizeof(char));
    FILE *file = fopen(path, "r");

    if (file == NULL) {
        printf("No such path found!");
        int a; scanf("%d", &a);
        return NULL;
    }

    fread(buffer, MAX, 1, file);
    fclose(file);

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        printf("There's a problem with cJson!");
        int a; scanf("%d", &a);
    }
    
    free(buffer);
    return json;
}

void overwrite(char *path, char *content) {
    FILE* file;
    file = fopen(path, "w");

    if (file == NULL) {
        printf("No such path found!");
        int a; scanf("%d", &a);
        return;
    }

    fwrite(content,sizeof(char),strlen(content),file);
    fclose(file);
}

bool isAPhoneNumber(char *phoneNumber) {
    regex_t reg;

    int value;
    value = regcomp(&reg, "^[\\+]\\{0,1\\}[(]\\{0,1\\}[0-9]\\{2,3\\}[)]\\{0,1\\}[0-9]\\{2,3\\}[0-9]\\{4,10\\}$", 0);
    value = regexec(&reg, phoneNumber, 0, NULL, 0);

    if (value == 0) return true;
    return false;
}

bool isAPassword(char *password) {
    regex_t reg;

    int value;
    value = regcomp(&reg, "^[^\\ .]\\{8,\\}$", 0);
    value = regexec(&reg, password, 0, NULL, 0);

    if (value == 0) return true;
    return false;
}

bool isADate(char *date) {
    int length = strlen(date), j = 0, count = 0;
    char *word = MA(char);

    for (int i = 0;i < length; i++) {
        char letter = *(date + i);

        if (letter != '-') {
            *(word + j) = letter;
            word = RA(char, word, j + 2);
            *(word + j + 1) = '\0';
            
            j += 1;
        } else {
            if (count == 1) {
                int len = strlen(word);
                int month = atoi(word);

                if (len > 2 || len < 1) return false;
                if (month < 0 || month > 12) return false;
            }

            *(word + 0) = '\0';
            j = 0;
            count += 1;
        }
    }

    if (count == 2) {
        int len = strlen(word);
        int day = atoi(word);

        if (len > 2 || len < 1) return false;
        if (day < 0 || day > 31) return false;

        return true;
    }
    return false;
}

void get(char *string, int n) {
    fgets(string, n, stdin);

    int len = strlen(string);
    if (string[len - 1] == '\n') {
        string[len - 1] = '\0';
    }
} 