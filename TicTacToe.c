/*****************************************************************************
* File:      TicTacToe.c                                                     *
* Unit:      COMP1000 Unix and C Programming                                 *
* Reference: None.                                                           *
* Comments:  Continue on a many errors as possible.                          *
* Requires:  LinkedList.h, effect.h, FileInput.h, stdio.h, stdlib.h, math.h  *
* Created:   19/09/2019 * Last Modified: 01/10/2019                          *
*****************************************************************************/
#include "TicTacToe.h"
#include "FileInput.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
#include <string.h>
#include <locale.h>

/* Constants */
#define LOGSTR_LEN 101

/* Forward declarations */
void showSettings(void*, Option* self);
void showLogs(void*, Option* self);
void saveLogs(void*, Option* self);
void printLogs(Playboard*, FILE*);
void editMNK(void*, Option* self);
void freeLog( void* );
int validateMNK(int m, int n, int k, int mMax, int nMax);
void setMenu(Menu*, void*);

/*****************************************************************************
* Name: main
* Purpose: Do all the things. It's main.
* Imports: argc, argv, the number and contents of commandline arguments
* Exports: error code, 0 for success.
*****************************************************************************/
int main( int argc, char **argv )
{
    static struct termios oldt, newt;
    struct winsize w;
    int m, n, k, nRead, mMax, nMax, valid;
    Playboard* pb = NULL;
    Menu* menu = NULL;

    setlocale(LC_ALL, "");

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    srand(time(NULL));
    mMax = (w.ws_col-3)/4;
    nMax = (w.ws_row-1)/2;
    
    if ( argc == 4 ) /* Command line mnk */
    {
        m = (argv[1][0]=='m' ? mMax : atoi(argv[1]));
        n = (argv[2][0]=='m' ? nMax : atoi(argv[2]));
        k = (argv[3][0]=='m' ? (mMax > nMax ? mMax : nMax) : atoi(argv[3]));
        valid = validateMNK(m, n, k, mMax, nMax);
        pb = newPlayboard( m, n, k );
    }
    else if ( argc == 2 ) /* File input mnk */
    {
        readSettings( argv[1], &pb );
        if (pb == NULL)
        {
            printf("Invalid settings file\n");
            valid = 6;
        }
        else
        {
            valid = validateMNK(pb->m, pb->n, pb->k, mMax, nMax);
        }
        
    }
    else /* Invalid args, ask user for mnk */
    {
        printf("Please enter the width, height and win length of the board\n");
        nRead = scanf("%d %d %d", &m, &n, &k);
        if (nRead == 3)
        {
            valid = validateMNK(m, n, k, mMax, nMax);
            pb = newPlayboard( m, n, k );
        }
        else
        {
            printf("Invalid inputs, try again next time\n");
            valid = 1;
        }
    }

    if (!valid)
    {
        /* Disable echo and enable input per char on terminal */
        tcgetattr( STDIN_FILENO, &oldt );
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr( STDIN_FILENO, TCSANOW, &newt);
        menu = makeMenu();/* Make the menu */
        setMenu(menu, pb);

        showMenu(menu);/* Main menu loop */

        printf("\033[27m\n\n");
        tcsetattr( STDIN_FILENO, TCSANOW, &oldt); /* Reset terminal */

        /* Free everything */
        for (m = 0; m < pb->m; m++)
        {
            free(pb->board[m]);
        }
        free(pb->board);
        free(pb->settings);
        freeLinkedList(pb->logs, &freeLog);
        free(pb);
        freeMenu(menu);
    }

    return valid;
}

int validateMNK(int m, int n, int k, int mMax, int nMax)
{
    int valid = 0;
    if (m < 2 || n < 2 || k < 2)
    {
        printf("Minimum mnk is 2,2,2\n");
        valid = 2;
    }
    else if (m > mMax)
    {
        printf("Maximum m is %d\n", mMax);
        valid = 3;
    }
    else if (n > nMax)
    {
        printf("Maximum n is %d\n", nMax);
        valid = 4;
    }
    else if (k > m && k > n)
    {
        printf("m or n can't be larger than k\n");
        valid = 5;
    }
    return valid;
}

void freeLog( void* data )
{
    free(data);
}

void play1(void* pb, Option* self)
{
    play((Playboard*)pb, 0);
}
void play2(void* pb, Option* self)
{
    play((Playboard*)pb, 1);
}

void spTurnOrder(void* data, Option* self)
{
    Playboard* pb = ((Playboard*)data);
    char* player;
    player = &pb->settings->spTurn;
    *player = (*player == 'O' ? 'X' : 'O');
    *(strchr(self->text, ':')+2) = *player;
    pb->settings->spTurn = *player;
}

void cpuDifficulty(void* data, Option* self)
{
    Playboard* pb = ((Playboard*)data);
    int ai;
    ai = pb->settings->aiDiff;
    if (ai == 1)
    {
        pb->settings->aiDiff = 2;
        strcpy((strchr(self->text, ':')+2),"Hard");
    }
    else if (ai == 2)
    {
        pb->settings->aiDiff = 1;
        strcpy((strchr(self->text, ':')+2),"Easy");
    }
}

void callDraws(void* data, Option* self)
{
    Playboard* pb = ((Playboard*)data);
    int cd;
    cd = pb->settings->callDraws;
    if (cd == 1)
    {
        pb->settings->callDraws = 0;
        strcpy((strchr(self->text, ':')+2),"No ");
    }
    else if (cd == 0)
    {
        pb->settings->callDraws = 1;
        strcpy((strchr(self->text, ':')+2),"Yes");
    }
}

void player1Col(void* data, Option* self)
{
    char s[3];
    int col = colourPicker("Player 1 Colour:");
    ((Playboard*)data)->settings->p1C = col;
    sprintf(s, "%2d", col);
    strncpy((strchr(self->text, '\033')+2),s,2);
}

void player2Col(void* data, Option* self)
{
    char s[3];
    int col = colourPicker("Player 2 Colour:");
    ((Playboard*)data)->settings->p1C = col;
    sprintf(s, "%2d", col);
    strncpy((strchr(self->text, '\033')+2),s,2);
}

void colSelect(void* data, Option* self)
{
    char s[3];
    int col = colourPicker(self->text);
    *(int*)data = col;
    sprintf(s, "%2d", col);
    strncpy((strchr(self->text, '\033')+2),s,2);
}

void charSelect(void* data, Option* self)
{
    int s;
    system("clear");
    printf("\033[1;1H%s", self->text);
    do
    {
        s = getchar();
    } while (s < 32 || s > 254 || s == 127);
    *(char*)data = (char)s;
    *(strchr(self->text, '\033')+5) = s;
}

void setMenu(Menu* menu, void* pb)
{
    Menu* settings = makeMenu();
    Menu* borderChar = makeMenu();
    Menu* borderCol = makeMenu();
    char scr[100];
    Settings* s = ((Playboard*)pb)->settings;
    settings->type = SUBMENU;
    settings->title = "Game Settings";
    addOption(settings, "Player 1 Colour: \033[27mX\033[39m", &player1Col, pb);
    addOption(settings, "Player 2 Colour: \033[27mO\033[39m", &player2Col, pb);
    addSubmenu(settings, "Border Characters", borderChar, pb);
    addSubmenu(settings, "Border Colours", borderCol, pb);
    addOption(settings, "Single Player Turn Order: X", &spTurnOrder, pb);
    addOption(settings, "Computer Difficulty: Hard", &cpuDifficulty, pb);
    addOption(settings, "Call Draws Early: No ", &callDraws, pb);

    borderChar->title = "Border Character Select";
    borderChar->type = SUBMENU;
    sprintf(scr,"Top and Bottom Char: \033[%dm%c\033[39m",s->borderTopC,s->borderTop);
    addOption(borderChar, scr, &charSelect, (void*)&s->borderTop);
    sprintf(scr,"Side Inner Char: \033[%dm%c\033[39m",s->borderSideInC,s->borderSideIn);
    addOption(borderChar, scr, &charSelect, (void*)&s->borderSideIn);
    sprintf(scr,"Side Outer Char: \033[%dm%c\033[39m",s->borderSideOutC,s->borderSideOut);
    addOption(borderChar, scr, &charSelect, (void*)&s->borderSideOut);
    sprintf(scr,"Horizontal Divider Char: \033[%dm%c\033[39m",s->borderHorizontalC,s->borderHorizontal);
    addOption(borderChar, scr, &charSelect, (void*)&s->borderHorizontal);
    sprintf(scr,"Vertical Divider Char: \033[%dm%c\033[39m",s->borderVerticalC,s->borderVertical);
    addOption(borderChar, scr, &charSelect, (void*)&s->borderVertical);
    sprintf(scr,"Corner Inner Char: \033[%dm%c\033[39m",s->borderCornerInC,s->borderCornerIn);
    addOption(borderChar, scr, &charSelect, (void*)&s->borderCornerIn);
    sprintf(scr,"Corner Outer Char: \033[%dm%c\033[39m",s->borderCornerOutC,s->borderCornerOut);
    addOption(borderChar, scr, &charSelect, (void*)&s->borderCornerOut);

    borderCol->title = "Border Colour Select";
    borderCol->type = SUBMENU;
    sprintf(scr,"Top and Bottom Colour: \033[%dm%c\033[39m",s->borderTopC,s->borderTop);
    addOption(borderCol, scr, &colSelect, (void*)&s->borderTopC);
    sprintf(scr,"Side Inner Colour: \033[%dm%c\033[39m",s->borderSideInC,s->borderSideIn);
    addOption(borderCol, scr, &colSelect, (void*)&s->borderSideInC);
    sprintf(scr,"Side Outer Colour: \033[%dm%c\033[39m",s->borderSideOutC,s->borderSideOut);
    addOption(borderCol, scr, &colSelect, (void*)&s->borderSideOutC);
    sprintf(scr,"Horizontal Divider Colour: \033[%dm%c\033[39m",s->borderHorizontalC,s->borderHorizontal);
    addOption(borderCol, scr, &colSelect, (void*)&s->borderHorizontalC);
    sprintf(scr,"Vertical Divider Colour: \033[%dm%c\033[39m",s->borderVerticalC,s->borderVertical);
    addOption(borderCol, scr, &colSelect, (void*)&s->borderVerticalC);
    sprintf(scr,"Corner Inner Colour: \033[%dm%c\033[39m",s->borderCornerInC,s->borderCornerIn);
    addOption(borderCol, scr, &colSelect, (void*)&s->borderCornerInC);
    sprintf(scr,"Corner Outer Colour: \033[%dm%c\033[39m",s->borderCornerOutC,s->borderCornerOut);
    addOption(borderCol, scr, &colSelect, (void*)&s->borderCornerOutC);

    menu->title = TITLE;
    addOption(menu, "1 Player Game", &play1, pb);
    addOption(menu, "2 Player Game", &play2, pb);
    addOption(menu, "View MNK Values", &showSettings, pb);
    #ifdef EDITOR
    addOption(menu, "Edit MNK Values", &editMNK);
    #endif
    addSubmenu(menu, "Change Game Settings", settings, pb);
    addOption(menu, "View Current Logs", &showLogs, pb);
    #ifndef SECRET
    addOption(menu, "Save Logs To File", &saveLogs, pb);
    #endif
}

void editMNK(void* pbIn, Option* self)
{
    Playboard* pb = (Playboard*)pbIn;
    int m, n, k, mMax, nMax;
    static struct termios oldt, newt;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    mMax = (w.ws_col-3)/4;
    nMax = (w.ws_row-1)/2;

    system("clear");

    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag |= (ICANON | ECHO);
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    printf("\033[1;1HPlease enter the width, height and win length of the board\n");
    scanf("%d %d %d", &m, &n, &k);
    if (!validateMNK(m, n, k, mMax, nMax))
    {
        pb->m = m;
        pb->n = n;
        pb->k = k;
    }

    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
    getKey();
    getKey();
}

void showSettings(void* pbIn, Option* self)
{
    Playboard* pb = (Playboard*)pbIn;
    system("clear");
    printf("\033[0;0H Current Settings Are:\n\n M = %d\n\n N = %d\n\n K = %d\n\n",
                                                            pb->m, pb->n, pb->k);
    getKey();
}

void showLogs(void* pbIn, Option* self)
{
    Playboard* pb = (Playboard*)pbIn;
    system("clear");

    if (pb->logs->head != NULL)
    {
        printLogs(pb, stdout);
    }
    getKey();
}

void saveLogs(void* pbIn, Option* self)
{
    Playboard* pb = (Playboard*)pbIn;
    time_t ltime;
    struct tm *tm;
    char fileName[100];
    FILE* out;
    
    ltime = time(NULL);
    tm = localtime(&ltime);
    sprintf(fileName, "MNK_%d-%d-%d_%d-%d_%d-%d.log", pb->m, pb->n, pb->k,
                        tm->tm_hour, tm->tm_min, tm->tm_mday, tm->tm_mon);
    out = fopen(fileName, "w");
    system("clear");
    printLogs(pb, out);
    fclose(out);
    printf("\033[1;1HLogs written to file");
    getKey();
}

void printLogs(Playboard* pb, FILE* file)
{
    ListNode* node = pb->logs->head;
    Log* log;
    int game = 0;
    char player;
    fprintf(file, "SETTINGS:\n\tM: %d\n\tN: %d\n\tK: %d\n\n", pb->m, pb->n, pb->k);
    while (node != NULL)
    {
        log = (Log*)node->data;
        if (log->turn == 1)
        {
            if (game != 0)
            {
                fprintf(file, "\t%c won game %d\n", player, game);
            }
            game++;
            fprintf(file, "GAME %d:\n", game);
        }
        if (log->turn%2) { player = 'X'; }
        else { player = 'O'; }
        fprintf(file, "\tTurn: %d\n\tPlayer: %c\n\tLocation: %d,%d\n\n",
                                    log->turn, player, log->x, log->y);
        node = node->next;
    }
    fprintf(file, "\t%c won game %d\n", player, game);
}
