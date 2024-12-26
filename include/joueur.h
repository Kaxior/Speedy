#ifndef JOUEUR_H
#define JOUEUR_H

#include <array>
#include "paquet.h"
#include "slot.h"

class Joueur {
private:
    Paquet stock;              // Cartes restantes du joueur
    std::array<Carte,4> picks; // Cartes visibles jouables
    int pickSelectionne;       // Index du pick sélectionné (-1 si aucun)
    int nbSlams;              // Nombre de slams réussis
    
public:
    Joueur();
    void selectionnerPick(int index);
    bool poserCarte(Slot& slot);  // Tente de poser le pick sélectionné
    void dessinerPicks(int x, int y) const;
    void dessinerStock(int x, int y) const;
    int getNbSlams() const { return nbSlams; }
    void ajouterSlam() { nbSlams++; }
};

#endif