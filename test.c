#include <stdio.h>
#include <stdlib.h>
void alB(char***);

int main ()
{
    unsigned int ii;
    /*char** arrPtr = NULL;*/
    for (ii = 32; ii < 255; ii++)
    {
        printf("%d: %c\n", ii, ii);
	putc(ii,stdout);
    }
    /*alB(&arrPtr);*/

    /*arrPtr[0][0] = 'a';
    printf("%c\n", arrPtr[0][0]);*/

    return 0;
}

void alB(char*** arrPtrIn)
{
    char** arrPtr;
    *arrPtrIn = (char**)malloc(sizeof(char*));
    arrPtr = *arrPtrIn;
    arrPtr[0] = (char*)malloc(sizeof(char));
}

