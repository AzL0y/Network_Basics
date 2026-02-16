#include "mbed.h"
#include "EthernetInterface.h"
#include "C12832.h"
C12832 lcd(p5, p7, p6, p8, p11);
EthernetInterface net;
// --- FORCE L'ADRESSE MAC AU NIVEAU SYSTEME
// Cette fonction remplace celle du driver matériel
extern "C" void mbed_mac_address(char *mac) {
mac[0] = 0xXX;
mac[1] = 0xXX;
mac[2] = 0xXX;              //XX:XX:XX:XX:XX:XX
mac[3] = 0xXX;
mac[4] = 0xXX;
mac[5] = 0xXX;
}
int main() {
    // 1. Initialisation
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Config MAC Spoofing...");
    // ---------------------------------------------------------
    // ETAPE A : Connexion DHCP
    // ---------------------------------------------------------
    // Sans arguments, connect() lance une requête DHCP standard.
    // Le serveur DHCP va croire que c'est le PC qui demande l'IP.
    nsapi_error_t status = net.connect();
    if (status != 0) {
        lcd.cls();
        lcd.printf("Echec DHCP !");
        return -1;
    }
    // ---------------------------------------------------------
    // ETAPE B : Vérification
    // ---------------------------------------------------------
    SocketAddress ip_addr;
    net.get_ip_address(&ip_addr);
    const char *mac_actuelle = net.get_mac_address();
    lcd.cls();
    // Affiche l'IP obtenue
    lcd.locate(0,0);
    lcd.printf("IP: %s", ip_addr.get_ip_address() ? ip_addr.get_ip_address() : "N/A");

    // Affiche l’adresse MAC pour prouver que l'usurpation a fonctionné
    lcd.locate(0,10);
    lcd.printf("MAC: %s", mac_actuelle ? mac_actuelle : "N/A");
    while(1) {
        ThisThread::sleep_for(1s);
 }
}
