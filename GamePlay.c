/*****************************************************************************
* File:      GamePlay.c                                                      *
* Unit:      COMP1000 Unix and C Programming                                 *
* Reference: None.                                                           *
* Comments:  Continue on a many errors as possible.                          *
* Requires:  LinkedList.h, effect.h, FileInput.h, stdio.h, stdlib.h, math.h  *
* Created:   20/09/2019 * Last Modified: 20/09/2019                          *
*****************************************************************************/
#include "GamePlay.h"
#include "AI2.h"
#include "TicTacToe.h"
#include "Menu.h"
#include <stdlib.h>
#include <stdio.h>

void play( Playboard* pb, int twoPlayer )
{
    int oldx, x, oldy, y, live = 1, col;
    char turn = 'X', temp;
    oldx = x = oldy = y = 0;
    drawPlayboard(pb);
    resetBoard(pb);
    printf("\033[%d;%dH\033[27m\033[%dmX\b",realY(oldy),realX(oldx),
        pb->settings->p1C);
    while (live)
    {/*1 - Up, 2 - Down, 3 - Left, 4 - Right, 5 - Enter, 6 - Exit*/
        switch (getKey())
        {
            case 6:
                live = 0;
                break;
            case 1:
                if (x > 0) { x--; }
                break;
            case 2:
                if (x < pb->m-1) { x++; }
                break;
            case 3:/*Up*/
                if (y > 0) { y--; }
                break;
            case 4:
                if (y < pb->n-1) { y++; }
                break;
            case 5:
                /*Do turn*/
                if (getBox(pb,x,y) == ' ')
                {
                    setBox(pb,x,y,turn);
                    /*printf("%d",moveScore(pb,x,y,turn));
                    getKey();*/
                    if (moveScore(pb, x, y, turn) >= pb->k)
                    {temp = turn;} else {temp = ' ';}
                    if (turn == 'X') { turn = 'O'; }
                    else if (turn == 'O') { turn = 'X'; }
                    if ((pb->turn > (pb->m * pb->n)) && (temp == ' ')) {temp = 'D';}
                    if (temp != ' ')
                    {
                        y = (int)(pb->m*4+3-14)/2+1;
                        if (y < 0) { y = 0; }
                        if ( temp != 'D' )
                        {
                            printf("\033[%d;%dH\033[39m%c IS THE WINNER\n",
                                    realY(pb->n)+1, y, temp);/*15*/
                        }
                        else
                        {
                            printf("\033[%d;%dH\033[39m  IT'S A DRAW  ",
                                                realY(pb->n)+1, y);
                        }
                        printf("\033[27m\033[39m\n\n");
                        getKey();
                        live = 0;
                    }
                    else
                    {
                        if (twoPlayer == 0)
                        {
                            if(aiTTTTurn(pb, pb->settings->aiDiff) >= pb->k)
                            {temp = 'O';} else {temp = ' ';}
                            if ((pb->turn > (pb->m * pb->n)) && (temp == ' ')) {temp = 'D';}
                            if (temp != ' ')
                            {
                                y = (int)(pb->m*4+3-14)/2+1;
                                if (y < 0) { y = 0; }
                                if ( temp != 'D' )
                                {
                                    printf("\033[%d;%dH\033[39m%c IS THE WINNER\n",
                                            realY(pb->n)+1, y, temp);/*15*/
                                }
                                else
                                {
                                    printf("\033[%d;%dH\033[39m  IT'S A DRAW  ",
                                                        realY(pb->n)+1, y);
                                }
                                printf("\033[27m\033[39m\n\n");
                                getKey();
                                live = 0;
                            }
                            turn = 'X';
                        }
                    }
                }
                break;
        }
        if (live)
        {
            temp = getBox(pb,oldx,oldy);
            col = (temp=='X' ? pb->settings->p1C : pb->settings->p2C);
            printf("\033[%d;%dH\033[27m\033[%dm%c",realY(oldy),realX(oldx),
                                                                col, temp);
            oldx = x;
            oldy = y;
            if (getBox(pb,x,y) == ' ')
            {
                temp = turn;
            }
            else
            {
                temp = getBox(pb,x,y);
            }
            col = (temp=='X' ? pb->settings->p1C : pb->settings->p2C);
            printf("\033[%d;%dH\033[7m\033[%dm%c\b",realY(oldy),realX(oldx), col, temp);
        }
    }
    printf("\033[0m");
}

int aiTTTTurn(Playboard* pb, int diff)
{
    int x, y;
    int* move;
    move = aiMove(pb, diff);
    x = move[0];
    y = move[1];
    free(move);
    setBox(pb,x,y,'O');
    printf("\033[%d;%dH\033[27m\033[%dm%c", realY(y), realX(x), pb->settings->p2C,
            getBox(pb,x,y));
    return moveScore(pb, x, y, 'O');
}

void setBox( Playboard* pb, int x, int y, char c )
{
    Log* log;
    pb->board[x][y] = c;
    log = (Log*)malloc(sizeof(Log));
    log->turn = pb->turn;
    log->x = x;
    log->y = y;

    insertLast(pb->logs, (void*)log);

    pb->turn++;
}

char getBox( Playboard* pb, int x, int y )
{
    return pb->board[x][y];
}

int realX( int x )
{
    return (4*x + 4);
}

int realY( int y )
{
    return (2*y + 2);
}

void setSettings(Settings* s)
{
    s->borderTop = L'=';
    s->borderSideIn = L':';
    s->borderSideOut = L'|';
    s->borderHorizontal = L'-';
    s->borderVertical = L'|';
    s->borderCornerIn = L'+';
    s->borderCornerOut = L'#';
    /*s->borderTop = L'═';
    s->borderSideIn = L':';
    s->borderSideOut = L'║';
    s->borderHorizontal = L'═';
    s->borderVertical = L'║';
    s->borderCornerIn = "╬";
    s->borderCornerOut = L'#';*/

    s->p1C = LIGHTMAGENTA;
    s->p2C = LIGHTRED;
    s->borderTopC = GREEN;
    s->borderSideInC = CYAN;
    s->borderSideOutC = GREEN;
    s->borderHorizontalC = CYAN;
    s->borderVerticalC = CYAN;
    s->borderCornerInC = YELLOW;
    s->borderCornerOutC = YELLOW;

    s->spTurn = 'X';
    s->aiDiff = 2;
    s->callDraws = 0;
}

Playboard* newPlayboard( int m, int n, int k )
{
    int ii, jj;
    Playboard* pb = (Playboard*)malloc(sizeof(Playboard));

    pb->board = (char**)malloc(sizeof(char*)*m);
    for (ii = 0; ii < m; ii++)
    {
        pb->board[ii] = (char*)malloc(sizeof(char*)*n);
    }

    for (ii = 0; ii < m; ii++)
    {
        for (jj = 0; jj < n; jj++)
        {
            pb->board[ii][jj] = ' ';
        }
    }

    pb->turn = 1;

    pb->m = m;
    pb->n = n;
    pb->k = k;

    pb->logs = (LinkedList*)malloc(sizeof(LinkedList));
    pb->logs->head = NULL;

    pb->logs->numNodes = 0;

    pb->settings = (Settings*)malloc(sizeof(Settings));
    setSettings(pb->settings);

    return pb;
}

void drawPlayboard( Playboard* pb )
{
    Settings* s = pb->settings;
    int ii, jj;
    system("clear");
    printf("\033[0;0H");
    printf("\033[%dm%c\033[%dm%c",s->borderCornerOutC, s->borderCornerOut,
        s->borderTopC, s->borderTop);
    for (ii = 0; ii < pb->m; ii++)
        { printf("\033[%dm%c%c%c%c",s->borderTopC,s->borderTop,s->borderTop,
            s->borderTop,s->borderTop); }
    printf("\b\033[%dm%c\033[%dm%c\n",s->borderTopC, s->borderTop,
        s->borderCornerOutC, s->borderCornerOut);
    for (ii = 0; ii < pb->n; ii++)
    {
        printf("\033[%dm%c\033[%dm%c\033[%dm",s->borderSideOutC,s->borderSideOut,
            s->borderSideInC,s->borderSideIn,s->borderVerticalC);
        for (jj = 0; jj < pb->m; jj++)
        {
            printf("   %c",s->borderVertical);
        }
        printf("\b\033[%dm%c\033[%dm%c\n",s->borderSideInC,s->borderSideIn,
            s->borderSideOutC,s->borderSideOut);
        if (ii == pb->n-1) { break; }
        printf("%c\033[%dm%c\033[%dm",s->borderSideOut,
            s->borderSideInC,s->borderSideIn,s->borderHorizontalC);
        for (jj = 0; jj < pb->m; jj++)
        {
            printf("%c%c%c\033[%dm%s\033[%dm",s->borderHorizontal,s->borderHorizontal,
                s->borderHorizontal,s->borderCornerInC,s->borderCornerIn,
                    s->borderHorizontalC);
        }
        printf("\b\033[%dm%c\033[%dm%c\n",s->borderSideInC,s->borderSideIn,
            s->borderSideOutC,s->borderSideOut);
    }
    printf("\033[%dm%c\033[%dm%c",s->borderCornerOutC, s->borderCornerOut,
        s->borderTopC, s->borderTop);
    for (ii = 0; ii < pb->m; ii++)
        { printf("\033[%dm%c%c%c%c",s->borderTopC,s->borderTop,s->borderTop,
            s->borderTop,s->borderTop); }
    printf("\b\033[%dm%c\033[%dm%c\n",s->borderTopC, s->borderTop,
        s->borderCornerOutC, s->borderCornerOut);
    printf("\033[39m");
}

void resetBoard( Playboard* pb )
{
    int ii, jj;
    for (ii = 0; ii < pb->m; ii++)
    {
        for (jj = 0; jj < pb->n; jj++)
        {
            pb->board[ii][jj] = ' ';
        }
    }
    pb->turn = 1;
}

char* colName(enum Colour col)
{
    int coli = (int)col;
    static char *strings[] = {"Black", "Red", "Green", "Yellow", "Blue",
        "Magenta", "Cyan", "Light Grey", "Dark Grey", "Light Red", "Light Green",
        "Light Yellow", "Light Blue", "Light Magenta", "Light Cyan", "White", "Default"};
    if (coli >= 30 && coli < 38) { coli = coli - 30; }
    else if (coli >= 90 && coli < 98) { coli = coli - 90 + 8; }
    else { coli = 16; }
    return strings[coli];
}