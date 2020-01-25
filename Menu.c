/*
 * File: menu.c
 ***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Menu.h"
#include "LinkedList.h"
#include "GamePlay.h"

Menu* makeMenu()
{
    Menu *menu = (Menu*)malloc(sizeof(Menu));
    menu->type = MENU;
    menu->options = createLinkedList();
    menu->optionCount = 0;
    menu->selection = 0;
    menu->title = NULL;
    menu->spacing = 1;
    menu->loop = 1;
    return menu;
}

void addOption(Menu* menu, char* text, void func(void*, Option*), void* data)
{
    Option *option = (Option*)malloc(sizeof(Option));

    option->type = OPTION;
    option->text = mystrdup(text);
    option->func.func = func;
    option->data = data;

    insertLast(menu->options, (void*)option);
    menu->optionCount++;
}

void addSubmenu(Menu* menu, char* text, Menu* submenu, void* data)
{
    Option *option = (Option*)malloc(sizeof(Option));

    option->type = SUBMENU;
    option->text = mystrdup(text);
    option->func.Menu = submenu;
    option->data = data;

    insertLast(menu->options, (void*)option);
    menu->optionCount++;
}

void freeOption(void* data)
{
    Option* option = (Option*)data;
    free(option->text);
    free(option);
}

void freeMenu(Menu* menu)
{
    ListNode* node = menu->options->head;
    while (node != NULL)
    {
        if (((Option*)(node->data))->type == SUBMENU)
        {
            freeMenu(((Option*)(node->data))->func.Menu);
        }
        node = node->next;
    }
    freeLinkedList(menu->options, &freeOption);
    free(menu);
}

void printMenu(Menu* menu)
{
    ListNode* node = menu->options->head;
    int ii;
    Option* option;
    system("clear");
    printf("\033[27m\033[0;0H  %s\n\n",menu->title);
    while (node != NULL)
    {
        option = (Option*)node->data;
        printf(" [ ] %s\n", option->text);
        for (ii = 0; ii < menu->spacing; ii++) printf("\n");
        node = node->next;
    }
    if (menu->type == MENU)
    {
        printf(" [ ] Exit Application\n");
    }
    else if (menu->type == SUBMENU)
    {
        printf(" [ ] Exit Menu\n");
    }
    printf("\033[%d;3H#\b", menu->selection*2+3);
}

void showMenu(Menu* menu)
{
    int key;
    Option* option;
    printMenu(menu);
    do
    {
        printf("\033[%d;3H#\b", menu->selection*(menu->spacing+1)+3);
        key = getKey();
        printf("\033[%d;3H \b", menu->selection*(menu->spacing+1)+3);
        switch (key)
        {
            case 1: case 2: /*Left or Right*/
                break;
            case 3: /* Down */
                if (menu->selection > 0) { menu->selection--; }
                break;
            case 4: /* Up */
                if (menu->selection < menu->optionCount) { menu->selection++; }
                break;
            case 5: /* Enter or Space */
                if (menu->selection < menu->optionCount)
                {
                    option = (Option*)getNode(menu->options, menu->selection);
                    if (option->type == OPTION)
                    {
                        (option->func.func)(option->data, option);
                    }
                    else if (option->type == SUBMENU)
                    {
                        showMenu(option->func.Menu);
                    }
                    if (menu->loop == 0) {key = 6;}
                    printMenu(menu);
                }
                else
                {
                    key = 6;
                }
                break;
            case 6:/* Escape key */
                break;
        }
    } while (key != 6);
}

int getKey()
{
    int key = 0;
    switch (getchar())
    {
        case 'w':
            key = 3;
            break;
        case 's':
            key = 4;
            break;
        case 'd':
            key = 2;
            break;
        case 'a':
            key = 1;
            break;
        case ' ': case '\n':
            key = 5;
            break;
        case '\033':
            switch(getchar())
            {
                case '[':
                switch(getchar())
                {
                    case 'A':
                    key = 3;
                    break;
                    case 'B':
                    key = 4;
                    break;
                    case 'C':
                    key = 2;
                    break;
                    case 'D':
                    key = 1;
                    break;
                }
                break;
                case '\033':
                key = 6;
                break;
            }
    }
    return key;
}

char* mystrdup(char* in)
{
    char* out = (char*)malloc(sizeof(char)*(1+strlen(in)));
    strcpy(out,in);
    return out;
}

void setCol(void* data, Option* self)
{
    int* col = (int*)data;
    sscanf(self->text, "\033[%dm", col);
}

int colourPicker(char* prompt)
{
    Menu* menu = makeMenu();
    int ii, colout;
    int* col = (int*)malloc(sizeof(int));
    char colString[100];
    *col = 39;
    menu->title = prompt;
    menu->spacing = 0;
    menu->loop = 0;
    menu->type = SUBMENU;
    for (ii = 0; ii < 17; ii++)
    {
        sprintf(colString, "\033[%dm%s\033[39m", Colour_map[ii], colName(Colour_map[ii]));
        addOption(menu, colString, &setCol, (void*)col);
    }
    showMenu(menu);

    freeMenu(menu);
    colout = *col;
    free(col);
    printf("colout=  %d  \n\n",colout);
    return colout;
}