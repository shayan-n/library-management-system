#include "account.h"
#include "cJSON.h"
#include <string.h>

typedef struct credentials {
    char *username;
    char *password;
} credentials;

void getAllUsers(Linked_List_User *users) {
    cJSON *json = JSON("Data/accounts.json");
    cJSON *usersObject = cJSON_GetObjectItemCaseSensitive(json, "users");

    int i = 0;
    foreach_node(cJSON*, usersObject->child) {
        User newUser;
        
        cJSON *id = cJSON_GetObjectItemCaseSensitive(current, "id");
        cJSON *sex = cJSON_GetObjectItemCaseSensitive(current, "sex");
        cJSON *name = cJSON_GetObjectItemCaseSensitive(current, "name");
        cJSON *address = cJSON_GetObjectItemCaseSensitive(current, "address");
        cJSON *password = cJSON_GetObjectItemCaseSensitive(current, "password");
        cJSON *lastname = cJSON_GetObjectItemCaseSensitive(current, "lastName");
        cJSON *phoneNumber = cJSON_GetObjectItemCaseSensitive(current, "phoneNumber");

        newUser.id = id->valueint;
        newUser.sex = sex->valuestring;
        newUser.name = name->valuestring;
        newUser.username = current->string;
        newUser.address = address->valuestring;
        newUser.password = password->valuestring;
        newUser.lastname = lastname->valuestring;
        newUser.phoneNumber = phoneNumber->valuestring;

        Append(newUser, users);
    }

    free(json);
}

void getAllAdmins(Linked_List_Admin *admins) {
    cJSON *json = JSON("Data/accounts.json");
    cJSON *adminsObject = cJSON_GetObjectItemCaseSensitive(json, "admins");

    foreach_node(cJSON*, adminsObject->child) {
        Admin newAdmin;

        cJSON *id = cJSON_GetObjectItemCaseSensitive(current, "id");
        cJSON *sex = cJSON_GetObjectItemCaseSensitive(current, "sex");
        cJSON *name = cJSON_GetObjectItemCaseSensitive(current, "name");
        cJSON *address = cJSON_GetObjectItemCaseSensitive(current, "address");
        cJSON *password = cJSON_GetObjectItemCaseSensitive(current, "password");
        cJSON *lastname = cJSON_GetObjectItemCaseSensitive(current, "lastName");
        cJSON *phoneNumber = cJSON_GetObjectItemCaseSensitive(current, "phoneNumber");

        newAdmin.id = id->valueint;
        newAdmin.sex = sex->valuestring;
        newAdmin.name = name->valuestring;
        newAdmin.username = current->string;
        newAdmin.address = address->valuestring;
        newAdmin.password = password->valuestring;
        newAdmin.lastname = lastname->valuestring;
        newAdmin.phoneNumber = phoneNumber->valuestring;

        Append(newAdmin, admins);
    }

    free(json);
}

int getLastId() {
    cJSON *json = JSON("Data/accounts.json");
    cJSON *id = cJSON_GetObjectItemCaseSensitive(json, "lastId");

    return id->valueint;
}

bool searchUser(struct credentials value, Node_User* head) {
    if (
        strcmp(value.username, head->value.username) == 0 &&
        strcmp(value.password, head->value.password) == 0
    ) return true;
    return false;
}

bool isUsernameTakenUser(char *username, Node_User *head) {
    if (strcmp(username, head->value.username) == 0) return true;
    return false;
}

bool isUsernameTakenAdmin(char *username, Node_Admin *head) {
    if (strcmp(username, head->value.username) == 0) return true;
    return false;
}

void addUser(char *username, User user) {
    cJSON *json = JSON("Data/accounts.json");
    cJSON *lastId = cJSON_GetObjectItemCaseSensitive(json, "lastId");
    cJSON *users = cJSON_GetObjectItemCaseSensitive(json, "users");
    cJSON *newUser = cJSON_AddObjectToObject(users, username);

    cJSON_AddNumberToObject(newUser, "id", user.id);
    cJSON_AddStringToObject(newUser, "sex", user.sex);
    cJSON_AddStringToObject(newUser, "name", user.name);
    cJSON_AddStringToObject(newUser, "address", user.address);
    cJSON_AddStringToObject(newUser, "lastName", user.lastname);
    cJSON_AddStringToObject(newUser, "password", user.password);
    cJSON_AddStringToObject(newUser, "phoneNumber", user.phoneNumber);

    cJSON_SetNumberValue(lastId, user.id);

    char *p = cJSON_Print(json);
    overwrite("Data/accounts.json", p);
}

User logIn(char *username, char *password) {
    LList(User, users);
    getAllUsers(users);

    credentials searchFor;
    searchFor.username = username;
    searchFor.password = password;

    Node_User *user = Search(searchFor, users, searchUser);
    
    if (user != NULL) return user->value;
    else {
        User nullUser; nullUser.id = -1;
        return nullUser;
    }
}

User signUp(char *username, char *password, char *phoneNumber, char *name) {
    User user;
    if (!isAPassword(password)) user.id = -1;
    else if (!isAPhoneNumber(phoneNumber)) user.id = -2; 

    LList(User, users);
    getAllUsers(users);
    LList(Admin, admins);
    getAllAdmins(admins);

    Node_User *sameUsernameUser = Search(username, users, isUsernameTakenUser);
    Node_Admin *sameUsernameAdmin = Search(username, admins, isUsernameTakenAdmin);
    if (sameUsernameUser != NULL || sameUsernameAdmin != NULL) user.id = -3; 

    if (user.id < 0) return user;

    user.sex = "";
    user.address = "";
    user.lastname = "";
    user.name = name;
    user.username = username;
    user.password = password;
    user.phoneNumber = phoneNumber;
    user.id = getLastId() + 1;

    addUser(username, user);
    return user;
}

