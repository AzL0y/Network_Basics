/**
 * @file serveur_http.c
 * @brief HTTP server serving a static HTML page.
 *
 * --> creates a TCP server socket on port 80,
 * --> accepts a single client connection,
 * --> reads the HTTP request,
 * --> sends a simple HTML page as response.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

/**
 * @brief Port number to run the HTTP server on.
 */
#define PORT 80

/**
 * @brief Max buffer size for reading client requests.
 */
#define BUFFER_SIZE 1024

/**
 * @brief Main function of the HTTP server.
 *
 * Steps:
 * 1. Create a TCP socket.
 * 2. Bind the socket to PORT on all interfaces.
 * 3. Listen for incoming connections.
 * 4. Accept a client connection.
 * 5. Read the client's HTTP request.
 * 6. Build a static HTML response with HTTP headers.
 * 7. Send the response to the client.
 * 8. Close the client and server sockets.
 *
 * @return int Returns 0 on success, exits on failure.
 */
int main() {
    int server_fd; /**< Server socket file descriptor */
    int client_fd; /**< Client socket file descriptor */
    struct sockaddr_in address; /**< Server address structure */
    int addrlen = sizeof(address); /**< Size of the address struct */
    char buffer[BUFFER_SIZE]; /**< Buffer to store client request */

    /* Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    /* Configure address */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    /* Bind socket */
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    /* Listen for connections */
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server running on port %d...\n", PORT);

    /* Accept client connection */
    client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (client_fd < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    /* Read client request */
    read(client_fd, buffer, BUFFER_SIZE);
    printf("Client request:\n%s\n", buffer);

    /* HTML content */
    char html[] =
        "<HTML>"
        "<HEAD><TITLE>Hello!</TITLE></HEAD>"
        "<BODY>Bienvenue sur mon serveur TOTO le plus beau de tous les TOTOs.</BODY>"
        "</HTML>";

    /* HTTP response with headers */
    char response[BUFFER_SIZE];
    sprintf(response,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %lu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        strlen(html),
        html
    );

    /* Send response */
    write(client_fd, response, strlen(response));

    /* Close sockets */
    close(client_fd);
    close(server_fd);

    return 0;
}

