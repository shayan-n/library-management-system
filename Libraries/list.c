#include "list.h"

DLinkedList(string, char *);   

bool isInNode(char *value, Node_string* current) {
    if (current->value == value) return true;
    return false;
}

int main() {
    LList(string, names);

    Append((char *) "shayan1", names);
    Append((char *) "shayan2", names);
    Append((char *) "shayan3", names);
    Append((char *) "shayan4", names);

    Node_string* answer = Search("shayan3", names, isInNode);    
    printf("%s\n", answer->value);

    // Append((char *) "shayan", names);
    // Append((char *) "erfan", names);
    // Append((char *) "erfan", names);
    // Append((char *) "erfan", names);

    // printf("%d\n", names->items->value);
}