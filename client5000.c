/**
 * @file client5000.c
 * @brief TCP client that connects to a server and exchanges keywords.
 *
 * --> creates a TCP socket 
 * --> connects to a server running on localhost,
 * --> sends a 3-letter keyword
 * --> receives the server's reply,
 * --> displays it.
 * 
 * The client continues until the user enters the keyword "qui" (quit).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

/**
 * @brief Port number to connect to on the server.
 */
#define PORT 5000

/**
 * @brief Max size of the buffer for receiving server messages.
 */
#define BUFFER_SIZE 1024

/**
 * @brief Main function of the TCP client.
 *
 * 		--Steps--
 * 1. Creates a TCP socket.
 * 2. Configures the server address (localhost:5000).
 * 3. Connects to the server.
 * 4. Continuously reads 3-letter keywords from the user.
 * 5. Sends the keyword to the server.
 * 6. Receives and prints the server's response.
 * 7. Terminates when the keyword "qui" is entered.
 *
 * @return int Returns 0 on success, exits with failure otherwise.
 */
int main() {

    int sock; /**< Socket file descriptor */
    struct sockaddr_in server_addr; /**< Server address structure */
    char keyword[4]; /**< Buffer to store 3-letter keyword plus null terminator */
    char buffer[BUFFER_SIZE]; /**< Buffer to store server response */

    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    /* Configure server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Connect to server */
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server.\n");

    while (1) {

        printf("Enter 3-letter keyword (tem/tim/mus/qui): ");
        scanf("%3s", keyword);

        /* Send keyword to server */
        write(sock, keyword, 3);

        /* Exit loop if keyword is 'qui' */
        if (strcmp(keyword, "qui") == 0) {
            break;
        }

        /* Receive reply from server */
        memset(buffer, 0, BUFFER_SIZE);
        read(sock, buffer, BUFFER_SIZE);
        printf("Server: %s\n", buffer);
    }

    /* Close the socket */
    close(sock);
    return 0;
}

