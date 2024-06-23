#include "account.h"
#include <string.h>

void getAllUsers(Linked_List_User *users) {
    struct json_object *usersObject;
    struct json_object *json = JSON("../Data/accounts.json");
    json_object_object_get_ex(json, "users", &usersObject);

    json_object_object_foreach(usersObject, key, val) {
        User newUser;
        
        struct json_object *id;
        struct json_object *sex;
        struct json_object *name;
        struct json_object *address;
        struct json_object *password;
        struct json_object *lastname;
        struct json_object *phoneNumber;

        json_object_object_get_ex(val, "id", &id);
        json_object_object_get_ex(val, "sex", &sex);
        json_object_object_get_ex(val, "name", &name);
        json_object_object_get_ex(val, "address", &address);
        json_object_object_get_ex(val, "password", &password);
        json_object_object_get_ex(val, "lastName", &lastname);
        json_object_object_get_ex(val, "phoneNumber", &phoneNumber);

        newUser.username = key;
        newUser.id = json_object_get_int(id);
        newUser.sex = (char *) json_object_get_string(sex);
        newUser.name = (char *) json_object_get_string(name);
        newUser.address = (char *) json_object_get_string(address);
        newUser.password = (char *) json_object_get_string(password);
        newUser.lastname = (char *) json_object_get_string(lastname);
        newUser.phoneNumber = (char *) json_object_get_string(phoneNumber);

        Append(newUser, users);
    }

    free(json);
}

void getAllAdmins(Linked_List_Admin *admins) {
    struct json_object *adminsObject;
    struct json_object *json = JSON("../Data/accounts.json");
    json_object_object_get_ex(json, "admins", &adminsObject);

    json_object_object_foreach(adminsObject, key, val) {
        Admin newAdmin;
        
        struct json_object *id;
        struct json_object *sex;
        struct json_object *name;
        struct json_object *address;
        struct json_object *password;
        struct json_object *lastname;
        struct json_object *phoneNumber;

        json_object_object_get_ex(val, "id", &id);
        json_object_object_get_ex(val, "sex", &sex);
        json_object_object_get_ex(val, "name", &name);
        json_object_object_get_ex(val, "address", &address);
        json_object_object_get_ex(val, "password", &password);
        json_object_object_get_ex(val, "lastName", &lastname);
        json_object_object_get_ex(val, "phoneNumber", &phoneNumber);

        newAdmin.username = key;
        newAdmin.id = json_object_get_int(id);
        newAdmin.sex = (char *) json_object_get_string(sex);
        newAdmin.name = (char *) json_object_get_string(name);
        newAdmin.address = (char *) json_object_get_string(address);
        newAdmin.password = (char *) json_object_get_string(password);
        newAdmin.lastname = (char *) json_object_get_string(lastname);
        newAdmin.phoneNumber = (char *) json_object_get_string(phoneNumber);

        Append(newAdmin, admins);
    }

    free(json);
}

typedef struct credentials {
    char *username;
    char *password;
} credentials;

bool searchUser(struct credentials value, Node_User* head) {
    if (
        strcmp(value.username, head->value.username) == 0 &&
        strcmp(value.password, head->value.password) == 0
    ) return true;
    return false;
}

User signIn(char *username, char *password) {
    LList(User, users);
    getAllUsers(users);

    credentials searchFor;
    searchFor.username = username;
    searchFor.password = password;

    Node_User *user = Search(searchFor, users, searchUser);
    printf("%s %s logged in successfully!\n", user->value.name, user->value.lastname);
    return user->value;
}

int main() {
    char *username = (char *) malloc(1000);
    char *password = (char *) malloc(1000);
    
    printf("Enter your username: ");
    scanf("%s", username);
    printf("Enter your password: ");
    scanf("%s", password);

    User user = signIn(username, password);

    free(username);
    free(password);
}