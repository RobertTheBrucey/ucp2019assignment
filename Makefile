CC = gcc
CFLAGS = -Wall -Werror -ansi -pedantic
OBJ = LinkedList.o TicTacToe.o GamePlay.o FileInput.o AI2.o Menu.o
LIB = -lm
EXEC = TicTacToe

$(EXEC) : rmTicTacToe $(OBJ)
	$(CC) $(OBJ) $(LIB) -o $(EXEC)

All : $(EXEC) Secret Editor

movcc : CC = movcc
movcc : CFLAGS = -DMOVCC=1
movcc : $(EXEC)

Secret : $(OBJ) rmTicTacToe rmSecret
	$(CC) -c TicTacToe.c $(CFLAGS) -DSECRET=1
	$(CC) $(OBJ) $(LIB) -o Secret

Editor : $(OBJ) rmTicTacToe rmEditor
	$(CC) -c TicTacToe.c $(CFLAGS) -DEDITOR=1
	$(CC) $(OBJ) $(LIB) -o Editor

debug : Debug

Debug : CFLAGS +=  -g
Debug : clean $(OBJ)
	$(CC) -c TicTacToe.c $(CFLAGS)
	$(CC) $(OBJ) $(LIB) -o TicTacToeDebug

Menu.o : Menu.c Menu.c GamePlay.h
	$(CC) -c Menu.c $(CFLAGS)

LinkedList.o : LinkedList.c LinkedList.h
	$(CC) -c LinkedList.c $(CFLAGS)

FileInput.o : FileInput.c FileInput.h LinkedList.h
	$(CC) -c FileInput.c $(CFLAGS)

AI2.o : AI2.h GamePlay.h
	$(CC) -c AI2.c $(CFLAGS)

GamePlay.o : GamePlay.c GamePlay.h AI2.h
	$(CC) -c GamePlay.c $(CFLAGS)

TicTacToe.o : TicTacToe.c TicTacToe.h LinkedList.h FileInput.h GamePlay.h Menu.h
	$(CC) -c TicTacToe.c $(CFLAGS)

clean :
	rm -f $(OBJ) $(EXEC) Secret Editor TicTacToeDebug

rmTicTacToe :
	rm -f TicTacToe.o

rmSecret :
	rm -f Secret

rmEditor :
	rm -f Editor
