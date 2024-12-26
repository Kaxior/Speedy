#ifndef PAQUET_H
#define PAQUET_H

#include <vector>
#include "carte.h"

class Paquet {
private:
    std::vector<Carte> cartes;
    
public:
    Paquet();
    void melanger();              // Mélange aléatoirement les cartes
    Carte tirerCarte();           // Retire et renvoie la carte du dessus
    int getNbCartes() const;      // Nombre de cartes restantes
    void ajouterCarte(Carte c);   // Ajoute une carte au paquet
};

#endif