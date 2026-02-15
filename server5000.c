#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int main() {

    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket error");
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
    listen(server_fd, 5);
    printf("Server listening on port %d...\n", PORT);

    // Accept client
    client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (client_fd < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    while (1) {

        memset(buffer, 0, BUFFER_SIZE);

        // Receive keyword
        read(client_fd, buffer, 3);
        buffer[3] = '\0';

        printf("Received: %s\n", buffer);

       // Compare keywords
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
    close(client_fd);
    close(server_fd);
    printf("Server closed.\n");
    return 0;
}
