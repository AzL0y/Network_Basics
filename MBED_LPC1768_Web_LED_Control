#include "mbed.h"
#include "EthernetInterface.h"
#include "C12832.h"
#include <string.h>

char buffer[1024];
char body[800];
char response[1024];

// --- A. DÉFINITION DES COMMANDES ---
enum CommandType {
    CMD_UNKNOWN,
    CMD_ON,
    CMD_OFF
};

// --- B. FONCTION DE TRADUCTION (PARSER) ---
CommandType parse_request(char* buffer) {

    if (strstr(buffer, "GET /on") != nullptr)
        return CMD_ON;

    if (strstr(buffer, "GET /off") != nullptr)
        return CMD_OFF;

    return CMD_UNKNOWN;
}
// --- PÉRIPHÉRIQUES ---
C12832 lcd(p5, p7, p6, p8, p11);
EthernetInterface net;
TCPSocket server;
DigitalOut led(LED1);

// --- FORCE L'ADRESSE MAC ---
extern "C" void mbed_mac_address(char *mac) {
    mac[0] = 0x4c;
    mac[1] = 0xd7;
    mac[2] = 0x17;
    mac[3] = 0xa1;
    mac[4] = 0x04;
    mac[5] = 0xd9;
}

int main() {

    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Init Reseau...");

    // Connexion DHCP
    if (net.connect() != 0) {
        lcd.printf(" Erreur!");
        return -1;
    }

    SocketAddress ip;
    net.get_ip_address(&ip);

    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("IP:");
    lcd.locate(0,10);
    lcd.printf("%s", ip.get_ip_address());

    // --- Démarrage Serveur ---
    if (server.open(&net) != NSAPI_ERROR_OK) {
        lcd.printf("Open Err");
        return -1;
    }

    if (server.bind(80) != NSAPI_ERROR_OK) {
        lcd.printf("Bind Err");
        return -1;
    }

    if (server.listen(5) != NSAPI_ERROR_OK) {
        lcd.printf("Listen Err");
        return -1;
    }



    while (true) {

        nsapi_error_t error;
        TCPSocket *client = server.accept(&error);

        lcd.locate(0,0);
        lcd.printf("Client Ok_0");


        if (client == nullptr || error != NSAPI_ERROR_OK) {
            ThisThread::sleep_for(100ms);
            continue;
        }

        lcd.locate(0,0);
        lcd.printf("Client Ok_1");


        //char buffer[1024];
        int n = client->recv(buffer, sizeof(buffer) - 1);

        if (n <= 0) {
            client->close();
            delete client;
            continue;
        }

        lcd.locate(0,0);
        lcd.printf("Client Ok_2");

        buffer[n] = '\0';

        // --- Analyse commande ---
        CommandType command = parse_request(buffer);

        //lcd.fillrect(0, 20, 128, 32, 0); // clear lower part

        lcd.locate(0,0);
        lcd.printf("Client Ok_3");

        switch (command) {
            case CMD_ON:
                led = 1;
                lcd.locate(0,0);
                lcd.printf("Action: ALLUMER");
                break;

            case CMD_OFF:
                led = 0;
                lcd.locate(0,0);
                lcd.printf("Action: ETEINDRE");
                break;

            default:
                lcd.locate(0,0);
                lcd.printf("Action: STATUS");
                break;
        }

        // --- Génération HTML ---
        //char body[800];

        int body_len = snprintf(body, sizeof(body),
            "<!DOCTYPE html><html>"
            "<head><style>"
            "body { font-family: Arial; text-align: center; margin-top: 50px; }"
            ".btn { padding: 15px 30px; font-size: 18px; text-decoration: none; color: white;"
            "border-radius: 5px; margin: 10px; display: inline-block; }"
            ".green { background-color: #28a745; }"
            ".red { background-color: #dc3545; }"
            "</style></head><body>"
            "<h1>Mbed Control</h1>"
            "<h2>Etat LED : <b style='color:%s'>%s</b></h2>"
            "<a href='/on' class='btn green'>ALLUMER</a>"
            "<a href='/off' class='btn red'>ETEINDRE</a>"
            "</body></html>",
            led ? "green" : "red",
            led ? "ON" : "OFF"
        );

        //char response[1024];

        int len = snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %d\r\n"
            "Connection: Keep-Alive\r\n\r\n"
            "%s",
            body_len,
            body
        );

        client->send(response, len);

        client->set_timeout(2000);
        delete client;

        ThisThread::sleep_for(50ms);
    }
}
