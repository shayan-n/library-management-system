#include "list.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdbool.h>
#include <json-c/json.h>

#define MAX 99999

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
char* user_json_object_str(User user);

bool isAPassword(char *password);
bool isAPhoneNumber(char *phoneNumber);