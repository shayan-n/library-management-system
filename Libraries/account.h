#include "utilities.h"

DLinkedList(User, User)
DLinkedList(Admin, Admin)

void getAllUsers(Linked_List_User *users);
void getAllAdmins(Linked_List_Admin *admins);

User signIn(char *username, char *password);