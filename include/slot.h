#ifndef SLOT_H
#define SLOT_H

#include <stack>
#include "carte.h"

class Slot {
private:
    std::stack<Carte> cartes;  // Pile de cartes posées
    
public:
    bool peutRecevoirCarte(const Carte& c) const;
    void ajouterCarte(Carte c);
    Carte getDerniereCarte() const;
    void dessiner(int x, int y) const;
    bool estVide() const { return cartes.empty(); }
};

#endif