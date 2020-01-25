#ifndef AI2_H
#define AI2_H 1
int* aiMove(Playboard*, int);
int moveScore(Playboard* pb, int x, int y, char turn);
int moveMax(Playboard* pb, int x, int y, char opTurn);
#endif