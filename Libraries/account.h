#include "cJSON.h"
#include "utilities.h"

DLinkedList(User, User)
DLinkedList(Admin, Admin)

void getAllUsers(Linked_List_User *users);
void getAllAdmins(Linked_List_Admin *admins);

int getLastId();
void addUser(char *username, User user);

User logIn(char *username, char *password);
User signUp(char *username, char *password, char *phoneNumber, char *name);