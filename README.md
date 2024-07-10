# library-management-system
This is a university project, in which we should create a library management system with C programming language.

# Run the app

First compile the app with gcc as below:
    - gcc -o app main.c Libraries/cJSON.c Libraries/utilities.c Libraries/list.c Libraries/models.c Libraries/gui.c Libraries/errors.h

Then run the app
    - ./app

# Debug the app

First run:
    - gcc -o app main.c Libraries/cJSON.c Libraries/utilities.c Libraries/list.c Libraries/models.c Libraries/gui.c Libraries/errors.h -g

Then run gdb:
    - gdb ./app