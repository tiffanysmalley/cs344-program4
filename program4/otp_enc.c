/******************************************************************************
 ** Filename:    otp_enc.c
 ** Author:      Scott Milton
 ** Date:        12/7/15
 **
 ** Description: This program 
 **
 ** Input:       from stdin:    type char*
 **             
 ** Output:      to stdout:     type char
 ******************************************************************************/

#include <fcntl.h>     // for open
#include <stdio.h>     // for printf
#include <stdlib.h>    // for exit


#define BUFFER_SIZE    128000


int main(int argc, char** argv)
{

    char buffer1[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    int fd;
    int i;
    int keyLength;
    int plaintextLength;
    int port;

    // make sure there are enough args
    if (argc < 4)
    {
        printf("Usage: otp_enc plaintext key port\n");
        exit(1);
    }

    // validate port number
    sscanf(argv[4], "%d", &port);
    if (port < 0 || port > 65535)
    {
        printf("otp_enc: invalid port\n");
        exit(2);
    }

    // open plaintext for reading
    fd = open(argv[1], O_RDONLY);

    // if unable to read file, display error message
    if (fd < 0)
    {
        printf("Error: cannot open plaintext file %s\n", argv[1]);
        exit(1);
    }

    // read contents of plaintext & keep track of # of bytes read
    plaintextLength = read(fd, buffer1, BUFFER_SIZE);

    // validate contents of plaintext
    for (i = 0; i < plaintextLength - 1; i++)
    {
        if ((int) buffer1[i] > 90 || ((int) buffer1[i] < 65 && (int) buffer1[i] != 32))
        {
            printf("otp_enc error: plaintext contains bad characters\n");
        }
    }

    // close plaintext
    close(fd);

    // open key for reading
    fd = open(argv[2], O_RDONLY);

    // if unable to read file, display error message
    if (fd < 0)
    {
        printf("Error: cannot open key file %s\n", argv[2]);
        exit(1);
    }

    // read contents of key & keep track of # of bytes read
    keyLength = read(fd, buffer2, BUFFER_SIZE);

    // validate contents of key
    for (i = 0; i < keyLength - 1; i++)
    {
        if ((int) buffer2[i] > 90 || ((int) buffer2[i] < 65 && (int) buffer2[i] != 32))
        {
            printf("otp_enc error: key contains bad characters\n");
        }
    }

    // close key
    close(fd);

    // compare length of plaintext to that of key
    if (keyLength < plaintextLength)
    {
        printf("Error: key '%s' is too short", argv[2]);
    }

    // connect to otp_enc_d

    // make sure not otp_dec_d though

    // send plaintext to otp_enc_d

    // receive ciphertext from otp_enc_d

    // output ciphertext to stdout

    // add newline to ciphertext
//    printf("\n");

    return 0;
}
