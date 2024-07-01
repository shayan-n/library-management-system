# library-management-system
This is a university project, in which we should create a library management system with C programming language.

using this "https://www.youtube.com/watch?v=dQyXuFWylm4"

DLinkedList(typeName, type) -> to create Node_typeName and Linked_List_typeName
    Node_typeName->value type is type
    Linked_List_typeName->length: is the count of items in your list
    Linked_List_typeName->items: is the head in the linked list

LList(typeName, name) -> an instance of Linked_List_typeName

At(index, lls) -> index of the Node_typeName in Linked_List_typeName
Append(newItem, lls) -> add the newItem to the end of the linked list
Search(value, lls, keyFunc) -> search value in lls with the function you have to sepecifi which return a bool


# To Run the file run the command below
you got to have -ljson-c pre-installed
    - gcc -o app main.c Libraries/cJSON.c Libraries/utilities.c Libraries/list.c Libraries/account.c Libraries/gui.c

then
    - ./app

# for debug
    - gcc -o app main.c Libraries/cJSON.c Libraries/utilities.c Libraries/list.c Libraries/account.c Libraries/gui.c -g
then
    - gdb ./app