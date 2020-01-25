/*****************************************************************************
* File:      GamePlay.h                                                      *
* Unit:      COMP1000 Unix and C Programming                                 *
* Purpose:   Draw pretty pictures                                            *
* Reference: None.                                                           *
* Comments:  This comment block is the maximum width of 78 characters.       *
* Requires:  Nothing. List dependencies here                                 *
* Created:   20/09/2019 * Last Modified: 20/09/2019                          *
*****************************************************************************/
#ifndef GAMEPLAY_H
#define GAMEPLAY_H 1
#include "LinkedList.h"
enum Colour {BLACK = 30, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, LIGHTGRAY,
                DARKGREY = 90, LIGHTRED, LIGHTGREEN, LIGHTYELLOW, LIGHTBLUE,
                LIGHTMAGENTA, LIGHTCYAN, WHITE, DEFAULT = 39};
static const enum Colour Colour_map[] = { BLACK, RED, GREEN, YELLOW, BLUE,
                MAGENTA, CYAN, LIGHTGRAY, DARKGREY, LIGHTRED, LIGHTGREEN,
                LIGHTYELLOW, LIGHTBLUE, LIGHTMAGENTA, LIGHTCYAN, WHITE, DEFAULT };

typedef struct Settings {
    char borderTop, borderSideIn, borderSideOut, borderHorizontal,
        borderVertical, borderCornerIn, borderCornerOut;
    int p1C, p2C, borderTopC, borderSideInC, borderSideOutC, borderHorizontalC,
        borderVerticalC, borderCornerInC, borderCornerOutC;
    char spTurn;
    int aiDiff, callDraws;
} Settings;

typedef struct Playboard {
    char** board;
    int turn, m, n, k, twoP;
    LinkedList* logs;
    Settings* settings;
} Playboard;

void play( Playboard* pb, int );
char winCheck( Playboard* pb );
void setBox( Playboard* pb, int x, int y, char c );
char getBox( Playboard* pb, int x, int y );
int realX( int x );
int realY( int y );
Playboard* newPlayboard( int m, int n, int k );
void drawPlayboard( Playboard* pb );
void resetBoard(Playboard*);
int aiTTTTurn(Playboard*, int);
char* colName(enum Colour col);
#endif
