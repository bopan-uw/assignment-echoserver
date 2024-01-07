//
// Created by B Pan on 12/3/23.
//
//for system calls, please refer to the MAN pages help in Linux
//sample echo server program over udp - CSS432 - winter 2024
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cstring>

#define SERV_UDP_PORT 61125
/* Size of maximum message to receive.                            */
#define MAX_MESG 2048

/* The dg_echo function receives data from the already initialized */
/* socket sockfd and returns them to the sender.                   */

int dg_echo(int sockfd) {
/* struct sockaddr is a general purpose data structure that holds  */
/* information about a socket that can use a variety of protocols. */
/* Here, we use Internet family protocols and UDP datagram ports.  */
/* This structure receives the client's address whenever a         */
/* datagram arrives, so it needs no initialization.                */

    struct sockaddr pcli_addr;

/* Temporary variables, counters and buffers.                      */

    int n;
    socklen_t cliLen;
    char mesg[MAX_MESG];

/* Main echo server loop. Note that it never terminates, as there  */
/* is no way for UDP to know when the data are finished.           */

    for ( ; ; ) {

/* Initialize the maximum size of the structure holding the        */
/* client's address.                                               */

        cliLen = sizeof(struct sockaddr);

/* Receive data on socket sockfd, up to a maximum of MAX_MESG       */
/* bytes, and store them in mesg. The sender's address is stored   */
/* in pcli_addr and the structure's size is stored in clilen.      */
        std::cout << "Waiting to receive data" << std::endl;
        n = recvfrom(sockfd, mesg, MAX_MESG, 0, &pcli_addr, &cliLen);

/* n holds now the number of received bytes, or a negative number  */
/* to show an error condition.                                    */

        if (n < 0) {
            perror("recvfrom error.");
            exit(3);
        }

/* Note that if you are using timeouts, n<0 may not mean an error, */
/* but that the call was interrupted by a signal. To see what      */
/* happened, you have to look at the value of the system variable  */
/* errno (defined in <errno.h>).                                   */

/* Send the received data back to the sender. The same socket      */
/* (sockfd) is used for the reverse direction, the buffer is mesg  */
/* with size n (the number of bytes received) and the sender's     */
/* address is as received in the previous call (pcli_addr with     */
/* size clilen). 0 is an unused flag byte. This call returns the   */
/* number of bytes sent, which differs from what we wanted in case */
/* of an error. Again, the return value may signify an interrupt.  */

        std::cout << "echo back to client." << std::endl;
        if (sendto(sockfd, mesg, n, 0, &pcli_addr, cliLen) != n) {
            perror("sendto error.");
            exit(4);
        }

        // Writing received data to file, DO NOT CHANGE.
        mesg[n] = 0;
        std::ofstream output;
        output.open("output_server.txt", std::ios::app);
        output << mesg;
        output.close();
    }
}

/* Main driver program. Initializes server's socket and calls the  */
/* dg_echo function that never terminates.                         */

int main(int argc, char *argv[]) {
    std::cout << "Starting server..." << std::endl;

/* General purpose socket structures are accessed using an         */
/* integer handle.                                                 */

    int sockfd;

/* The Internet specific address structure. We must cast this into */
/* a general purpose address structure when setting up the socket. */

    struct sockaddr_in serv_addr;

    /* Reset serv_addr to 0 */
    memset(&serv_addr, 0, sizeof(serv_addr));

    /*
     * TODO:
     *  1. Create a UDP socket. Use SOCK_DGRAM. Print error if creation fails.
     *      Create a UDP socket (an Internet datagram socket). AF_INET
     *      means Internet protocols and SOCK_DGRAM means UDP. 0 is an
     *      unused flag byte. A negative value is returned on error.
     *  2. Fill in serv_addr information
     *      To use the socket created, we must assign to it a local IP
     *      address and a UDP port number, so that the client can send data
     *      to it. To do this, we first prepare a sockaddr struct.
     *      As sockaddr is a general purpose structure, we must declare
     *      what type of address it holds.
     *      If the server has multiple interfaces, it can accept calls from
     *      any of them. Instead of using one of the server's addresses,
     *      we use INADDR_ANY to say that we will accept calls on any of
     *      the server's addresses. Note that we have to convert the host
     *      data representation to the network data representation.
     *      We must use a specific port for our server for the client to
     *      send data to (a well-known port).
     *  3. Bind. Associate the socket to serv_addr. Print error if bind fails.
     *      We initialize the socket pointed to by sockfd by binding to it
     *      the address and port information from serv_addr. Note that we
     *      must pass a general purpose structure rather than an Internet
     *      specific one to the bind call and also pass its size. A
     *      negative return value means an error occurred.
     */


/* We can now start the echo server's main loop. We only pass the  */
/* local socket to dg_echo, as the client's data are included in   */
/* all received datagrams.                                         */
    dg_echo(sockfd);

/* The echo function in this example never terminates, so this     */
/* code should be unreachable.                                     */
    return 0;
}