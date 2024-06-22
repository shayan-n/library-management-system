#include "utilities.h"

struct json_object* JSON(char *path) {
    char *buffer = (char *) malloc(MAX * sizeof(char));
    FILE *file = fopen(path, "r");

    if (file == NULL) {
        printf("No such path found!");
        return NULL;
    }

    fread(buffer, MAX, 1, file);
    fclose(file);

    return json_tokener_parse(buffer);
}