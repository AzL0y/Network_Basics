#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 80
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Configure address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket<->port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    printf("Server running on port %d...\n", PORT);

    // Accept client
    client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (client_fd < 0) {
        perror("Accept");
        exit(EXIT_FAILURE);
    }

    // Read client request
    read(client_fd, buffer, BUFFER_SIZE);
    printf("Client request:\n%s\n", buffer);

    // HTML content
    char html[] =
        "<HTML>"
        "<HEAD><TITLE>Hello!</TITLE></HEAD>"
        "<BODY>Bienvenue sur mon serveur TOTO le plus beau de tous les TOTOs.</BODY>"
        "</HTML>";

    // HTTP response (with correct headers)
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

    // Send response
    write(client_fd, response, strlen(response));

    // Close sockets
    close(client_fd);
    close(server_fd);

    return 0;
}

