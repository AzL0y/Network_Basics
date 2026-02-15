#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    	if (argc < 2) {
        	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        	exit(EXIT_FAILURE);
    	}

    	int server_fd, client_fd;
    	struct sockaddr_in address;
    	int addrlen = sizeof(address);
    	int port = atoi(argv[1]);

    	// ACTION : socket() -> ÉTAT : CLOSED
    	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	    if(server_fd < 0) {
		perror("socket");
		return 1;
	}
    
    	int opt = 1;
    	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	    memset(&address, 0, sizeof(address));
    	address.sin_family = AF_INET;
    	address.sin_addr.s_addr = INADDR_ANY; // Écoute sur toutes les interfaces
    	address.sin_port = htons(port);

    	// ACTION : bind() -> ÉTAT : BOUND
    	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        	perror("Bind échoué");
        	exit(EXIT_FAILURE);
    	}

    	// ACTION : listen() -> ÉTAT : LISTEN
    	if(listen(server_fd, 3) < 0){
		perror("listen");
		return 1;
	}
    	
	printf("Serveur PING-PONG prêt sur le port %d...\n", port);

    	while(1) {
        	// ACTION : accept() -> ÉTAT : ESTABLISHED
        	client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        	
		if (client_fd < 0) {
			perror("accept");
			continue;
		}
		
        	char *ip_client = inet_ntoa(address.sin_addr);
        	printf("Client connecté (%s). En attente de PING...\n", ip_client);

        	char buffer_in;
        	// read() est BLOQUANTE
        	if (read(client_fd, &buffer_in, 1) > 0) {
            		if (buffer_in == 'P') {
                	printf("PING reçu ! Envoi du PONG...\n");
                	char buffer_out = 'O';
                	send(client_fd, &buffer_out, 1, 0);
            		}		
        	}

        	// ACTION : close() -> ÉTAT : CLOSED (pour ce client)
        	close(client_fd);
        	printf("Session terminée.\n-----------------\n");
    	}

	close(server_fd);
    	return 0;
}
