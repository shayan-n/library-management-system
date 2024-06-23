#include <stdio.h>
#include <stdbool.h>
#include <json-c/json.h>
#include "list.h"

#define MAX 99999

typedef struct {
    char *sex;
    char *name;
    char *address;
    char *password;
    char *lastname;
    char *username;
    char *phoneNumber;
    unsigned long int id;
} User;

typedef struct {
    char *sex;
    char *name;
    char *address;
    char *password;
    char *lastname;
    char *username;
    char *phoneNumber;
    unsigned long int id;
} Admin;

struct json_object* JSON(char *path);