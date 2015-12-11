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
    sscanf(argv[3], "%d", &port);
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
        printf("Error: key '%s' is too short\n", argv[2]);
    }

    // create socket 
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("Error: could not contact otp_enc_d on port %d\n", port);
        exit(2);
    }

    // connect to otp_enc_d
    if (connect(sockfd, (stuct sockaddr *) &server, sizeof(server)) == -1)
    {
        printf("Error: could not connect to otp_enc_d on port %d\n", port);
        exit(2);
    }

    struct sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr("192.168.1.1");


    // make sure not otp_dec_d though

    // send plaintext to otp_enc_d
    ssize_t numSent = send(sockfd, buffer1, plaintextLength - 1, 0);
    if (numSent < plaintextLength)
    {
        printf("Error: could not send plaintext to otp_enc_d on port %d\n", port);
        exit(2);
    }

    // send key to otp_enc_d
    numSent = send(sockfd, buffer2, keyLength - 1, 0);
    if (numSent < keyLength)
    {
        printf("Error: could not send key to otp_enc_d on port %d\n", port);
        exit(2);
    }

    ssize_t numReceived;

    do
    {
        // receive ciphertext from otp_enc_d
        numReceived = recv(sockfd, buffer1, BUFFER_SIZE, 0);
    }
    while (numReceived > 0);

    if (numReceived < plaintextLength)
    {
       printf("Error: did not receive full ciphertext from otp_enc_d\n", port);
       exit(2);
    }

    // output ciphertext to stdout
    for (i = 0; i < plaintextLength; i++)
    {
        printf("%c", buffer1[i]);
    }

    // add newline to ciphertext ouput
    printf("\n");

    return 0;
}
