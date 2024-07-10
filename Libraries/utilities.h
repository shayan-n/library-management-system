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

DLinkedList(string, char*);

typedef struct {
    char *sex;
    char *name;
    char *role;
    char *address;
    char *password;
    char *lastname;
    char *username;
    char *phoneNumber;
    int id;
} User;

typedef struct {
    double rate;
    bool isBorrow;
    char *id;
    char *title;
    char *genre;
    char *author;
    char *summary;
    char *keywords;
    char *releaseDate;
    char *borrowedTill;
    Linked_List_string *comments;
} Book;

cJSON* JSON(char *path);

void overwrite(char *path, char *content);

bool isAPassword(char *password);
bool isAPhoneNumber(char *phoneNumber);

void get(char *string, int n);