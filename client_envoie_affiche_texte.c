/**
 * @file client_envoi_affiche_texte.c
 * @brief TCP client that sends text messages to a server and displays responses.
 *
 * --> connects to a TCP server on a given IP and port,
 * --> sends user input messages
 * --> prints the server's responses.
 * 
 * The client can exit by typing "quit".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/**
 * @brief Main function of the text client.
 *
 * Steps:
 * 1. Create a TCP socket.
 * 2. Connect to the server using IP and port from command line arguments.
 * 3. Loop:
 *    - Read user message from stdin
 *    - Send it to the server
 *    - Receive server response and print it
 *    - Exit if user types "quit"
 * 4. Close socket and terminate.
 *
 * @param argc Number of command line arguments
 * @param argv Command line arguments: argv[1]=server IP, argv[2]=port
 * @return int Returns 0 on success, exits on failure.
 */
int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <ip_serveur> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock; /**< Socket file descriptor */
    struct sockaddr_in serv_addr; /**< Server address structure */
    char *ip_serveur = argv[1]; /**< Server IP */
    int port = atoi(argv[2]); /**< Server port */

    /* 1. Create socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erreur création socket");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip_serveur, &serv_addr.sin_addr) <= 0) {
        fprintf(stderr, "Adresse IP invalide ou non supportée\n");
        return -1;
    }

    /* 2. Connect to server */
    printf("Connexion à %s:%d...\n", ip_serveur, port);
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("La connexion a échoué");
        return -1;
    }
    printf("Connecté au serveur !\n");

    char message[1024]; /**< User input message */
    char buffer_reponse[2048]; /**< Server response buffer */

    while (1) {
        /* 3. Read user message */
        printf("Entrez un message (ou 'quit' pour quitter) : ");
        if (!fgets(message, sizeof(message), stdin)) {
            printf("Erreur lecture message.\n");
            break;
        }

        message[strcspn(message, "\n")] = 0; // Remove newline

        /* 4. Send message to server */
        send(sock, message, strlen(message), 0);

        /* 5. Quit if requested */
        if (strcmp(message, "quit") == 0) {
            int octets_recus = read(sock, buffer_reponse, sizeof(buffer_reponse) - 1);
            if (octets_recus > 0) {
                buffer_reponse[octets_recus] = '\0';
                printf("%s\n", buffer_reponse);
            }
            break;
        }

        /* 6. Receive server response */
        int octets_recus = read(sock, buffer_reponse, sizeof(buffer_reponse) - 1);
        if (octets_recus > 0) {
            buffer_reponse[octets_recus] = '\0';
            printf("Réponse serveur : %s\n", buffer_reponse);
        } else {
            printf("Le serveur a fermé la connexion.\n");
            break;
        }
    }

    /* 7. Close socket */
    close(sock);
    printf("Connexion terminée.\n");

    return 0;
}

