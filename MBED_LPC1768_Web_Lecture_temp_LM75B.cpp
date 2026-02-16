#include "mbed.h"
#include "EthernetInterface.h"
#include "C12832.h"
#include "LM75B.h"

// --- Déclarations des périphériques ---
C12832 lcd(p5, p7, p6, p8, p11);
EthernetInterface net;
TCPSocket server;
LM75B sensor(p28, p27);

// --- Force l'adresse MAC ---
extern "C" void mbed_mac_address(char *mac) {
    mac[0] = 0xXX;
    mac[1] = 0xXX;
    mac[2] = 0xXX;
    mac[3] = 0xXX;
    mac[4] = 0xXX;
    mac[5] = 0xXX;
}

int main() {

    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Config MAC & DHCP...");

    // Connexion réseau
    if (net.connect() != 0) {
        lcd.locate(0,10);
        lcd.printf("Erreur DHCP !");
        return -1;
    }

    SocketAddress ip;
    net.get_ip_address(&ip);

    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("IP: %s", ip.get_ip_address());
    lcd.locate(0,10);
    lcd.printf("Port: 80 (Pret)");

    // Configuration serveur
    server.open(&net);
    server.bind(80);
    server.listen(5);

    while (true) {

        nsapi_error_t error;
        TCPSocket *client = server.accept(&error);

        if (error == NSAPI_ERROR_OK && client != NULL) {

            char recv_buffer[512];
            int received = client->recv(recv_buffer, sizeof(recv_buffer)-1);

            if (received > 0) {
                recv_buffer[received] = '\0';
            }

            float temp_actuelle = sensor.temp();

            // --- Manual float formatting ---
            int temp_int = (int)temp_actuelle;
            int temp_dec = (int)((temp_actuelle - temp_int) * 100);

            if (temp_dec < 0) temp_dec = -temp_dec;

            char response[1024];

            int len = snprintf(response, sizeof(response),
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Connection: close\r\n\r\n"
                "<!DOCTYPE html><html>"
                "<head><meta http-equiv='refresh' content='2'>"
                "<style>"
                "body { font-family: Arial; text-align: center; margin-top: 50px; background-color: #f4f4f9; }"
                ".card { background: white; padding: 30px; border-radius: 10px; display: inline-block; "
                "box-shadow: 0 4px 8px rgba(0,0,0,0.1); }"
                ".temp { font-size: 60px; color: #007BFF; font-weight: bold; }"
                "</style></head><body>"
                "<div class='card'>"
                "<h1>Salle de TP - Capteur 1</h1>"
                "<p>Temperature en temps reel :</p>"
                "<div class='temp'>%d.%02d &deg;C</div>"
                "<p><small>Mise a jour automatique (2s)</small></p>"
                "</div>"
                "</body></html>",
                temp_int, temp_dec
            );

            if (len > 0) {
                client->send(response, len);
            }

            client->close();
            delete client;

            lcd.locate(0,10);
            lcd.printf("Dernier: %d.%02d C   ", temp_int, temp_dec);
        }
    }
}
