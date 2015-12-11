/******************************************************************************
 ** Filename:    keygen.c
 ** Author:      Scott Milton
 ** Date:        12/7/15
 **
 ** Description: This program accepts a whole number
 **
 ** Input:       from the keyboard: type char[]
 **              from files:        type char[]
 **
 ** Output:      to the console and files : type char[], char*, const char*, int
 ******************************************************************************/


#include <stdio.h>     // for printf
#include <stdlib.h>    // for exit, rand, srand
#include <time.h>      // for time

// #include <errno.h>     // for errno
// #include <fcntl.h>     // for open
// #include <limits.h>    // for INT_MAX
// #include <signal.h>    // for sigset_t
// #include <string.h>    // for strcpy, strcat
// #include <sys/stat.h>  // for stat 
// #include <sys/types.h> // for pid_t
// #include <sys/wait.h>  // for waitpid
// #include <unistd.h>    // for exec

// #define DEBUG             0 // change to 1 for debugging print statements 
// #define MAX_ARGS        512 // maximum arguments accepted on command line
// #define MAX_LENGTH     2048 // maximum length for a command line
// #define MAX_PIDS       1000 // maximum PIDs to track

// define bool as type
//typedef enum { false, true } bool;



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

    // validate format
//    printf("Usage: keygen keyLength\n");

    // i.e. - make sure there are enough args
    if (argc < 1)
    {
        printf("Usage: keygen keyLength\n");
        exit(1);
    }

    // i.e. - make sure arg1 is digits

    // get length of key to generate from arg #1
    length = argv[1];

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
