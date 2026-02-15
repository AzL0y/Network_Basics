/**
 * @file serveur_envoie_affiche_texte.c
 * @brief TCP server that receives text messages and responds with Caesar cipher.
 *
 * --> accepts multiple client connections sequentially.
 * --> For each message received 
 *  	|--> applies a Caesar cipher (shift=10)
 * 	|--> sends the result back.
 * 
 * If the client sends "quit", the server
 * responds with a goodbye message and closes the connection.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/**
 * @brief Applies a Caesar cipher to the input string with shift=10 (A vaut K)
 *
 * @param msg The message to encrypt (modified in place)
 */
void translate(char *msg) {
    int shift = 10;  
    for (int i = 0; msg[i]; i++) {
        if (msg[i] >= 'a' && msg[i] <= 'z') {
            msg[i] = ((msg[i] - 'a' + shift) % 26) + 'a';
        } 
        else if (msg[i] >= 'A' && msg[i] <= 'Z') {
            msg[i] = ((msg[i] - 'A' + shift) % 26) + 'A';
        }
    }
}

/**
 * @brief Main function of the text server.
 *
 * Steps:
 * 1. Create a TCP socket.
 * 2. Bind to the given port (from command line).
 * 3. Listen for client connections.
 * 4. Accept clients sequentially.
 * 5. For each client:
 *    - Receive messages
 *    - If message == "quit", send goodbye and close client
 *    - Otherwise, encrypt with Caesar cipher and send back
 * 6. Loop indefinitely.
 *
 * @param argc Number of command line arguments
 * @param argv Command line arguments: argv[1] = port number
 * @return int Returns 0 on success, exits on failure
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int server_fd, client_fd; /**< Socket descriptors */
    struct sockaddr_in address; /**< Server address */
    socklen_t addrlen = sizeof(address);
    int port = atoi(argv[1]); /**< Server port */

    /* 1. Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    /* 2. Bind */
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /* 3. Listen */
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Serveur TOTO actif sur le port %d...\n", port);

    while (1) {
        /* 4. Accept */
        client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }
        printf("Client connecté.\n");

        char buffer[1024]; /**< Buffer for client messages */
        while (1) {
            memset(buffer, 0, sizeof(buffer));

            /* 5. Read message from client */
            int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
            if (bytes_read <= 0) {
                printf("Client déconnecté.\n");
                break;
            }
            buffer[bytes_read] = '\0';
            printf("Message reçu : %s\n", buffer);

            /* 6. Quit check */
            if (strncmp(buffer, "quit", 4) == 0) {
                char au_revoir[] = "Serveur TOTO : Au revoir !\n";
                send(client_fd, au_revoir, strlen(au_revoir), 0);
                printf("Client a quitté.\n");
                break;
            }

            /* 7. Encrypt message */
            translate(buffer);

            /* 8. Send encrypted message */
            char message_reponse[1024];
            snprintf(message_reponse, sizeof(message_reponse), "Serveur TOTO : %.1006s\n", buffer);
            send(client_fd, message_reponse, strlen(message_reponse), 0);
        }

        /* 9. Close client connection */
        close(client_fd);
        printf("-----------------\n");
    }

    close(server_fd);
    return 0;
}

