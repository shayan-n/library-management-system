#include "list.h"
#include "cJSON.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <json-c/json.h>

#define MAX 99999
#define STR_MALLOC_LENGTH 1000
#define string(text) \
    char *text = CA(char); \
    text = RA(char, text, STR_MALLOC_LENGTH);

typedef struct {
    char *sex;
    char *name;
    char *address;
    char *password;
    char *lastname;
    char *username;
    char *phoneNumber;
    int id;
} User;

typedef struct {
    char *sex;
    char *name;
    char *address;
    char *password;
    char *lastname;
    char *username;
    char *phoneNumber;
    int id;
} Admin;

cJSON* JSON(char *path);

void overwrite(char *path, char *content);

bool isAPassword(char *password);
bool isAPhoneNumber(char *phoneNumber);