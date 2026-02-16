#include "mbed.h"
#include "EthernetInterface.h"
#include "C12832.h"
// 1. Initialisation de l'écran LCD (Brochage Application Board)
C12832 lcd(p5, p7, p6, p8, p11);
// 2. Création de l'interface réseau
EthernetInterface net;
int main() {
 // --- A. Configuration de l'écran et premier affichage ---
 lcd.cls();
 lcd.locate(0,0);
 lcd.printf("Config Static IP...");
 // --- B. Configuration de l'IP Statique ---
 // REMPLACEZ CES VALEURS par celles relevées sur le PC
 // Syntaxe : net.set_network(IP, MASQUE, PASSERELLE);

 net.set_network("XX.XX.XX.XX",  // IP
                 "XX.XX.XX.XX", // Mask   
                 "XX.XX.XX.XX");  // GW

 // --- C. Connexion ---
 // En statique, connect() applique simplement la configuration ci-dessus
 nsapi_error_t status = net.connect();
 if (status != 0) {
    lcd.cls();
    lcd.printf("Erreur Config !");
    return -1; // On quitte
 }
 // --- D. Affichage et Vérification ---
 // Note : get_ip_address() nécessite désormais un objet SocketAddress
SocketAddress ip_addr;
 net.get_ip_address(&ip_addr);
 const char *ip = ip_addr.get_ip_address();
 lcd.cls();
 lcd.locate(0,0);
 // Affichage sur l'écran LCD comme demandé
 lcd.printf("IP: %s", ip ? ip : "Erreur");

 lcd.locate(0,10);
 lcd.printf("Mode: Statique");
 // --- E. Boucle infinie ---
 while(1) {
    ThisThread::sleep_for(1s); // Remplace wait()
 }
}
