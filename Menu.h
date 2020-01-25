/*
 * File: Menu.h
 **/
#ifndef MENU_H
#define MENU_H 1
#define TEXTLEN 100
#include "LinkedList.h"

enum OptType {OPTION, SUBMENU, MENU};

typedef struct Menu {
    enum OptType type;
    char *title;
    LinkedList *options;
    int optionCount, selection, spacing, loop;
} Menu;

typedef struct Option {
    enum OptType type;
    char *text;
    union func {
        void (*func)(void*, struct Option*);
        Menu* Menu;
    } func;
    void* data;
} Option;

Menu* makeMenu();
void addOption(Menu* menu, char* text, void func(void*, Option*), void*);
void addSubmenu(Menu* menu, char* text, Menu* submenu, void* data);
void freeMenu(Menu* menu);
void showMenu(Menu* menu);
void freeOption(void* option);
int getKey();
int colourPicker(char* prompt);
char* mystrdup(char* in);
#endif