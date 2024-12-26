// slot.cpp
#include "slot.h"

bool Slot::peutRecevoirCarte(const Carte& c) const {
   if(cartes.empty()) return true;
   return c.estAdjacente(cartes.top());
}

void Slot::ajouterCarte(Carte c) {
   c.setVisible(true);
   cartes.push(c);
}

Carte Slot::getDerniereCarte() const {
   if(cartes.empty()) return Carte(0);
   return cartes.top();
}

void Slot::dessiner(int x, int y) const {
   if(!cartes.empty()) {
       cartes.top().dessiner(x, y);
   }
}
