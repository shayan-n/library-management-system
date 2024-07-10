#include "Libraries/models.h"
#include "Libraries/errors.h"
#include "Libraries/gui.h"
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

bool darkMode = false;
struct winsize terminal;

int page = 0;
int delayPage = -1;

char *error = "";
char *listItemDoneDesign = "strikethrough";
char *listItemActiveDesign = "bold,[38;2;255;139;23m";
char *listItemActiveOptionDesign = "blink,bold,[38;2;255;139;23m";
char *goingBackText = "  For going back write (back..)";
char *goingBackDesgin = "italic,dim,[38;2;100;100;100m";

typedef struct line {
    int width;
    char *str;
    char *place;
    char *design;
} line;

void setLine(line *l, char *str, char *place, char *design) {
    l->str = str;
    l->place = place;
    l->design = design;
    l->width = terminal.ws_col;
}

void print(char *str, char *place, int width) {
    int spaceCountAfterStr = (int) (width / 2) - (int) (strlen(str) / 2);
    int spaceCountBeforeStr = (int) (width / 2) - (int) (strlen(str) / 2);

    if (strcmp(place, "right") == 0) {
        spaceCountBeforeStr += spaceCountAfterStr;  
        spaceCountAfterStr = 0;
    } else if (strcmp(place, "center") != 0) {
        spaceCountAfterStr += spaceCountBeforeStr;
        spaceCountBeforeStr = 0;
    }

    if (width % 2 == 0 && strlen(str) % 2 != 0) {
        if (spaceCountAfterStr != 0) spaceCountAfterStr -= 1;
        else if (spaceCountBeforeStr != 0) spaceCountBeforeStr -= 1;
    } else if (width % 2 != 0 && strlen(str) % 2 == 0) {
        if (spaceCountBeforeStr != 0) spaceCountBeforeStr += 1;
        else if (spaceCountAfterStr != 0) spaceCountAfterStr += 1;
    }

    printf("%*s%s%*s", spaceCountBeforeStr, "", str, spaceCountAfterStr, "");
}

void designPrint(char *str, char *place, int width, char *spaceDesigns, char *textDesign) {
    int spaceCountAfterStr = (int) (width / 2) - (int) (strlen(str) / 2);
    int spaceCountBeforeStr = (int) (width / 2) - (int) (strlen(str) / 2);

    if (strcmp(place, "right") == 0) {
        spaceCountBeforeStr += spaceCountAfterStr;  
        spaceCountAfterStr = 0;
    } else if (strcmp(place, "center") != 0) {
        spaceCountAfterStr += spaceCountBeforeStr;
        spaceCountBeforeStr = 0;
    }

    if (width % 2 == 0 && strlen(str) % 2 != 0) {
        if (spaceCountAfterStr != 0) spaceCountAfterStr -= 1;
        else if (spaceCountBeforeStr != 0) spaceCountBeforeStr -= 1;
    } else if (width % 2 != 0 && strlen(str) % 2 == 0) {
        if (spaceCountBeforeStr != 0) spaceCountBeforeStr += 1;
        else if (spaceCountAfterStr != 0) spaceCountAfterStr += 1;
    }

    applyDesign(spaceDesigns);
    printf("%*s", spaceCountBeforeStr, "");
    
    applyDesign(textDesign);
    printf("%s", str);
    setESC(BASH_NORMAL);

    applyDesign(spaceDesigns);
    printf("%*s", spaceCountAfterStr, "");
    setESC(BASH_NORMAL);
}

void header() {
    setESC(BASH_BOLD);
    rgb(28, 89, 115, true);
    rgb(237, 250, 255, false);
    print("", "left", terminal.ws_col);
    print("", "left", terminal.ws_col);
    print("", "left", terminal.ws_col);
    print("Welcome to our library management system!", "center", terminal.ws_col);
    print("", "right", terminal.ws_col);
    print("", "right", terminal.ws_col);
    print("", "right", terminal.ws_col);
    setESC(BASH_NORMAL);
}

void body(line *lines, int n, int start, int deleteRow) {
    int eh = 8 + deleteRow; // extra height
    int extra = start + n + 7 > terminal.ws_row - eh ? start + n + 7 - terminal.ws_row + eh : 0;
    int length = terminal.ws_row - eh + extra;

    for (int i = 0; i < length; i++) {
        rgb(26, 26, 26, darkMode);
        rgb(245, 245, 245, !darkMode);

        // errorBox
        int errorLen = (int) strlen(error);
        if (errorLen != 0 && (i == length - 5 || i == length - 6 || i == length - 7)) {
            string(text);
            int errorLine = 0;

            if (i == length - 6) {
                sprintf(text, "%s", error);
                errorLine = 4;
            } else {
                sprintf(text, "%*s", errorLen + 4, " ");
            }

            int width = strlen(text);
            setESC(BASH_BOLD);
            setESC(BASH_WHITE_BRIGHT_YELLOW);
            printf("  ");
            rgb(255, 23, 50, true);           
            rgb(245, 245, 245, false);           
            print(text, "left", width + errorLine);
            rgb(26, 26, 26, darkMode);
            rgb(245, 245, 245, !darkMode);
            print("", "", terminal.ws_col - width - errorLine - 2);
            setESC(BASH_NORMAL);
            continue;
        }

        // footer
        if (i == length - 3 || i == length - 1) {
            rgb(28, 89, 115, true);
            print(" ", "", terminal.ws_col);
            setESC(BASH_NORMAL);
            continue;
        }
        if (i == length - 2) {
            setESC(BASH_BOLD);
            rgb(28, 89, 115, true);
            rgb(237, 250, 255, false);
            char *link = hyperLink("https://github.com/shayan-n/library-management-system", "(Project Link)");
            string(text);

            sprintf(text, "© 2024 Shayan Bajelan | %s", link);
            print(text, "center", terminal.ws_col + strlen(link) - strlen("(Project Link)") + 1);
            setESC(BASH_NORMAL);
            continue;
        }

        // body
        if (i >= start) {
            line l = *(lines + i - start);
            if (i - start < n) {
                string(spaceDesign);

                sprintf(spaceDesign, "[%d8;2;26;26;26m,[%d8;2;245;245;245m", darkMode ? 4 : 3, darkMode ? 3 : 4);
                designPrint(l.str, l.place, l.width, spaceDesign, l.design);
            }
            else print("", "", terminal.ws_col);
        } else  print("", "", terminal.ws_col);
        
        setESC(BASH_NORMAL);
    }
}

// page 0
int accountPage(User *user, int *activeAction) {
    if (page != 0) return -1; // -1 mean page skiped

    line *lines = MA(line);
    lines = RA(line, lines, 6);

    setLine(lines + 0, "1. Login           ", "center", "bold");
    setLine(lines + 1, "2. Signup          ", "center", "bold");
    setLine(lines + 2, "3. Forgot password?", "center", "bold,[38;2;26;100;219m");
    setLine(lines + 3, "", "center", "");
    setLine(lines + 4, "", "center", "");
    setLine(lines + 5, "4. Quit            ", "center", "bold");

    body(lines, 6, terminal.ws_row / 4, 0);

    string(optionStr);
    printf(">> Select an option: ");
    get(optionStr, STR_MALLOC_LENGTH);
    int option = atoi(optionStr);

    if (option < 1 || option > 4) error = WRONG_OPTION_ERR;
    else error = "";

    if (option > 0 && option < 4) {
        page = option;
        
        emptyUser(user);
        if (page == 1) *activeAction = 0;
        if (page == 2) *activeAction = 3;
        if (page == 3) *activeAction = 8;
    }

    // -2 mean continue this page while showing the error
    return strlen(error) != 0 ? -2 : option;
}

// page 1 activeAction 0 - 2
void logInPage(User *user, int* activeAction) {
    if (page != 1) return; //actions are 0 and 1

    line *lines = MA(line);
    lines = RA(line, lines, 7);

    int action = *activeAction;
    string(line1);
    sprintf(line1, "%s 1. Enter your username      ", action == 0 ? "->" : "  ");
    string(line2);
    sprintf(line2, "%s 2. Enter your password      ", action == 1 ? "->" : "  ");

    setLine(lines + 0, "             Login             ", "center", "bold");
    setLine(lines + 1, "", "center", "");
    setLine(lines + 2, line1, "center", action == 0 ? listItemActiveDesign : listItemDoneDesign);
    setLine(lines + 3, line2, "center", action == 1 ? listItemActiveDesign : action > 1 ? listItemDoneDesign : "");
    setLine(lines + 4, "", "center", "");
    setLine(lines + 5, "", "center", "");
    setLine(lines + 6, goingBackText, "center", goingBackDesgin);

    body(lines, 7, terminal.ws_row / 4, 0);

    string(input);
    if (action == 0) {
        printf(">> Username: ");
        get(input, STR_MALLOC_LENGTH);
        user->username = input;
    }
    else if (action == 1) {
        printf(">> Password: ");
        get(input, STR_MALLOC_LENGTH);
        user->password = input;
    }

    if (strcmp(input, "back..") == 0) {
        page = 0;
        error = "";
        *activeAction = 0;
        return;
    }
    
    *(activeAction) += 1;

    if (*activeAction == 2) {
        *user = logIn(user->username, user->password);

        if (user->id == -1) {
            error = WRONG_CREDENTIALS_ERR;
            *(activeAction) = 0;
        } 
        else {
            page = 0;
            delayPage = 5;
            error = "";
            *(activeAction) = 14;
        } 
    }
}

// page 2 activeAction 3 - 7
void signUpPage(User *user, int* activeAction) {
    if (page != 2) return; // action from 3 

    line *lines = MA(line);
    lines = RA(line, lines, 9);

    int action = *activeAction;

    string(line1);
    sprintf(line1, "%s 2. Enter your name           ", action == 3 ? "->" : "  ");
    string(line2);
    sprintf(line2, "%s 1. Enter your username       ", action == 4 ? "->" : "  ");
    string(line3);
    sprintf(line3, "%s 2. Enter your password       ", action == 5 ? "->" : "  ");
    string(line4);
    sprintf(line4, "%s 2. Enter your phone number   ", action == 6 ? "->" : "  ");

    setLine(lines + 0, "             Signup            ", "center", "bold");
    setLine(lines + 1, "", "center", "");
    setLine(lines + 2, line1, "center", action == 3 ? listItemActiveDesign : listItemDoneDesign);
    setLine(lines + 3, line2, "center", action == 4 ? listItemActiveDesign : action > 4 ? listItemDoneDesign : "");
    setLine(lines + 4, line3, "center", action == 5 ? listItemActiveDesign : action > 5 ? listItemDoneDesign : "");
    setLine(lines + 5, line4, "center", action == 6 ? listItemActiveDesign : action > 6 ? listItemDoneDesign : "");
    setLine(lines + 6, "", "center", "");
    setLine(lines + 7, "", "center", "");
    setLine(lines + 8, goingBackText, "center", goingBackDesgin);

    body(lines, 9, terminal.ws_row / 4, 0);

    string(input);
    if (action == 3) {
        printf(">> Name: ");
        get(input, STR_MALLOC_LENGTH);
        user->name = input;
    } else if (action == 4) {
        printf(">> Username: ");
        get(input, STR_MALLOC_LENGTH);
        user->username = input;
    } else if (action == 5) {
        printf(">> Password: ");
        get(input, STR_MALLOC_LENGTH);
        user->password = input;
    } else if (action == 6) {
        printf(">> Phone number: ");
        get(input, STR_MALLOC_LENGTH);
        user->phoneNumber = input;
    }

    if (strcmp(input, "back..") == 0) {
        page = 0;
        error = "";
        *activeAction = 0;
        return;
    }

    *(activeAction) += 1;

    if (*activeAction == 7) {
        char *name = user->name;
        char *username = user->username;
        char *password = user->password;
        char *phoneNumber = user->phoneNumber;
        User newUser = signUp(username, password, phoneNumber, name, "user");

        if (newUser.id == -1) {
            error = TAKEN_USERNAME_ERR;
            *(activeAction) = 4;
        } else if (newUser.id == -2) {
            error = INVALID_PASSWORD_ERR;
            *(activeAction) = 5;
        } else if (newUser.id == -3) {
            error = INVALID_PHONENUMBER_ERR;
            *(activeAction) = 6;
        } else {
            page = 0;
            delayPage = 5;
            error = "";
            *(activeAction) = 14;
        } 
    }
}

// page 3 activeAction 8 - 10
void forgetPasswordPage(User *user, int* activeAction) {
    if (page != 3) return; 

    line *lines = MA(line);
    lines = RA(line, lines, 7);

    int action = *activeAction;
    string(line1);
    sprintf(line1, "%s 1. Enter your username      ", action == 8 ? "->" : "  ");
    string(line2);
    sprintf(line2, "%s 2. Enter your phone number  ", action == 9 ? "->" : "  ");

    setLine(lines + 0, "     Forget your password?     ", "center", "bold");
    setLine(lines + 1, "", "center", "");
    setLine(lines + 2, line1, "center", action == 8 ? listItemActiveDesign : listItemDoneDesign);
    setLine(lines + 3, line2, "center", action == 9 ? listItemActiveDesign : action > 9 ? listItemDoneDesign : "");
    setLine(lines + 4, "", "center", "");
    setLine(lines + 5, "", "center", "");
    setLine(lines + 6, goingBackText, "center", goingBackDesgin);

    body(lines, 7, terminal.ws_row / 4, 0);

    string(input);
    if (action == 8) {
        printf(">> Username: ");
        get(input, STR_MALLOC_LENGTH);
        user->username = input;
    }
    else if (action == 9) {
        printf(">> Phone number: ");
        get(input, STR_MALLOC_LENGTH);
        user->phoneNumber = input;
    }

    if (strcmp(input, "back..") == 0) {
        page = 0;
        error = "";
        *activeAction = 0;
        return;
    }

    *(activeAction) += 1;

    if (*activeAction == 10) {
        *user = findUserWithMobile(user->username, user->phoneNumber);

        if (user->id == -1) {
            error = NOUSER_FOUND_WRONG_INPUT_ERR;
            *(activeAction) = 8;
        } 
        else {
            page = 0;
            delayPage = 4;
            error = "";
            *(activeAction) = 11;
        } 
    }
}

// page 4 activeAction 11 - 13
void setNewPasswordPage(User *user, int* activeAction) {
    if (page != 4) return; 

    line *lines = MA(line);
    lines = RA(line, lines, 7);

    int action = *activeAction;
    string(line1);
    sprintf(line1, "%s 1. Enter your new password  ", action == 11 ? "->" : "  ");
    string(line2);
    sprintf(line2, "%s 2. Confirm the new password ", action == 12 ? "->" : "  ");

    setLine(lines + 0, "       Set new password!       ", "center", "bold");
    setLine(lines + 1, "", "center", "");
    setLine(lines + 2, line1, "center", action == 11 ? listItemActiveDesign : listItemDoneDesign);
    setLine(lines + 3, line2, "center", action == 12 ? listItemActiveDesign : action > 12 ? listItemDoneDesign : "");
    setLine(lines + 4, "", "center", "");
    setLine(lines + 5, "", "center", "");
    setLine(lines + 6, goingBackText, "center", goingBackDesgin);

    body(lines, 7, terminal.ws_row / 4, 0);

    string(input);
    if (action == 11) {
        printf(">> New password: ");
        get(input, STR_MALLOC_LENGTH);
        user->password = input;
    }
    else if (action == 12) {
        printf(">> Confirm password: ");
        get(input, STR_MALLOC_LENGTH);
    }

    if (strcmp(input, "back..") == 0) {
        page = 0;
        error = "";
        *activeAction = 0;
        return;
    }
    else if (action == 12 && strcmp(input, user->password) != 0) {
        error = PASSWORD_DONOT_MATCH_ERR;
        *(activeAction) = 11;
        return;
    }

    *(activeAction) += 1;

    if (*activeAction == 13) {
        if (!isAPassword(user->password)) {
            error = INVALID_PASSWORD_ERR;
            *(activeAction) = 11;
        } 
        else {
            changeUserData(user->username, "password", user->password);

            page = 0;
            error = "";
            *(activeAction) = 14;
        } 
    }
}

// 2 line menu design
void menu() {
    int pages[4] = { 7, 8, 9, 6 };
    char *menuItems = " |A. Donate Book|  |B. Borrow Book|  |C. Books list|  |D. Setting| ";
    char *items[4] = { " |A. Donate Book| ", " |B. Borrow Book| ", " |C. Books list| ", " |D. Setting| " };
    char *space[4] = { "                  ", "                  ", "                 ", "              " };

    int spaceBeforeAndAfter = (int) (terminal.ws_col / 2) - (int) (strlen(menuItems) / 2);
    int cutSpace = (terminal.ws_col % 2 == 0 && strlen(menuItems) % 2 != 0) ? 1 : 0;

    // items
    rgb(28, 89, 115, true);
    print("", "left",  spaceBeforeAndAfter - cutSpace);
    for (int i = 0; i < 4; i++) {
        print(page != pages[i] ? items[i] : space[i], "center", strlen(space[i]));
    }
    print("", "left",  spaceBeforeAndAfter);
    setESC(BASH_NORMAL);

    // line under Items
    if (!darkMode) rgb(245, 245, 245, true);
    else rgb(26, 26, 26, true);

    print("", "left",  spaceBeforeAndAfter - cutSpace);
    for (int i = 0; i < 4; i++) {
        if (!darkMode) rgb(245, 245, 245, true);
        else rgb(26, 26, 26, true);

        if (page == pages[i]) {
            setESC(BASH_BOLD);
            rgb(28, 89, 115, true);
        }

        print(page == pages[i] ? items[i] : space[i], "center", strlen(space[i]));
        setESC(BASH_NORMAL);
    }
    if (!darkMode) rgb(245, 245, 245, true);
    else rgb(26, 26, 26, true);
    
    print("", "left",  spaceBeforeAndAfter);
    setESC(BASH_NORMAL);
}

// navigating functionality
bool menuNavigation(char *input, int* activeAction) {
    bool isA = strcmp(input, "A") == 0;
    bool isB = strcmp(input, "B") == 0;
    bool isC = strcmp(input, "C") == 0;
    bool isD = strcmp(input, "D") == 0;

    if (isA || isB || isC || isD) {
        page = 0;
        error = "";
        delayPage = isA ? 7 : isB ? 8 : isC ? 9 : 6;
        *activeAction = isA ? 22 : isB ? 14 : isC ? 14 : 14;

        return true;
    } else {
        return false;
    }
}

// page 5 - no activeAction 
void homePage(User *user, int* activeAction) {
    if (page != 5) return;

    line *lines = MA(line);
    lines = RA(line, lines, 38);

    menu();

    // other lines
    char *coffeeDesign = "[38;2;184;184;184m";
    setLine(lines + 0, "              Home Page               ", "center", "bold");
    setLine(lines + 1, "Please choose an option from the menu!", "center", "bold,underline");
    setLine(lines + 2, "", "center", "");
    setLine(lines + 3, "", "center", "");
    setLine(lines + 2 + 2, "                 1111   111    1111               ", "center", coffeeDesign);
    setLine(lines + 3 + 2, "                  111    111    1111              ", "center", coffeeDesign);
    setLine(lines + 4 + 2, "                   111    111    111              ", "center", coffeeDesign);
    setLine(lines + 5 + 2, "                   111    111    111              ", "center", coffeeDesign);
    setLine(lines + 6 + 2, "                  111    1111   1111              ", "center", coffeeDesign);
    setLine(lines + 7 + 2, "                1111    111    1111               ", "center", coffeeDesign);
    setLine(lines + 8 + 2, "              1111    1111   1111                 ", "center", coffeeDesign);
    setLine(lines + 9 + 2, "             1111   1111   1111                   ", "center", coffeeDesign);
    setLine(lines + 10 + 2, "            1111   1111   1111                    ", "center", coffeeDesign);
    setLine(lines + 11 + 2, "            1111   1111   1111                    ", "center", coffeeDesign);
    setLine(lines + 12 + 2, "             111    1111   1111                   ", "center", coffeeDesign);
    setLine(lines + 13 + 2, "              1111   1111   1111                  ", "center", coffeeDesign);
    setLine(lines + 14 + 2, "               11111   1111   1111                ", "center", coffeeDesign);
    setLine(lines + 15 + 2, "                 1111    111    111               ", "center", coffeeDesign);
    setLine(lines + 16 + 2, "                   111    111    111              ", "center", coffeeDesign);
    setLine(lines + 17 + 2, "                   111    111    111              ", "center", coffeeDesign);
    setLine(lines + 18 + 2, "                  1111   1111    111              ", "center", coffeeDesign);
    setLine(lines + 19 + 2, "                 111    1111   1111               ", "center", coffeeDesign);
    setLine(lines + 20 + 2, "                11     111     11                 ", "center", coffeeDesign);
    setLine(lines + 21 + 2, "          11111111111111111111111111111111        ", "center", coffeeDesign);
    setLine(lines + 22 + 2, "111111111111  1111111111111111111111111111        ", "center", coffeeDesign);
    setLine(lines + 23 + 2, "111111111111  1111111111111111111111111111        ", "center", coffeeDesign);
    setLine(lines + 24 + 2, "111       11  1111111111111111111111111111        ", "center", coffeeDesign);
    setLine(lines + 25 + 2, "111       11  1111111111111111111111111111        ", "center", coffeeDesign);
    setLine(lines + 26 + 2, "111       11  1111111111111111111111111111        ", "center", coffeeDesign);
    setLine(lines + 27 + 2, "111       11  1111111111111111111111111111        ", "center", coffeeDesign);
    setLine(lines + 28 + 2, " 11111111111  1111111111111111111111111111        ", "center", coffeeDesign);
    setLine(lines + 29 + 2, " 11111111111  1111111111111111111111111111        ", "center", coffeeDesign);
    setLine(lines + 30 + 2, "          11   111111111111111111111111111        ", "center", coffeeDesign);
    setLine(lines + 31 + 2, "          111   1111111111111111111111111         ", "center", coffeeDesign);
    setLine(lines + 32 + 2, "            1111111111111111111111111111          ", "center", coffeeDesign);
    setLine(lines + 33 + 2, "11                                             111", "center", coffeeDesign);
    setLine(lines + 34 + 2, "11111111111111111111111111111111111111111111111111", "center", coffeeDesign);
    setLine(lines + 35 + 2, "  1111111111111111111111111111111111111111111111  ", "center", coffeeDesign);

    body(lines, 38, 3, 2);

    string(input);
    printf(">> Select an option: ");
    get(input, STR_MALLOC_LENGTH);

    bool navStatus = menuNavigation(input, activeAction);
    if (!navStatus) error = WRONG_OPTION_ERR;
}

// page 6 options
void changeDarkMode(int* activeAction) {
    string(input);
    printf(">> Dark mode (on / off)? ");
    get(input, STR_MALLOC_LENGTH);

    bool isOn = strcmp(input, "on") == 0;
    bool isOff = strcmp(input, "off") == 0;

    if (!isOn && !isOff) {
        error = WRONG_INPUT_ERR;
    } else {
        error = "";
        darkMode = isOn;
        *activeAction = 14;
    }
}

void changeUserSex(User *user, int* activeAction) {
    string(input);
    printf(">> Your sex (male, female)? ");
    get(input, STR_MALLOC_LENGTH);

    bool isMale = strcmp(input, "male") == 0;
    bool isFemale = strcmp(input, "female") == 0;

    if (!isMale && !isFemale) {
        error = WRONG_INPUT_ERR;
    } else {
        error = "";
        user->sex = isFemale ? "female" : "male";
        changeUserData(user->username, "sex", user->sex);
        *activeAction = 14;
    }
}

void changeUserName(User *user, int* activeAction) {
    string(input);
    printf(">> Set your name: ");
    get(input, STR_MALLOC_LENGTH);

    error = "";
    user->name = input;
    changeUserData(user->username, "name", user->name);
    *activeAction = 14;
}

void changeUserLastname(User *user, int* activeAction) {
    string(input);
    printf(">> Set your last name: ");
    get(input, STR_MALLOC_LENGTH);

    error = "";
    user->lastname = input;
    changeUserData(user->username, "lastName", user->lastname);
    *activeAction = 14;
}

void changeUserPhonenumber(User *user, int* activeAction) {
    string(input);
    printf(">> Set your phone number: ");
    get(input, STR_MALLOC_LENGTH);

    if (!isAPhoneNumber(input)) {
        error = INVALID_PHONENUMBER_ERR;        
    } else {
        error = "";
        user->phoneNumber = input;
        changeUserData(user->username, "phoneNumber", user->phoneNumber);
        *activeAction = 14;
    }
}

void changeUserAddress(User *user, int* activeAction) {
    string(input);
    printf(">> Set your address: ");
    get(input, STR_MALLOC_LENGTH);

    error = "";
    user->address = input;
    changeUserData(user->username, "address", user->address);
    *activeAction = 14;
}

void signOut(User *user, int* activeAction) {
    page = 0;
    error = "";
    *activeAction = 0;

    emptyUser(user);
}

// page 6 activeAction 14 - 21 
// activeAction: 14 => in selecting option state 
void settingPage(User *user, int* activeAction) {
    if (page != 6) return;

    line *lines = MA(line);
    lines = RA(line, lines, 13);
    menu();

    int maxLength = 0;
    int action = *activeAction;
    for (int i = 0;i < 6; i++) {
        string(item);
        item = "off";

        if (i == 1)      { item = user->sex; }
        else if (i == 2) { item = user->name; }
        else if (i == 3) { item = user->lastname; }
        else if (i == 4) { item = user->phoneNumber; }
        else if (i == 5) { item = user->address; }
        
        int length = strlen(item);

        if (i == 0 || i == 3) length += 14;
        if (i == 1) length += 8;
        if (i == 2) length += 9;
        if (i == 4) length += 17;
        if (i == 5) length += 12;

        if (length > maxLength) maxLength = length;
    }

    string(line0);
    string(line1);
    string(line2);
    string(line3);
    string(line4);
    string(line5);
    string(line6);

    // max length is always more than 17 because default item above is off which has len 3 and the "1. Dark mode: " has 14
    sprintf(line0, "1. Dark mode: %s%*s", darkMode ? "On " : "Off", maxLength - 17, "");
    sprintf(line1, "2. Sex: %s%*s", user->sex, maxLength - 8 - (int)strlen(user->sex), "");
    sprintf(line2, "3. Name: %s%*s", user->name, maxLength - 9 - (int)strlen(user->name), "");
    sprintf(line3, "4. Last name: %s%*s", user->lastname, maxLength - 14 - (int)strlen(user->lastname), "");
    sprintf(line4, "5. Phone number: %s%*s", user->phoneNumber, maxLength - 17 - (int)strlen(user->phoneNumber), "");
    sprintf(line5, "6. Address: %s%*s", user->address, maxLength - 12 - (int)strlen(user->address), "");
    sprintf(line6, "7. Signout %*s", maxLength - 11, "");

    setLine(lines + 0, line0, "center", action == 15 ? listItemActiveOptionDesign : "");
    setLine(lines + 1, "", "center", "");
    setLine(lines + 2, line1, "center", action == 16 ? listItemActiveOptionDesign : "");
    setLine(lines + 3, line2, "center", action == 17 ? listItemActiveOptionDesign : "");
    setLine(lines + 4, line3, "center", action == 18 ? listItemActiveOptionDesign : "");
    setLine(lines + 5, line4, "center", action == 19 ? listItemActiveOptionDesign : "");
    setLine(lines + 6, line5, "center", action == 20 ? listItemActiveOptionDesign : "");
    setLine(lines + 7, "", "center", "");
    setLine(lines + 8, "", "center", "");
    setLine(lines + 9, "", "center", "");
    setLine(lines + 10, line6, "center", "[38;2;255;23;50m,bold");
    setLine(lines + 11, "", "center", "");
    setLine(lines + 12, goingBackText, "center", goingBackDesgin);

    body(lines, 13, terminal.ws_row / 4, 2);

    if (action == 15) {
        changeDarkMode(activeAction);
        return;
    } else if (action == 16) {
        changeUserSex(user, activeAction);
        return;
    } else if (action == 17) {
        changeUserName(user, activeAction);
        return;
    } else if (action == 18) {
        changeUserLastname(user, activeAction);
        return;
    } else if (action == 19) {
        changeUserPhonenumber(user, activeAction);
        return;
    } else if (action == 20) {
        changeUserAddress(user, activeAction);
        return;
    } else if (action == 21) {
        signOut(user, activeAction);
        return;
    }

    string(optionStr);
    printf(">> Select an option: ");
    get(optionStr, STR_MALLOC_LENGTH);

    if (strcmp(optionStr, "back..") == 0) {
        page = 5;
        error = "";
        *activeAction = 14;
        return;
    }
    bool navStatus = menuNavigation(optionStr, activeAction);
    if (navStatus) return;

    int option = atoi(optionStr);
    if (option < 1 || option > 7) {
        error = WRONG_OPTION_ERR;
        return;
    } 

    error = "";
    if (option == 1) *activeAction = 15;
    else if (option == 2) *activeAction = 16;
    else if (option == 3) *activeAction = 17;
    else if (option == 4) *activeAction = 18;
    else if (option == 5) *activeAction = 19;
    else if (option == 6) *activeAction = 20;
    else if (option == 7) *activeAction = 21;
}

// page 7 activeAction 22
void donateBookPage(User *user, Book *book, int* activeAction) {
    if (page != 7) return;

    int activeActionStarter = 22;
    line *lines = MA(line);
    lines = RA(line, lines, 11);
    menu();

    LList(string, stringLines);
    char *items[7] = { 
        "     %s 1. Title: %s",
        "     %s 2. Author: %s",
        "     %s 3. Genre: %s",
        "     %s 4. Keywords: %s",
        "     %s 5. Summary: %s",
        "     %s 6. Release Date: %s"
        };

    for (int i = 0;i < 7; i++) {
        string(line);
        sprintf(line, items[i], *activeAction == activeActionStarter + i ? "->" : "  ", "");
        Append(line, stringLines);
    }

    setLine(lines + 0, "Enter your book details!", "center", "bold");
    setLine(lines + 1, "", "center", "");
    for (int i = 0;i < 7; i++) {
        setLine(lines + 2 + i, At(i, stringLines)->value, "left", *activeAction == activeActionStarter + i ? listItemActiveDesign : *activeAction > activeActionStarter + i ? listItemDoneDesign : "");
    }    
    setLine(lines + 9, "", "center", "");
    setLine(lines + 10, goingBackText, "center", goingBackDesgin);

    body(lines, 11, terminal.ws_row / 4, 2);

    string(input);
    bool navStatus = menuNavigation(input, activeAction);
    if (navStatus) return;
    char *qs[7] = { 
        ">> Book title: ",
        ">> Book author: ",
        ">> Book genre: ",
        ">> Book keywords: ",
        ">> Book summary: ",
        ">> Book release date: "
    };

    printf("%s", qs[*activeAction - activeActionStarter]);
    get(input, STR_MALLOC_LENGTH);

    if (*activeAction == activeActionStarter + 0) book->title = input;
    if (*activeAction == activeActionStarter + 1) book->author = input;
    if (*activeAction == activeActionStarter + 2) book->genre = input;
    if (*activeAction == activeActionStarter + 3) book->keywords = input;
    if (*activeAction == activeActionStarter + 4) book->summary = input;
    if (*activeAction == activeActionStarter + 5) book->releaseDate = input;

    *activeAction += 1;
    if (*activeAction == 28) {
        addBook(*book);
        page = 0;
        delayPage = 5;
        error = "";
        *activeAction = 14;
    }
}

int main() {
    User *user = MA(User); 
    Book *book = MA(Book);

    int activeAction = 0;
    user->id = 0;

    while (true) {
        if (delayPage != -1) {
            page = delayPage;
            delayPage = -1;
        }

        // Starter
        printf("\n");
        // Clearing every thing in terminal
        system("clear");

        // Getting terminal sizes
        ioctl(0, TIOCGWINSZ, &terminal);

        // Header Page 
        header();

        // If we are in account page
        int option = accountPage(user, &activeAction);
        if (option == 4) {
            system("clear");
            break;
        } else if (option != -1) continue;
            
        logInPage(user, &activeAction);
        signUpPage(user, &activeAction);
        forgetPasswordPage(user, &activeAction);
        setNewPasswordPage(user, &activeAction);

        homePage(user, &activeAction);
        settingPage(user, &activeAction);
        donateBookPage(user, book, &activeAction);
    }
}