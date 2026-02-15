#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <ip_serveur> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock;
    struct sockaddr_in serv_addr;
    char *ip_serveur = argv[1];
    int port = atoi(argv[2]);

    // 1. Créer le socket
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

    // 2. Connexion au serveur
    printf("Connexion à %s:%d...\n", ip_serveur, port);
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("La connexion a échoué");
        return -1;
    }

    printf("Connecté au serveur !\n");
    char message[1024];
    char buffer_reponse[2048];

    while (1) {
        // 3. Lire le message de l'utilisateur
        printf("Entrez un message (ou 'quit' pour quitter) : ");
        if (!fgets(message, sizeof(message), stdin)) {
            printf("Erreur lecture message.\n");
            break;
        }

        // Supprimer le '\n' final
        message[strcspn(message, "\n")] = 0;

        // 4. Envoyer le message au serveur
        send(sock, message, strlen(message), 0);

        // 5. Vérifier si l'utilisateur veut quitter
        if (strcmp(message, "quit") == 0) {
            // On lit la dernière réponse ("Au revoir") du serveur
            int octets_recus = read(sock, buffer_reponse, sizeof(buffer_reponse) - 1);
            if (octets_recus > 0) {
                buffer_reponse[octets_recus] = '\0';
                printf("%s\n", buffer_reponse);
            }
            break;
        }

        // 6. Lire la réponse du serveur
        int octets_recus = read(sock, buffer_reponse, sizeof(buffer_reponse) - 1);
        if (octets_recus > 0) {
            buffer_reponse[octets_recus] = '\0';
            printf("Réponse serveur : %s\n", buffer_reponse);
        } else {
            printf("Le serveur a fermé la connexion.\n");
            break;
        }
    }

    // 7. Fermer le socket
    close(sock);
    printf("Connexion terminée.\n");

    return 0;
}

