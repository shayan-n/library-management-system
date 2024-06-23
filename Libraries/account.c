#include "account.h"

int getAllUsers(User *users) {
    struct json_object *usersObject;
    struct json_object *json = JSON("../Data/accounts.json");

    json_object_object_get_ex(json, "users", &usersObject);
    users = (User *) realloc(users, json_object_object_length(usersObject) * sizeof(User));

    // uses linked lists
    int i = 0;
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

        *(users + i) = newUser;
        i += 1;
    }


    free(json);
    return i;
}

int getAllAdmins(Admin *admins) {
    struct json_object *adminsObject;
    struct json_object *json = JSON("../Data/accounts.json");

    json_object_object_get_ex(json, "admins", &adminsObject);
    admins = (Admin *) realloc(admins, json_object_object_length(adminsObject) * sizeof(Admin));

    // uses linked lists
    int i = 0;
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

        *(admins + i) = newAdmin;
        i += 1;
    }


    free(json);
    return i;
}

int main() {
    User *users = (User *) malloc(5 * sizeof(User));
    Admin *admins = (Admin *) malloc(5 * sizeof(Admin));

    int usersLen = getAllUsers(users);
    int adminsLen = getAllAdmins(admins);

    for (int i = 0; i < usersLen; i++) {
        printf("%s\n", (users + i)->name);
    }
    for (int i = 0; i < adminsLen; i++) {
        printf("%s\n", (admins + i)->name);
    }
}