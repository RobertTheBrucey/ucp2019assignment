/*****************************************************************************
* File:      FileInput.c                                                     *
* Reference: None.                                                           *
* Unit:      COMP1000 Unix and C Programming                                 *
* Purpose:   Read in a file and populate a linked list                       *
* Comments:  Valid command must be exactly a command name (case insensitive) *
* Requires:  LinkedList.h, stdio.h, stdlib.h, string.h, ctype.h              *
* Created:   20/09/2019 * Last Modified: 20/09/2019                          *
*****************************************************************************/
#include "FileInput.h"
#include "GamePlay.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define VALID_LEN 6
#define COMMAND_LEN 51

/* Forward Declarations */

/* Array of valid commands to check against */
const char* const validCommands[VALID_LEN] = {"m", "n", "k"};

void readSettings( char* fileName, Playboard** pbp )
{
    FILE *in;
    int m = 0, n = 0, k = 0, valid = 0;/* 0 means valid */
    char line[COMMAND_LEN];
    /*Playboard *pb;*/

    if ( fileName == NULL )
        fprintf( stderr, "File name not specified.\n" );
    else
    {
        in = fopen(fileName, "r");
        if ( in == NULL )
            fprintf( stderr, "Problem opening file\n" );
        else if ( ferror(in) )
            perror( "File read error\n" );
        else
        {
            while ( fgets( line, COMMAND_LEN, in ) != NULL )
            {
                if (line[2] == ' ') {line[0] = ' ';}
                switch(tolower(line[0]))
                {
                    case 'm':
                        if (m != 0)
                            valid = 1;
                        else
                            sscanf( line, "%*c=%d", &m);
                        if (m < 2)
                            valid = 2;
                        break;
                    case 'n':
                        if (n != 0)
                            valid = 1;
                        else
                            sscanf( line, "%*c=%d", &n);
                        if (n < 2)
                            valid = 2;
                        break;
                    case 'k':
                        if (k != 0)
                            valid = 1;
                        else
                            sscanf( line, "%*c=%d", &k);
                        if (k < 2)
                            valid = 2;
                        break;
                    case ' ':
                        valid = 4;
                        break;
                    default:
                        if (m > 2 && n > 2 && k > 2)
                            valid = 3;
                        else
                            valid = 4;
                        break;
                }
            }
            fclose(in);
            if ( m < 2 || n < 2 || k < 2 )
                valid = 5;
            switch (valid)
            {
                case 0:
                    printf( "Settings file valid and loaded\n" );
                    *pbp = newPlayboard( m, n, k );
                    break;
                case 1:
                    printf( "Duplicate setting present in file\n" );
                    break;
                case 2:
                    printf( "Invalid value for value in file\n" );
                    break;
                case 3:
                    printf( "Invalid line in file, continuing\n" );
                    valid = 0;
                    break;
                case 4:
                    printf( "Invalid line in file, not recoverable\n" );
                    break;
                case 5:
                    printf( "Setting missing from file\n" );
                    break;
            }
        }
    }
}

