#include "mbed.h"
#include "EthernetInterface.h"
#include "C12832.h"
// 1. Déclaration des périphériques
C12832 lcd(p5, p7, p6, p8, p11);
EthernetInterface net;
TCPSocket server;
extern "C" void mbed_mac_address(char *mac) {
mac[0] = 0xXX;
mac[1] = 0xXX;
mac[2] = 0xXX;              //XX:XX:XX:XX:XX:XX
mac[3] = 0xXX;
mac[4] = 0xXX;
mac[5] = 0xXX;
}
int main() {
 lcd.cls();
 lcd.locate(0,0);
 lcd.printf("Config MAC & DHCP...");
 // Connexion au réseau
 if (net.connect() != 0) {
    lcd.cls(); lcd.printf("Echec DHCP !");
    return -1;
    }
    SocketAddress ip_server;
    net.get_ip_address(&ip_server);

    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("IP: %s", ip_server.get_ip_address());
    lcd.locate(0,10);
    lcd.printf("Port: 80 (Pret)");
    // 3. Démarrage du Serveur avec TCPSocket
    server.open(&net);
    server.bind(80);
    server.listen(5);
    while (true) {
        nsapi_error_t error;
        // Acceptation du client : la méthode retourne un pointeur vers un nouveau socket
        TCPSocket *client = server.accept(&error);
        if (error == NSAPI_ERROR_OK) {
            SocketAddress client_addr;
            client->getpeername(&client_addr); // Récupère l'IP du client
            lcd.cls();
            lcd.locate(0,0);
            lcd.printf("Client: %s", client_addr.get_ip_address());
            // Réception
            char buffer_recv[512]; // Réduit pour économiser la RAM
            client->recv(buffer_recv, sizeof(buffer_recv));
            // Construction de la réponse avec Entêtes + Séparateur + HTML
            char buffer_resp[1024];
            int len = sprintf(buffer_resp,
            "HTTP/1.1 200 OK\r\n" // Statut : Succès
            "Content-Type: text/html\r\n" // Type : Page Web
            "Connection: close\r\n" // Gestion : On ferme après
            "\r\n" // SÉPARATEUR OBLIGATOIRE
            "<!DOCTYPE html>" // Début du HTML
            "<html>"
            "<head><title>Mbed Server</title></head>"
            "<body>"
            "<h1>Hello World !</h1>"
            "<p>Ceci est mon serveur Mbed HTTP.</p>"
            "</body>"
            "</html>"
            );
            client->send(buffer_resp, sizeof(buffer_resp) - 1);
            // Fermeture et libération de la mémoire
            client->close();
            delete client; // IMPORTANT : accept() crée un objet avec 'new'
            lcd.locate(0,10);
            lcd.printf("Pret pour suivant...");
        }
    }
}
