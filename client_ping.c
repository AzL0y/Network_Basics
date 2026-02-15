#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

    
int main(int argc, char *argv[]) {
	
	int sock; 
	struct sockaddr_in serv_addr;
	char *ip_serveur = argv[1];
	int port = atoi(argv[2]);

    	if (argc < 3) {
        	fprintf(stderr, "Usage: %s <ip_serveur> <port>\n", argv[0]);
        	exit(EXIT_FAILURE);
    	}

    	ip_serveur = argv[1];
    	port = atoi(argv[2]);

    	// 1. ACTION : socket() - Création Socket
    	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        	perror("Erreur création socket");
        	return -1;
    	}

    	memset(&serv_addr, 0, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET;
    	serv_addr.sin_port = htons(port);

    	// Conversion de l'adresse IP texte en binaire   
	if (inet_pton(AF_INET, ip_serveur, &serv_addr.sin_addr) <= 0) {
       		perror("Adresse IP invalide\n");
		close(sock);
        	return -1;
	}

    	// 2. ACTION : connect() -> ÉTAT : ESTABLISHED
    	printf("Connexion à %s:%d...\n", ip_serveur, port);
    	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        	perror("Connexion échouée");
        	close(sock);
		return -1;
    		}

    	// 3. Échange de données
    	char ping = 'P';
    	send(sock, &ping, 1, 0);
    	printf("PING envoyé. Attente de réponse...\n");

    	char pong;
    	if (read(sock, &pong, 1) > 0) {
        	if (pong == 'O') {
            		printf("Succès ! PONG reçu du serveur.\n");
        		}
		else printf("Reponse innatendue: %c\n", pong);

    	}
	else perror("Erreur lecture");

    	// 4. ACTION : close()
    	close(sock);
    	return 0;
}

