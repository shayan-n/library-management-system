#include "utilities.h"
#include "cJSON.h"
#include <regex.h>
#include <string.h>

cJSON* JSON(char *path) {
    char *buffer = (char *) malloc(MAX * sizeof(char));
    FILE *file = fopen(path, "r");

    if (file == NULL) {
        printf("No such path found!");
        return NULL;
    }

    fread(buffer, MAX, 1, file);
    fclose(file);

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) printf("There's a problem with cJson!");
    
    return json;
}

void overwrite(char *path, char *content) {
    FILE* file;
    file = fopen(path, "w");

    if (file == NULL) {
        printf("No such path found!");
        return;
    }

    fwrite(content,sizeof(char),strlen(content),file);
    fclose(file);
}

char* user_json_object_str(User user) {
    char *str = MA(char);
    
    str = RA(char, str, MAX);
    sprintf(str, " \"%s\": { \"id\": %d, \"sex\": \"%s\", \"name\": \"%s\", \"lastname\": \"%s\", \"address\": \"%s\", \"phoneNumber\": \"%s\", \"password\": \"%s\" } ", user.username, user.id, user.sex, user.name, user.lastname, user.address, user.phoneNumber, user.password);
    str = RA(char, str, strlen(str) + 10);

    return str;
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