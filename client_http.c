/**
 * @file client_http.c
 * @brief HTTP client connecting to localhost and retrieving the homepage.
 *
 * --> creates a TCP socket,
 * --> connects to a server on localhost:80,
 * --> sends an HTTP GET request, 
 * --> prints the HTTP response to the console.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

/**
 * @brief Port number of the HTTP server.
 */
#define PORT 80

/**
 * @brief Max size of buffer for receiving server response.
 */
#define BUFFER_SIZE 4096

/**
 * @brief Main function of the HTTP client.
 *
 * Steps:
 * 1. Create a TCP socket.
 * 2. Configure the server address (127.0.0.1:80).
 * 3. Connect to the server.
 * 4. Send a simple HTTP GET request.
 * 5. Read the server's response and print it.
 * 6. Close the socket and exit.
 *
 * @return int Returns 0 on success, exits on failure.
 */
int main() {
    int sock; /**< Socket file descriptor */
    struct sockaddr_in server_addr; /**< Server address structure */
    char buffer[BUFFER_SIZE]; /**< Buffer to store server response */

    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    /* Configure server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost

    /* Connect to server */
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server on port %d\n\n", PORT);

    /* HTTP GET request */
    char request[] =
        "GET / HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Connection: close\r\n"
        "\r\n";

    /* Send request */
    send(sock, request, strlen(request), 0);

    /* Receive response */
    int bytes_read;
    while ((bytes_read = read(sock, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);
    }

    /* Close socket */
    close(sock);

    return 0;
}

