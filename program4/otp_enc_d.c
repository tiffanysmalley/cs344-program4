/******************************************************************************
 ** Filename:    otp_enc_d.c
 ** Author:      Scott Milton
 ** Date:        12/7/15
 **
 ** Description: This program 
 **
 ** Input:       from stdin     type char*
 **              from socket    type char
 **
 ** Output:      to socket      type char
 ******************************************************************************/

#include <fcntl.h>     // for open
#include <sys/socket.h>
#include <stdio.h>     // for printf
#include <stdlib.h>    // for exit
#include <sys/stat.h> 
#include <unistd.h>

#define BUFFER_SIZE    128000


int main(int argc, char** argv)
{

    char buffer1[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    char buffer3[BUFFER_SIZE];
//    int fd;
    int i;
    int keyLength;
    int plaintextLength;
    int port;

    // make sure there are enough args
    if (argc < 2)
    {
        printf("Usage: otp_enc_d port\n");
        exit(1);
    }

    // validate port number
    sscanf(argv[1], "%d", &port);
    if (port < 0 || port > 65535)
    {
        printf("otp_enc_d: invalid port\n");
        exit(2);
    }

    // // create socket 
    // int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // if (sockfd == -1)
    // {
    //     printf("Error: could not contact otp_enc_d on port %d\n", port);
    //     exit(2);
    // }

    // // connect to otp_enc
    // if (connect(sockfd, (stuct sockaddr *) &server, sizeof(server)) == -1)
    // {
    //     printf("Error: could not connect to otp_enc_d on port %d\n", port);
    //     exit(2);
    // }

    // struct sockaddr_in server;

    // server.sin_family = AF_INET;
    // server.sin_port = htons(port);
    // server.sin_addr.s_addr = inet_addr("192.168.1.1");


    // make sure not otp_dec_d though

    // receive plaintext from otp_enc
    // ssize_t numSent = send(sockfd, buffer1, plaintextLength - 1, 0);
    // if (numSent < plaintextLength)
    // {
    //     printf("Error: could not send plaintext to otp_enc_d on port %d\n", port);
    //     exit(2);
    // }

    // receive key from otp_enc
    // numSent = send(sockfd, buffer2, keyLength - 1, 0);
    // if (numSent < keyLength)
    // {
    //     printf("Error: could not send key to otp_enc_d on port %d\n", port);
    //     exit(2);
    // }

    // ssize_t numReceived;



    // validate contents of plaintext
    // for (i = 0; i < plaintextLength; i++)
    // {
    //     if ((int) buffer1[i] > 90 || ((int) buffer1[i] < 65 && (int) buffer1[i] != 32))
    //     {
    //         printf("otp_enc_d error: plaintext contains bad characters\n");
    //     }
    // }

    // // validate contents of key
    // for (i = 0; i < keyLength; i++)
    // {
    //     if ((int) buffer2[i] > 90 || ((int) buffer2[i] < 65 && (int) buffer2[i] != 32))
    //     {
    //         printf("otp_enc_d error: key contains bad characters\n");
    //     }
    // }

    // // compare length of plaintext to that of key
    // if (keyLength < plaintextLength)
    // {
    //     printf("Error: key '%s' is too short\n", argv[2]);
    // }

    // processing: create ciphertext
    for (i = 0; i < plaintextLength; i++)
    {
        // change spaces to asterisks
        if (buffer1[i] == ' ')
        {
            buffer1[i] = '@';
        }
        if (buffer2[i] == ' ')
        {
            buffer2[i] = '@';
        }

        // type conversion to int
        int msg = (int) buffer1[i];
        int key = (int) buffer2[i];

        // subtract 64 so that range is 0 - 26 (27 characters)
        msg = msg - 64;
        key = key - 64;

        // combine key and message using modular addition
        int cipher = (message + key) % 27;

        // add 64 back to that range is 64 - 90
        cipher = cipher + 64;

        // type conversion back to char
        buffer3[i] = (char) cipher + 0;

        // after encryption, change asterisks to spaces
        if (buffer3[i] == '@')
        {
            buffer3[i] = ' ';
        }
    }



    // do
    // {
    //     // receive ciphertext from otp_enc_d
    //     numReceived = recv(sockfd, buffer1, BUFFER_SIZE, 0);
    // }
    // while (numReceived > 0);

    // if (numReceived < plaintextLength)
    // {
    //    printf("Error: did not receive full ciphertext from otp_enc_d\n", port);
    //    exit(2);
    // }

    return 0;
}
