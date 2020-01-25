#include "GamePlay.h"
#include "AI2.h"
#include <stdlib.h>
#include <stdio.h>

void aiMove2(Playboard* pb, int* move);

int* aiMove(Playboard* pb, int diff)
{
    int x=0,y=0;
    char temp = ' ';
    int* move = (int*)malloc(2*sizeof(int));
    if (diff == 1)
    {
        do
        {
            x = rand() % pb->m;
            y = rand() % pb->n;
            temp = getBox(pb, x, y);
        } while (temp != ' ');
        move[0] = x;
        move[1] = y;
    }
    else if (diff == 2)
    {
        aiMove2(pb, move);
    }
    return move;
}

void aiMove2(Playboard* pb, int* move)
{
    int ii, jj, max = 0, msO, msX;
    /*Search for winning move*/
    ii = jj = 0;
    while ((ii < pb->m) && (jj < pb->n) && !max)
    {
        if (getBox(pb,ii,jj) == ' ')
        {
            if (moveScore(pb,ii,jj,'O') >= pb->k)
            {
                move[0] = ii;
                move[1] = jj;
                max++;
            }
        }
        ii++;
        if (ii == pb->m)
        {
            ii = 0;
            jj++;
        }
    }
    /*Search for enemy winning move*/
    if (!max)
    {
        ii = jj = 0;
        while ((ii < pb->m) && (jj < pb->n) && !max)
        {
            if (getBox(pb,ii,jj) == ' ')
            {
                if (moveScore(pb,ii,jj,'X') >= pb->k)
                {
                    move[0] = ii;
                    move[1] = jj;
                    max++;
                }
            }
            ii++;
            if (ii == pb->m)
            {
                ii = 0;
                jj++;
            }
        }
    }
    /*Find best move*/
    if (!max)
    {
        ii = jj = 0;
        while ((ii < pb->m) && (jj < pb->n))
        {
            if (getBox(pb,ii,jj) == ' ')
            {
                msO = moveScore(pb,ii,jj,'O');
                msX = moveScore(pb,ii,jj,'X');
                if (msO >= msX)
                {
                    pb->board[ii][jj] = msO;
                }
                else
                {
                    pb->board[ii][jj] = msX;
                }
                max = (pb->board[ii][jj] > max ? pb->board[ii][jj] : max);
            }
            ii++;
            if (ii == pb->m)
            {
                ii = 0;
                jj++;
            }
        }
        /* Nodes and max have to be set */
        do
        {
            ii = rand() % pb->m;
            jj = rand() % pb->n;
        } while (getBox(pb,ii,jj) != (char)max);
        move[0] = ii;
        move[1] = jj;
        for (ii = 0; ii < pb->m; ii++)
        {
            for (jj = 0; jj < pb->n; jj++)
            {
                if (pb->board[ii][jj] < 'O')
                {
                    pb->board[ii][jj] = ' ';
                }
            }
        }
    }
}

int moveScore(Playboard* pb, int x, int y, char turn)
{
    int maxCurrent[4] = {1,1,1,1}, maxTotal = 0, ii;
    /* Check if part of winning line */
    if (moveMax(pb,x,y,turn) >= pb->k)
    {
        /*Check horizontal*/
        ii = x+1;
        while ((ii < pb->m) && (getBox(pb, ii, y) == turn))
        {
            maxCurrent[0]++;
            ii++;
        }
        ii = x-1;
        while ((ii >= 0) && (getBox(pb, ii, y) == turn))
        {
            maxCurrent[0]++;
            ii--;
        }
        /*Check Vertical*/
        ii = y+1;
        while ((ii < pb->n) && (getBox(pb, x, ii) == turn))
        {
            maxCurrent[1]++;
            ii++;
        }
        ii = y-1;
        while ((ii >= 0) && (getBox(pb, x, ii) == turn))
        {
            maxCurrent[1]++;
            ii--;
        }
        /*Check Diagonal 1*/
        ii = 1;
        while (((ii+y) < pb->n) && ((ii+x) < pb->m) && (getBox(pb, x+ii, y+ii) == turn))
        {
            maxCurrent[2]++;
            ii++;
        }
        ii = -1;
        while (((ii+y) >= 0) && ((ii+x) >= 0) && (getBox(pb, x+ii, y+ii) == turn))
        {
            maxCurrent[2]++;
            ii--;
        }
        /*Check Diagonal 2*/
        ii = 1;
        while (((y-ii) >= 0) && ((ii+x) < pb->m) && (getBox(pb, x+ii, y-ii) == turn))
        {
            maxCurrent[3]++;
            ii++;
        }
        ii = -1;
        while (((y-ii) < pb->n) && ((ii+x) >= 0) && (getBox(pb, x+ii, y-ii) == turn))
        {
            maxCurrent[3]++;
            ii--;
        }
        /* Find Best */
        for (ii = 0; ii < 4; ii++)
        {
            if (maxCurrent[ii] > maxTotal)
            {
                maxTotal = maxCurrent[ii];
            }
        }
    }

    return maxTotal;
}

int moveMax(Playboard* pb, int x, int y, char opTurn)
{
    int maxCurrent[4] = {1,1,1,1}, maxTotal = 1, ii;
    char turn = (opTurn == 'X' ? 'O' : 'X');
    /*Check horizontal*/
    ii = x+1;
    while ((ii < pb->m) && (getBox(pb, ii, y) != turn))
    {
        maxCurrent[0]++;
        ii++;
    }
    ii = x-1;
    while ((ii >= 0) && (getBox(pb, ii, y) != turn))
    {
        maxCurrent[0]++;
        ii--;
    }
    /*Check Vertical*/
    ii = y+1;
    while ((ii < pb->n) && (getBox(pb, x, ii) != turn))
    {
        maxCurrent[1]++;
        ii++;
    }
    ii = y-1;
    while ((ii >= 0) && (getBox(pb, x, ii) != turn))
    {
        maxCurrent[1]++;
        ii--;
    }
    /*Check Diagonal 1*/
    ii = 1;
    while (((ii+y) < pb->n) && ((ii+x) < pb->m) && (getBox(pb, x+ii, y+ii) != turn))
    {
        maxCurrent[2]++;
        ii++;
    }
    ii = -1;
    while (((ii+y) >= 0) && ((ii+x) >= 0) && (getBox(pb, x+ii, y+ii) != turn))
    {
        maxCurrent[2]++;
        ii--;
    }
    /*Check Diagonal 2*/
    ii = 1;
    while (((y-ii) >= 0) && ((ii+x) < pb->m) && (getBox(pb, x+ii, y-ii) != turn))
    {
        maxCurrent[3]++;
        ii++;
    }
    ii = -1;
    while (((y-ii) < pb->n) && ((ii+x) >= 0) && (getBox(pb, x+ii, y-ii) != turn))
    {
        maxCurrent[3]++;
        ii--;
    }
    /* Find Best */
    for (ii = 0; ii < 4; ii++)
    {
        if (maxCurrent[ii] > maxTotal)
        {
            maxTotal = maxCurrent[ii];
        }
    }

    return maxTotal;
}