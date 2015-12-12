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
#include <netinet/in.h>
#include <stdio.h>     // for printf
#include <stdlib.h>    // for exit
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE    128000
#define DEBUG          0


int main(int argc, char** argv)
{

    char buffer1[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    char buffer3[BUFFER_SIZE];
    int sockfd;
    int newsockfd;
    int i;
    int keyLength;
    int numSent;
    int plaintextLength;
    int port;
    socklen_t clilen;

    struct sockaddr_in server;
    struct sockaddr_in client;

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

    // create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error: opt_enc_d could not create socket\n");
        exit(1);
    }

    // zero out the IP address memory space
    memset(&server, '\0', sizeof(server));


    // set up an address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // bind socket to a port
    if (bind(sockfd, (struct sockaddr *) &server, sizeof(server)) < 0)
    {
        printf("Error: otp_enc_d unable to bind socket to port %d\n", port);
        exit(2);
    }

    // listen for connections
    if (listen(sockfd, 5) == -1)
    {
        printf("Error: otp_enc_d unable to listen on port %d\n", port);
        exit(2);
    }

    if (DEBUG)
    {
        fflush(stdout);
        printf("\notp_enc_d: now listening on port %d\n", port);
    }

    clilen = sizeof(client);

    // accept connections
//    while (1)
//    {
//        if ((newsockfd = accept(sockfd, NULL, NULL)) == -1)
        newsockfd = accept(sockfd, (struct sockaddr *) &client, &clilen);
        if (newsockfd < 0)
        {
            printf("Error: opt_enc_d unable to accept connection\n");
 //           continue;
            exit(2); 
        }
//    }

    // bzero(buffer,256);

    // every time a client connects, spawn off that process
    // grab code from smallsh

    if (DEBUG)
    {
        printf("opt_enc_d: connection established with client\n");
    }


    // make sure not otp_dec_d though!!!

    // receive plaintext from otp_enc
    // ssize_t numRead = send(sockfd, buffer1, plaintextLength - 1, 0);
    plaintextLength = read(newsockfd, buffer1, BUFFER_SIZE);
    if (plaintextLength < 0)
    {
        printf("Error: otp_end_d could not read plaintext on port %d\n", port);
        exit(2);
    }

    if (DEBUG)
    {
        printf("opt_enc_d: plaintext read: %d characters\n", plaintextLength);
    }

    // send acknowledgement to client
    numSent = write(newsockfd, "!", 1);
    if (numSent < 0)
    {
        printf("otp_enc_d error sending acknowledgement to client\n");
        exit(2);
    }

    if (DEBUG)
    {
        printf("opt_enc_d: acknowledgement sent to client\n");
    }

    // receive key from otp_enc
    // numSent = send(sockfd, buffer2, keyLength - 1, 0);
    keyLength = read(newsockfd, buffer2, BUFFER_SIZE);
    if (keyLength < 0)
    {
        printf("Error: otp_end_d could not read key on port %d\n", port);
        exit(2);
    }

    // ssize_t numReceived;

    if (DEBUG)
    {
        printf("opt_enc_d: key read: %d characters. now processing\n", keyLength);
    }

    // validate contents of plaintext
    for (i = 0; i < plaintextLength; i++)
    {
        if ((int) buffer1[i] > 90 || ((int) buffer1[i] < 65 && (int) buffer1[i] != 32))
        {
            printf("otp_enc_d error: plaintext contains bad characters\n");
            exit(1);
        }
    }

    // // validate contents of key
    for (i = 0; i < keyLength; i++)
    {
        if ((int) buffer2[i] > 90 || ((int) buffer2[i] < 65 && (int) buffer2[i] != 32))
        {
            printf("otp_enc_d error: key contains bad characters\n");
            exit(1);
        }
    }

    // compare length of plaintext to that of key
    if (keyLength < plaintextLength)
    {
        printf("otp_enc_d error: key is too short\n");
        exit(1);
    }

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
        int cipher = (msg + key) % 27;

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

    if (DEBUG)
    {
        printf("opt_enc_d: sending response\n");
    }

    // send ciphertext to otp_enc
    numSent = write(newsockfd, buffer3, plaintextLength);
    if (numSent < plaintextLength)
    {
        printf("otp_enc_d error writing to socket\n");
        exit(2);
    }

    if (DEBUG)
    {
        printf("opt_enc_d: response sent\n");
    }

    // close sockets
    close(newsockfd);
    close(sockfd);


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
