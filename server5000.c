/**
 * @file server5000.c
 * @brief TCP server that responds to 3-letter keywords from a client.
 *
 * --> creates a TCP socket,
 * --> binds it to port 5000,
 * --> listens for a client,
 * --> receives 3-letter keywords,
 * --> responds with predefined messages.
 * 
 * The server stops when the client sends the keyword "qui" (quit).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

/**
 * @brief Port number the server listens on.
 */
#define PORT 5000

/**
 * @brief Max size of the buffer for receiving client messages.
 */
#define BUFFER_SIZE 1024

/**
 * @brief Main function of the TCP server.
 *
 * Steps performed:
 * 1. Create a TCP socket.
 * 2. Bind the socket to PORT on all interfaces (INADDR_ANY).
 * 3. Listen for incoming client connections.
 * 4. Accept a client connection.
 * 5. Continuously receive 3-letter keywords from the client.
 * 6. Respond with a predefined message for each keyword:
 *    - "tem": temperature message
 *    - "tim": time message
 *    - "mus": music message
 *    - "qui": quit (server exits loop)
 *    - other: unknown keyword message
 * 7. Close the client and server sockets and terminate.
 *
 * @return int Returns 0 on success, exits with failure otherwise.
 */
int main() {

    int server_fd; /**< Server socket file descriptor */
    int client_fd; /**< Client socket file descriptor */
    struct sockaddr_in address; /**< Server address structure */
    int addrlen = sizeof(address); /**< Size of address structure */
    char buffer[BUFFER_SIZE] = {0}; /**< Buffer for receiving client messages */

    /* Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    /* Configure server address */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    /* Bind socket to port */
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    /* Listen for connections */
    listen(server_fd, 5);
    printf("Server listening on port %d...\n", PORT);

    /* Accept a client */
    client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (client_fd < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Client connected.\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        /* Receive 3-letter keyword from client */
        read(client_fd, buffer, 3);
        buffer[3] = '\0'; /* Null-terminate the keyword */
        printf("Received: %s\n", buffer);

        /* Respond based on keyword */
        if (strcmp(buffer, "tem") == 0) {
            char *msg = "Il fait -10°C (trop froid)\n";
            write(client_fd, msg, strlen(msg));
        }
        else if (strcmp(buffer, "tim") == 0) {
            char *msg = "Il est 23:30 quelque part dans le monde\n";
            write(client_fd, msg, strlen(msg));
        }
        else if (strcmp(buffer, "mus") == 0) {
            char *msg = "Currently playing : Sting Ft. Cheb Mami - Desert Rose\n";
            write(client_fd, msg, strlen(msg));
        }
        else if (strcmp(buffer, "qui") == 0) {
            printf("Client requested quit.\n");
            break;
        }
        else {
            char *msg = "Unknown keyword\n";
            write(client_fd, msg, strlen(msg));
        }
    }

    /* Close sockets */
    close(client_fd);
    close(server_fd);
    printf("Server closed.\n");

    return 0;
}

