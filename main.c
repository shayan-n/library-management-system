#include "Libraries/account.h"
#include "Libraries/gui.h"
#include <sys/ioctl.h>

bool darkMode = false;
struct winsize terminal;

int page = 0;
char *error = "";

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

void body(line *lines, int n, int start) {
    int eh = 8; // extra height
    for (int i = 0; i < terminal.ws_row - eh; i++) {
        rgb(26, 26, 26, darkMode);
        rgb(245, 245, 245, !darkMode);

        // errorBox
        int errorLen = (int) strlen(error);
        if (errorLen != 0 && (i == terminal.ws_row - eh - 5 || i == terminal.ws_row - eh - 6 || i == terminal.ws_row - eh - 7)) {
            char *text = CA(char); text = RA(char, text, STR_MALLOC_LENGTH);
            int errorLine = 0;

            if (i == terminal.ws_row - eh - 6) {
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
        if (i == terminal.ws_row - eh - 3 || i == terminal.ws_row - eh - 1) {
            rgb(28, 89, 115, true);
            print(" ", "", terminal.ws_col);
            setESC(BASH_NORMAL);
            continue;
        }
        if (i == terminal.ws_row - eh - 2) {
            setESC(BASH_BOLD);
            rgb(28, 89, 115, true);
            rgb(237, 250, 255, false);
            char *link = hyperLink("https://github.com/shayan-n/library-management-system", "(Project Link)");
            char *text = CA(char);
            text = RA(char, text, STR_MALLOC_LENGTH); 

            sprintf(text, "© 2024 Shayan Bajelan | %s", link);
            print(text, "center", terminal.ws_col + strlen(link) - strlen("(Project Link)") + 1);
            setESC(BASH_NORMAL);
            continue;
        }

        // body
        if (i >= start) {
            line l = *(lines + i - start);
            if (i - start < n) {
                char *spaceDesign = CA(char); spaceDesign = RA(char, spaceDesign, STR_MALLOC_LENGTH); 

                sprintf(spaceDesign, "[%d8;2;26;26;26m,[%d8;2;245;245;245m", darkMode ? 4 : 3, darkMode ? 3 : 4);
                designPrint(l.str, l.place, l.width, spaceDesign, l.design);
            }
            else print("", "", terminal.ws_col);
        } else  print("", "", terminal.ws_col);
        
        setESC(BASH_NORMAL);
    }
}

int accountPage() {
    if (page != 0) return -1; // -1 mean page skiped

    line *lines = MA(line);
    lines = RA(line, lines, 6);

    setLine(lines + 0, "1. Login           ", "center", "bold");
    setLine(lines + 1, "2. Signup          ", "center", "bold");
    setLine(lines + 2, "3. Forgot password?", "center", "bold,[38;2;26;100;219m");
    setLine(lines + 3, "", "center", "");
    setLine(lines + 4, "", "center", "");
    setLine(lines + 5, "4. Quit            ", "center", "bold");

    body(lines, 6, terminal.ws_row / 4);

    int option;
    printf(">> Select one option: ");
    scanf("%d", &option);

    if (option < 1 || option > 4) error = "  Error: Please choose an option from the list!";
    else error = "";

    if (option > 0 && option < 4) page = option;

    // -2 mean continue this page while showing the error
    return strlen(error) != 0 ? -2 : option;
}

void logInPage() {
    if (page != 1) return;

    line *lines = MA(line);
    lines = RA(line, lines, 5);

    setLine(lines + 0, "Enter your username           ", "center", "strikethrough");
    setLine(lines + 1, "-> Enter your password           ", "center", "bold");
    setLine(lines + 2, "", "center", "");
    setLine(lines + 3, "", "center", "");
    setLine(lines + 4, "For going back write 'back..'!", "center", "");

    body(lines, 5, terminal.ws_row / 4);
    
    int a;
    scanf("%d", &a);
}

int main() {
    while (true) {
        // Clearing every thing in terminal
        system("clear");

        // Getting terminal sizes
        ioctl(0, TIOCGWINSZ, &terminal);

        // Header Page 
        header();

        // If we are in account page
        int option = accountPage();
        if (option == 4) {
            system("clear");
            break;
        } else if (option != -1) continue;
            
        // If we are in login page
        logInPage();
    }
}