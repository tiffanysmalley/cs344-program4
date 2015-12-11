/******************************************************************************
 ** Filename:    keygen.c
 ** Author:      Scott Milton
 ** Date:        12/7/15
 **
 ** Description: This program accepts a whole number from the command line and
 **              outputs that number of random characters that are either 
 **              capital letters or spaces. 
 **
 ** Input:       from stdin: 	type char*
 **             
 ** Output:      to stdout:     type char
 ******************************************************************************/


#include <stdio.h>     // for printf
#include <stdlib.h>    // for exit, rand, srand
#include <time.h>      // for time



/*****************************************************************************
 ** Function:         randInt()
 ** Description:      This function generates and returns a psuedo-random number
 **                   in range from min to max, inclusive.
 ** Parameters:       two int values: min, max
 ** Pre-Conditions:   min and max are integer values
 ** Post-Conditions:  returns the random number
 **
 *****************************************************************************/
int randInt(int min, int max);



int main(int argc, char** argv)
{

    char randLetter;
    int i;
    int length;
    time_t sysClock;

    // i.e. - make sure there are enough args
    if (argc < 2)
    {
        printf("Usage: keygen keyLength\n");
        exit(1);
    }

    // get length of key to generate from arg #1
    sscanf(argv[1], "%d", &length);

    if (length < 1)
    {
        printf("keygen: invalid keyLength\n");
        exit(1);
    }

    // seed the rand() function with system clock
    srand((unsigned) time(&sysClock));

    // loop length number of times
    for (i = 0; i < length; i++)
    {
        // generate a random letter
        randLetter = (char) randInt(64, 90);

        // replace asterisk character with space
        if (randLetter == '@')
        {
           randLetter = ' ';
        }

        printf("%c", randLetter);
    }

    printf("\n");

    return 0;
}



/* uses stdlib.h */
int randInt(int min, int max)
{   // returns a random integer between min and max, inclusive
    // (max - min + 1) is the range
    // + min sets the bottom of the range
    return rand() % (max - min + 1) + min;
}
