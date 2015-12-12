/******************************************************************************
 ** Filename:    otp_dec_d.c
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
    int pid;
    int ciphertextLength;
    int port;
    socklen_t clilen;

    struct sockaddr_in server;
    struct sockaddr_in client;

    // make sure there are enough args
    if (argc < 2)
    {
        printf("Usage: otp_dec_d port\n");
        exit(1);
    }

    // validate port number
    sscanf(argv[1], "%d", &port);
    if (port < 0 || port > 65535)
    {
        printf("otp_dec_d: invalid port\n");
        exit(2);
    }

    // create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error: otp_dec_d could not create socket\n");
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
        printf("Error: otp_dec_d unable to bind socket to port %d\n", port);
        exit(2);
    }

    if (DEBUG)
    {
        fflush(stdout);
        printf("\notp_dec_d: bound socket on port %d\n", port);
    }

    // listen for connections
    if (listen(sockfd, 5) == -1)
    {
        printf("Error: otp_dec_d unable to listen on port %d\n", port);
        exit(2);
    }

    if (DEBUG)
    {
        printf("\notp_dec_d: now listening on port %d\n", port);
    }

    clilen = sizeof(client);

    // accept connections
    while (1)
    {
        newsockfd = accept(sockfd, (struct sockaddr *) &client, &clilen);
        if (newsockfd < 0)
        {
            printf("Error: otp_dec_d unable to accept connection\n");
            continue;
        }

        // every time a client connects, spawn off that process
        pid = fork();

        if (pid < 0)
        {
            perror("otp_dec_c: error on fork\n");
        }

        // child process, do work
        if (pid == 0)
        {
            // do work

            if (DEBUG)
            {
                printf("otp_dec_d: connection established with client\n");
            }

            // make sure not otp_enc_d though!!!

            // zero out buffer
            memset(buffer1, 0, BUFFER_SIZE);

            // receive ciphertext from otp_dec

            ciphertextLength = read(newsockfd, buffer1, BUFFER_SIZE);
            if (ciphertextLength < 0)
            {
                printf("Error: otp_end_d could not read ciphertext on port %d\n", port);
                exit(2);
            }

            if (DEBUG)
            {
                printf("otp_dec_d: ciphertext read: %d characters\n", ciphertextLength);
            }

            // send acknowledgement to client
            numSent = write(newsockfd, "!", 1);
            if (numSent < 0)
            {
                printf("otp_dec_d error sending acknowledgement to client\n");
                exit(2);
            }

            if (DEBUG)
            {
                printf("otp_dec_d: acknowledgement sent to client\n");
            }

            // zero out buffer
            memset(buffer2, 0, BUFFER_SIZE);

            // receive key from otp_dec
            keyLength = read(newsockfd, buffer2, BUFFER_SIZE);
            if (keyLength < 0)
            {
                printf("Error: otp_dec_d could not read key on port %d\n", port);
                exit(2);
            }

            if (DEBUG)
            {
                printf("otp_dec_d: key read: %d characters. now processing\n", keyLength);
            }

            // validate contents of ciphertext
            for (i = 0; i < ciphertextLength; i++)
            {
                if ((int) buffer1[i] > 90 || ((int) buffer1[i] < 65 && (int) buffer1[i] != 32))
                {
                    printf("otp_dec_d error: ciphertext contains bad characters\n");
                    exit(1);
                }
            }

            // validate contents of key
            for (i = 0; i < keyLength; i++)
            {
                if ((int) buffer2[i] > 90 || ((int) buffer2[i] < 65 && (int) buffer2[i] != 32))
                {
                    printf("otp_dec_d error: key contains bad characters\n");
                    exit(1);
                }
            }

            // compare length of ciphertext to that of key
            if (keyLength < ciphertextLength)
            { 
                printf("otp_dec_d error: key is too short\n");
                exit(1);
            }

            // processing: decrypt ciphertext
            for (i = 0; i < ciphertextLength; i++)
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

                // combine key and message using modular subtraction
                int decrypted = (msg - key) % 27;

                // add 64 back to that range is 64 - 90
                decrypted = decrypted + 64;

                // type conversion back to char
                buffer3[i] = (char) decrypted + 0;

                // after decryption, change asterisks to spaces
                if (buffer3[i] == '@')
                {
                    buffer3[i] = ' ';
                }
            }

            if (DEBUG)
            {
                printf("otp_dec_d: sending response\n");
            }

            // send plaintext to otp_enc
            numSent = write(newsockfd, buffer3, ciphertextLength);
            if (numSent < ciphertextLength)
            {
                printf("otp_dec_d error writing to socket\n");
                exit(2);
            }

            if (DEBUG)
            {
                printf("otp_dec_d: response sent\n");
            }

            // close sockets
            close(newsockfd);
            close(sockfd);

            exit(0);
        }

        else close(newsockfd);
    } // end while loop

    return 0;
}
