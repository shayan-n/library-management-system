#include "models.h"
#include <string.h>

char *booksPath = "Data/books.json";
char *accountsPath = "Data/accounts.json";

int getLastId(char *path) {
    cJSON *json = JSON(path);
    cJSON *id = cJSON_GetObjectItemCaseSensitive(json, "lastId");

    return id->valueint;
}

void emptyUser(User *user) {
    user->id = 0;
    user->sex = "";
    user->name = "";
    user->role = "";
    user->address = "";
    user->username = "";
    user->lastname = "";
    user->password = "";
    user->phoneNumber = "";
}

User getUser(char *username) {
    User user;

    cJSON *json = JSON(accountsPath);
    cJSON *users = cJSON_GetObjectItemCaseSensitive(json, "users");
    cJSON *_user = cJSON_GetObjectItemCaseSensitive(users, username);

    if (_user == NULL) {
        user.id = -404;
        return user;
    }

    cJSON *id = cJSON_GetObjectItemCaseSensitive(_user, "id");
    cJSON *sex = cJSON_GetObjectItemCaseSensitive(_user, "sex");
    cJSON *name = cJSON_GetObjectItemCaseSensitive(_user, "name");
    cJSON *role = cJSON_GetObjectItemCaseSensitive(_user, "role");
    cJSON *address = cJSON_GetObjectItemCaseSensitive(_user, "address");
    cJSON *password = cJSON_GetObjectItemCaseSensitive(_user, "password");
    cJSON *lastname = cJSON_GetObjectItemCaseSensitive(_user, "lastName");
    cJSON *phoneNumber = cJSON_GetObjectItemCaseSensitive(_user, "phoneNumber");

    user.id = id->valueint;
    user.username = username;
    user.sex = sex->valuestring;
    user.name = name->valuestring;
    user.role = role->valuestring;
    user.address = address->valuestring;
    user.password = password->valuestring;
    user.lastname = lastname->valuestring;
    user.phoneNumber = phoneNumber->valuestring;

    return user;
}

void getAllUsers(Linked_List_User *users) {
    cJSON *json = JSON(accountsPath);
    cJSON *usersObject = cJSON_GetObjectItemCaseSensitive(json, "users");

    int i = 0;
    foreach_node(cJSON*, usersObject->child) {
        User newUser;
        
        cJSON *id = cJSON_GetObjectItemCaseSensitive(current, "id");
        cJSON *sex = cJSON_GetObjectItemCaseSensitive(current, "sex");
        cJSON *name = cJSON_GetObjectItemCaseSensitive(current, "name");
        cJSON *role = cJSON_GetObjectItemCaseSensitive(current, "role");
        cJSON *address = cJSON_GetObjectItemCaseSensitive(current, "address");
        cJSON *password = cJSON_GetObjectItemCaseSensitive(current, "password");
        cJSON *lastname = cJSON_GetObjectItemCaseSensitive(current, "lastName");
        cJSON *phoneNumber = cJSON_GetObjectItemCaseSensitive(current, "phoneNumber");

        newUser.id = id->valueint;
        newUser.sex = sex->valuestring;
        newUser.name = name->valuestring;
        newUser.role = role->valuestring;
        newUser.username = current->string;
        newUser.address = address->valuestring;
        newUser.password = password->valuestring;
        newUser.lastname = lastname->valuestring;
        newUser.phoneNumber = phoneNumber->valuestring;

        Append(newUser, users);
    }

    free(json);
}

void addUser(char *username, User user) {
    cJSON *json = JSON(accountsPath);
    cJSON *lastId = cJSON_GetObjectItemCaseSensitive(json, "lastId");
    cJSON *users = cJSON_GetObjectItemCaseSensitive(json, "users");
    cJSON *newUser = cJSON_AddObjectToObject(users, username);

    cJSON_AddNumberToObject(newUser, "id", user.id);
    cJSON_AddStringToObject(newUser, "sex", user.sex);
    cJSON_AddStringToObject(newUser, "name", user.name);
    cJSON_AddStringToObject(newUser, "role", user.role);
    cJSON_AddStringToObject(newUser, "address", user.address);
    cJSON_AddStringToObject(newUser, "lastName", user.lastname);
    cJSON_AddStringToObject(newUser, "password", user.password);
    cJSON_AddStringToObject(newUser, "phoneNumber", user.phoneNumber);

    cJSON_SetNumberValue(lastId, user.id);

    char *p = cJSON_Print(json);
    overwrite(accountsPath, p);
}

void changeUserData(char *username, char *key, char *value) {
    cJSON *json = JSON(accountsPath);
    cJSON *users = cJSON_GetObjectItemCaseSensitive(json, "users");
    cJSON *user = cJSON_GetObjectItemCaseSensitive(users, username);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(user, key);

    cJSON_SetValuestring(item, value);

    char *p = cJSON_Print(json);
    overwrite(accountsPath, p);
}

User logIn(char *username, char *password) {
    User user = getUser(username);

    if (user.id == -404 || strcmp(user.password, password) != 0) {
        user.id = -1;
    } 

    return user;
}

User signUp(char *username, char *password, char *phoneNumber, char *name, char *role) {
    User user = getUser(username);

    if (user.id != -404) user.id = -1; 
    else if (!isAPassword(password)) user.id = -2;
    else if (!isAPhoneNumber(phoneNumber)) user.id = -3; 

    if (user.id < 0 && user.id != -404) return user;

    user.sex = "";
    user.address = "";
    user.lastname = "";
    user.name = name;
    user.role = role;
    user.username = username;
    user.password = password;
    user.phoneNumber = phoneNumber;
    user.id = getLastId(accountsPath) + 1;

    addUser(username, user);
    return user;
}

User findUserWithMobile(char *username, char *phoneNumber) {
    User user = getUser(username);

    if (user.id == -404 || strcmp(user.phoneNumber, phoneNumber) != 0) {
        user.id = -1;
    }

    return user;
}

Book getBook(char *id) {
    Book book;

    cJSON *json = JSON(booksPath);
    cJSON *books = cJSON_GetObjectItemCaseSensitive(json, "books");
    cJSON *_book = cJSON_GetObjectItemCaseSensitive(books, id);

    if (_book == NULL) {
        book.id = NULL;
        return book;
    }

    cJSON *rate = cJSON_GetObjectItemCaseSensitive(_book, "rate");
    cJSON *title = cJSON_GetObjectItemCaseSensitive(_book, "title");
    cJSON *genre = cJSON_GetObjectItemCaseSensitive(_book, "genre");
    cJSON *author = cJSON_GetObjectItemCaseSensitive(_book, "author");
    cJSON *summary = cJSON_GetObjectItemCaseSensitive(_book, "summary");
    cJSON *isBorrow = cJSON_GetObjectItemCaseSensitive(_book, "isBorrow");
    cJSON *keywords = cJSON_GetObjectItemCaseSensitive(_book, "keywords");
    cJSON *comments = cJSON_GetObjectItemCaseSensitive(_book, "comments");
    cJSON *releaseDate = cJSON_GetObjectItemCaseSensitive(_book, "releaseDate");
    cJSON *borrowedTill = cJSON_GetObjectItemCaseSensitive(_book, "borrowedTill");

    book.id = id;
    book.rate = rate->valuedouble;
    book.title = title->valuestring;
    book.genre = genre->valuestring;
    book.author = author->valuestring;
    book.summary = summary->valuestring;
    book.isBorrow = isBorrow->valueint;
    book.keywords = keywords->valuestring;
    book.releaseDate = releaseDate->valuestring;
    book.borrowedTill = borrowedTill->valuestring;

    cJSON *current;
    cJSON_ArrayForEach(current, comments){
        Append(current->valuestring, book.comments);
    }

    return book;
}

void addBook(Book book) {
    string(newId);
    cJSON *json = JSON(booksPath);
    cJSON *lastId = cJSON_GetObjectItemCaseSensitive(json, "lastId");
    cJSON *books = cJSON_GetObjectItemCaseSensitive(json, "books");
    sprintf(newId, "%d", atoi(lastId->valuestring) + 1);
    cJSON *newBook = cJSON_AddObjectToObject(books, newId);

    cJSON_AddBoolToObject(newBook, "isBorrow", book.isBorrow);
    cJSON_AddNumberToObject(newBook, "rate", book.rate);
    cJSON_AddStringToObject(newBook, "title", book.title);
    cJSON_AddStringToObject(newBook, "summary", book.summary);
    cJSON_AddStringToObject(newBook, "author", book.author);
    cJSON_AddStringToObject(newBook, "genre", book.genre);
    cJSON_AddStringToObject(newBook, "releaseDate", book.releaseDate);
    cJSON_AddStringToObject(newBook, "borrowedTill", book.borrowedTill);
    cJSON_AddStringToObject(newBook, "keywords", book.keywords);
    cJSON *comments = cJSON_AddArrayToObject(newBook, "comments");

    foreach_node(Node_string*, book.comments->items){
        cJSON *newItem = cJSON_CreateString(current->value);
        cJSON_AddItemToArray(comments, newItem);
    }

    cJSON_SetValuestring(lastId, newId);
    free(newId);

    char *p = cJSON_Print(json);
    overwrite(booksPath, p);
}

#define SetBookData(ExtraLine)                                      \
    cJSON *json = JSON(booksPath);                                  \
    cJSON *books = cJSON_GetObjectItemCaseSensitive(json, "books"); \
    cJSON *book = cJSON_GetObjectItemCaseSensitive(books, id);      \
    cJSON *item = cJSON_GetObjectItemCaseSensitive(book, key);      \
    ExtraLine                                                       \
    char *p = cJSON_Print(json);                                    \
    overwrite(booksPath, p);

void changeBookDataString(char *id, char *key, char *value) {
    SetBookData(cJSON_SetValuestring(item, value););
}

void changeBookDataDouble(char *id, char *key, double value) {
    SetBookData(cJSON_SetNumberValue(item, value););
}

void changeBookDataBool(char *id, char *key, bool value) {
    SetBookData(cJSON_SetBoolValue(item, value););
}