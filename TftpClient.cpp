//
// Created by B Pan on 12/3/23.
//

//Sample echo client program - CSS 432 - Winter 2024

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>


#define SERV_UDP_PORT 61125
#define SERV_HOST_ADDR "127.0.0.1"

/* Size of maximum message to send.                                */
#define MAX_LINE 512

/* The dg_cli function reads lines from the terminal, sends them   */
/* to the echo server pointed to by pserv_addr, and prints to the  */
/* terminal the echoed data. The local endpoint is sockfd. The     */
/* function returns when an EOF is seen. Note that the server's    */
/* address in pserv_addr is already initialized. Its size is held  */
/* in serv_len, so that the function works with other protocol     */
/* families that have different address sizes.                     */

void dg_cli(int sockfd, struct sockaddr *pserv_addr, int serv_len) {

/* Various counter and buffer variables. The extra byte in the     */
/* receive buffer is used to add a null to terminate the string,   */
/* as the network routines do not use nulls but I/O functions do.  */

    size_t n;
    char send_line[MAX_LINE], recv_line[MAX_LINE + 1];

/* Main client loop. Terminates on EOF. Get terminal input on the  */
/* send_line buffer (up to MAX_LINE bytes).                        */

    while (fgets(send_line, MAX_LINE, stdin) != nullptr) {

/* Find the string's length to pass it as a parameter to the send  */
/* calls (excluding the null in the end).                          */

        n = strlen(send_line);

/* Send data using the sockfd socket, to the server at pserv_addr. */
/* We also pass the send buffer and its size (send_line and n), an */
/* unused flag byte (0) and the server's address size (serv_len).  */
/* The returned number is the number of bytes sent. If it is not n */
/* either an error or an interrupt has occurred.                   */
        printf("Sending data to server: %s\n", send_line);
        if (sendto(sockfd, send_line, n, 0, pserv_addr, serv_len) != n) {
            perror("sendto error on socket.");
            exit(3);
        }

/* Read the echoed data from the same socket into the recv_line    */
/* buffer (up to MAX_LINE bytes). The server's address and its size*/
/* are returned, but we do not need them, so we use null pointers. */
/* The return value is the number of bytes received.               */

        std::cout << "Waiting to receive data from server" << std::endl;

        n = recvfrom(sockfd, recv_line, MAX_LINE, 0, nullptr, nullptr);
        if (n < 0) {
            perror("recvfrom error.");
            exit(4);
        } else {
            std::cout << "Received data from server: " << std::endl;

/* The exchanged data is not null terminated, as the string length */
/* is explicitly sent. We need to null terminate the string before */
/* using fputs to output it on the terminal.                       */

            recv_line[n] = 0;
            fputs(recv_line, stdout);
            std::cout << "\n************\n" << std::endl;

            // Writing received output to file. DO NOT CHANGE.
            std::ofstream output;
            output.open("output_client.txt", std::ios::app);
            output << recv_line;
            output.close();
        }
    }
}

/* The main program sets up the local socket for communication     */
/* and the server's address and port (well-known numbers) before   */
/* calling the dg_cli main loop.                                   */

int main(int argc, char *argv[]) {
    std::cout << "Starting client..." << std::endl;
    int sockfd; // socket descriptor

/* We need to set up two addresses, one for the client and one for */
/* the server.                                                     */

    struct sockaddr_in cli_addr, serv_addr;

/* Reset serv_addr and cli_addr */

    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(&cli_addr, 0, sizeof(cli_addr));

    /*
     *  TODO:
     *   1. Fill in serv_addr. Use AF_INET, SERV_HOST_ADDR and SERV_UDP_PORT
     *      The system needs a 32 bit integer as an Internet address, so we
     *      use inet_addr to convert the dotted decimal notation to it.
     *   2. Fill in cli_addr. Use INADDR_ANY to let system choose any address.
     *      The client can also choose any port for itself (it is not
     *      well-known). Using 0 as port number lets the system allocate any free
     *      port to our program. Note that we have to convert the host
     *      data representation to the network data representation. See "htonl"
     *      and "htons"
     *   3. Create a UDP socket, using SOCK_DGRAM. Print error if creation fails
     *   4. Bind. Bind the socket to cli_addr. Print error if bind fails.
     *      The initialized address structure can be now associated with
     *      the socket we created. Note that we use a different parameter
     */


/* Call the main client loop. We need to pass the socket to use    */
/* on the local endpoint, and the server's data that we already    */
/* set up, so that communication can start from the client.        */

    std::cout << "Type a message to send to server..." << std::endl;
    dg_cli(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

/* We return here after the client sees the EOF and terminates.    */
/* We can now release the socket and exit normally.                */

    close(sockfd);
    exit(0);
}
