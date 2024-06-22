#include "Libraries/utilities.h"

int main() {
    char *username = (char *) malloc(1000);
    char *password = (char *) malloc(1000);
    
    printf("Enter your username: ");
    scanf("%s", username);
    printf("Enter your password: ");
    scanf("%s", password);

    

    free(username);
    free(password);
}