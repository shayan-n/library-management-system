#include "account.h"
#include "cJSON.h"
#include <string.h>

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

    cJSON *json = JSON("Data/accounts.json");
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
    cJSON *json = JSON("Data/accounts.json");
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

int getLastId() {
    cJSON *json = JSON("Data/accounts.json");
    cJSON *id = cJSON_GetObjectItemCaseSensitive(json, "lastId");

    return id->valueint;
}

void addUser(char *username, User user) {
    cJSON *json = JSON("Data/accounts.json");
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
    overwrite("Data/accounts.json", p);
}

void changeUserData(char *username, char *key, char *value) {
    // I'm not checking for user existence, since its rarely happens and its computationally expensive
    cJSON *json = JSON("Data/accounts.json");
    cJSON *users = cJSON_GetObjectItemCaseSensitive(json, "users");
    cJSON *user = cJSON_GetObjectItemCaseSensitive(users, username);
    cJSON *item = cJSON_GetObjectItemCaseSensitive(user, key);

    cJSON_SetValuestring(item, value);

    char *p = cJSON_Print(json);
    overwrite("Data/accounts.json", p);
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
    user.id = getLastId() + 1;

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