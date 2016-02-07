/*========================================
 *    sl.c: SL version 5.02
 *        Copyright 1993,1998,2014
 *                  Toyoda Masashi
 *                  (mtoyoda@acm.org)
 *        Last Modified: 2014/06/03
 *========================================
 */

#define D51HEIGHT    9
#define D51FUNNEL    7
#define D51LENGTH   83
#define D51PATTERNS  6

#define ENG1  "      ====        ________                ___________ "
#define ENG2  "  _D _|  |_______/        \\__I_I_____===__|_________| "
#define ENG3  "   |(_)---  |   H\\________/ |   |        =|___ ___|   "
#define ENG4  "   /     |  |   H  |  |     |   |         ||_| |_||   "
#define ENG5  "  |      |  |   H  |__--------------------| [___] |   "
#define ENG6  "  | ________|___H__/__|_____/[][]~\\_______|       |   "
#define ENG7  "  |/ |   |-----------I_____I [][] []  D   |=======|__ "

#define WHL11 "__/ =| o |=-~~\\  /~~\\  /~~\\  /~~\\ ____Y___________|__ "
#define WHL12 " |/-=|___|=    ||    ||    ||    |_____/~\\___/        "
#define WHL13 "  \\_/      \\O=====O=====O=====O_/      \\_/            "

#define WHL21 "__/ =| o |=-~~\\  /~~\\  /~~\\  /~~\\ ____Y___________|__ "
#define WHL22 " |/-=|___|=O=====O=====O=====O   |_____/~\\___/        "
#define WHL23 "  \\_/      \\__/  \\__/  \\__/  \\__/      \\_/            "

#define WHL31 "__/ =| o |=-O=====O=====O=====O \\ ____Y___________|__ "
#define WHL32 " |/-=|___|=    ||    ||    ||    |_____/~\\___/        "
#define WHL33 "  \\_/      \\__/  \\__/  \\__/  \\__/      \\_/            "

#define WHL41 "__/ =| o |=-~O=====O=====O=====O\\ ____Y___________|__ "
#define WHL42 " |/-=|___|=    ||    ||    ||    |_____/~\\___/        "
#define WHL43 "  \\_/      \\__/  \\__/  \\__/  \\__/      \\_/            "

#define WHL51 "__/ =| o |=-~~\\  /~~\\  /~~\\  /~~\\ ____Y___________|__ "
#define WHL52 " |/-=|___|=   O=====O=====O=====O|_____/~\\___/        "
#define WHL53 "  \\_/      \\__/  \\__/  \\__/  \\__/      \\_/            "

#define WHL61 "__/ =| o |=-~~\\  /~~\\  /~~\\  /~~\\ ____Y___________|__ "
#define WHL62 " |/-=|___|=    ||    ||    ||    |_____/~\\___/        "
#define WHL63 "  \\_/      \\_O=====O=====O=====O/      \\_/            "

#define COAL01 "                              "
#define COAL02 "                              "
#define COAL03 "    _________________         "
#define COAL04 "   _|                \\_____A  "
#define COAL05 " =|                        |  "
#define COAL06 " -|                        |  "
#define COAL07 "__|________________________|_ "
#define COAL08 "|__________________________|_ "
#define COAL09 "   |_D__D__D_|  |_D__D__D_|   "
#define COAL10 "    \\_/   \\_/    \\_/   \\_/    "

#include <curses.h>
#include <signal.h>
#include <unistd.h>

int my_mvaddstr(int y, int x, char *str) {
    for ( ; x < 0; ++x, ++str)
        if (*str == '\0')
            return ERR;
    for ( ; *str != '\0'; ++str, ++x)
        if (mvaddch(y, x, *str) == ERR)
            return ERR;
    return OK;
}

void add_smoke(int y, int x)
#define SMOKEPTNS        16
{
    static struct smokes {
        int y, x;
        int ptrn, kind;
    } S[1000];
    static int sum = 0;
    static char *Smoke[2][SMOKEPTNS]
        = {{"(   )", "(    )", "(    )", "(   )", "(  )",
            "(  )" , "( )"   , "( )"   , "()"   , "()"  ,
            "O"    , "O"     , "O"     , "O"    , "O"   ,
            " "                                          },
           {"(@@@)", "(@@@@)", "(@@@@)", "(@@@)", "(@@)",
            "(@@)" , "(@)"   , "(@)"   , "@@"   , "@@"  ,
            "@"    , "@"     , "@"     , "@"    , "@"   ,
            " "                                          }};
    static char *Eraser[SMOKEPTNS]
        =  {"     ", "      ", "      ", "     ", "    ",
            "    " , "   "   , "   "   , "  "   , "  "  ,
            " "    , " "     , " "     , " "    , " "   ,
            " "                                          };
    static int dy[SMOKEPTNS] = { 2,  1, 1, 1, 0, 0, 0, 0, 0, 0,
                                 0,  0, 0, 0, 0, 0             };
    static int dx[SMOKEPTNS] = {-2, -1, 0, 1, 1, 1, 1, 1, 2, 2,
                                 2,  2, 2, 3, 3, 3             };
    int i;

    if (x % 4 == 0) {
        for (i = 0; i < sum; ++i) {
            my_mvaddstr(S[i].y, S[i].x, Eraser[S[i].ptrn]);
            S[i].y    -= dy[S[i].ptrn];
            S[i].x    += dx[S[i].ptrn];
            S[i].ptrn += (S[i].ptrn < SMOKEPTNS - 1) ? 1 : 0;
            my_mvaddstr(S[i].y, S[i].x, Smoke[S[i].kind][S[i].ptrn]);
        }
        my_mvaddstr(y, x, Smoke[sum % 2][0]);
        S[sum].y = y;    S[sum].x = x;
        S[sum].ptrn = 0; S[sum].kind = sum % 2;
        sum ++;
    }
}

int add_D51(int x) {
    static char *d51[D51PATTERNS][D51HEIGHT + 1]
        = {{ENG1, ENG2, ENG3, ENG4, ENG5, ENG6, ENG7, WHL11, WHL12, WHL13},
           {ENG1, ENG2, ENG3, ENG4, ENG5, ENG6, ENG7, WHL21, WHL22, WHL23},
           {ENG1, ENG2, ENG3, ENG4, ENG5, ENG6, ENG7, WHL31, WHL32, WHL33},
           {ENG1, ENG2, ENG3, ENG4, ENG5, ENG6, ENG7, WHL41, WHL42, WHL43},
           {ENG1, ENG2, ENG3, ENG4, ENG5, ENG6, ENG7, WHL51, WHL52, WHL53},
           {ENG1, ENG2, ENG3, ENG4, ENG5, ENG6, ENG7, WHL61, WHL62, WHL63}};
    static char *coal[D51HEIGHT + 1]
        = {COAL01, COAL02, COAL03, COAL04, COAL05,
           COAL06, COAL07, COAL08, COAL09, COAL10};

    int y, i, dy = 0;

    if (x < -D51LENGTH)
        return ERR;
    y = LINES / 2 - 5;

    for (i = 0; i <= D51HEIGHT; ++i) {
        my_mvaddstr(y + i, x, d51[(D51LENGTH + x) % D51PATTERNS][i]);
        my_mvaddstr(y + i + dy, x + 53, coal[i]);
    }
    add_smoke(y - 1, x + D51FUNNEL);
    return OK;
}

int main(int argc, char *argv[]) {
    int x, i;

    initscr();
    signal(SIGINT, SIG_IGN);
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    leaveok(stdscr, TRUE);
    scrollok(stdscr, FALSE);

    for (x = COLS - 1; ; --x) {
        if (add_D51(x) == ERR) break;
        getch();
        refresh();
        usleep(40000);
    }
    mvcur(0, COLS - 1, LINES - 1, 0);
    endwin();
}
