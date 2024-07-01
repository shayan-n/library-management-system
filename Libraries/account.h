#include "cJSON.h"
#include "utilities.h"

DLinkedList(User, User)

void emptyUser(User *user);
User getUser(char *username);
void getAllUsers(Linked_List_User *users);

int getLastId();
void addUser(char *username, User user);
void changeUserData(char *username, char *key, char *value);

User logIn(char *username, char *password);
User signUp(char *username, char *password, char *phoneNumber, char *name, char *role);
User findUserWithMobile(char *username, char *phoneNumber);