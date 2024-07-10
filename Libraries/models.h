#include "cJSON.h"
#include "utilities.h"

DLinkedList(User, User)
DLinkedList(Book, Book)

int getLastId(char *path);

void emptyUser(User *user);
User getUser(char *username);
void addUser(char *username, User user);
void changeUserData(char *username, char *key, char *value);
User logIn(char *username, char *password);
User signUp(char *username, char *password, char *phoneNumber, char *name, char *role);
User findUserWithMobile(char *username, char *phoneNumber);
// void getAllUsers(Linked_List_User *users);

Book getBook(char *id);
void addBook(Book book);
void emptyBook(Book *book);
void getAllBooks(Linked_List_Book *books);
void changeBookDataBool(char *id, char *key, bool value);
void changeBookDataString(char *id, char *key, char *value);
void changeBookDataDouble(char *id, char *key, double value);