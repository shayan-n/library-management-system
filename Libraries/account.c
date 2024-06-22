#include "account.h"

User *getAllUsers() {
    struct json_object *usersObject;
    struct json_object *json = JSON("../Data/accounts.json");

    json_object_object_get_ex(json, "users", &usersObject);
    User *users = (User *) malloc(json_object_object_length(usersObject) * sizeof(User));

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
    return users;
}

int main() {
    // User *users = getAllUsers();
    int numbers[] = {1, 2, 3, 4, 3, 3, 3, 3, 3, 3, 3};
    int *p = numbers;

    printf("%d", (int) ((*(p + 3)) / 2));


    // printf("%s", users[0].name);
}