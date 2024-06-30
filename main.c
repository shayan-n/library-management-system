#include "Libraries/account.h"
#include "Libraries/gui.h"
#include <sys/ioctl.h>

struct winsize terminal;

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
    for (int i = 0; i < terminal.ws_row - 8; i++) {
        rgb(26, 26, 26, false);
        rgb(245, 245, 245, true);
        
        if (i >= start) {
            line l = *(lines + i - start);
            if (i - start < n) {
                applyDesign(l.design);
                print(l.str, l.place, l.width);
                setESC(BASH_BOLD);
            }
            else print("", "", terminal.ws_col);
        } else  print("", "", terminal.ws_col);
    }
    setESC(BASH_NORMAL);
}

void accountDesign() {
    line *lines = MA(line);
    lines = RA(line, lines, 6);

    setLine(lines + 0, "1. Login           ", "center", "bold");
    setLine(lines + 1, "2. Signup          ", "center", "bold");
    setLine(lines + 2, "3. Forgot password?", "center", "bold,[38;2;26;100;219m");
    setLine(lines + 3, "", "center", "");
    setLine(lines + 4, "", "center", "");
    setLine(lines + 5, "4. Quit            ", "center", "bold");

    body(lines, 6, terminal.ws_row / 4);
}

int main() {
    // Clearing every thing in terminal
    system("clear");

    // Getting terminal sizes
    ioctl(0, TIOCGWINSZ, &terminal);
    
    header();
    accountDesign();
}

// If wanted to print a line with hyper link
// char *link = hyperLink("https://github.com/shayan-n/library-management-system", "github");
// (lines + 1)->width += strlen(link) - strlen("github");