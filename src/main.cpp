#include <iostream>
#include "include/carte.h"

int main() {
    // Création de quelques cartes
    Carte carte1(7, "Coeur");
    Carte carte2(12, "Pique");

    // Test des méthodes
    std::cout << "Carte 1 (non visible) : " << carte1.toString() << std::endl;
    
    carte1.setVisible(true);
    std::cout << "Carte 1 (visible) : " << carte1.toString() << std::endl;
    
    std::cout << "Valeur de la carte 2 : " << carte2.getValeur() << std::endl;

    return 0;
}