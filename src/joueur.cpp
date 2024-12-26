// joueur.cpp
#include "joueur.h"

Joueur::Joueur() : pickSelectionne(-1), nbSlams(0) {
   // Initialisation des picks avec des cartes vides
   for(int i = 0; i < 4; i++) {
       picks[i] = Carte(0);
   }
}

void Joueur::selectionnerPick(int index) {
   if(index >= 0 && index < 4) {
       pickSelectionne = (pickSelectionne == index) ? -1 : index;
   }
}

bool Joueur::poserCarte(Slot& slot) {
   if(pickSelectionne < 0) return false;
   
   if(slot.peutRecevoirCarte(picks[pickSelectionne])) {
       slot.ajouterCarte(picks[pickSelectionne]);
       // Remplacer le pick utilisé
       picks[pickSelectionne] = stock.tirerCarte();
       picks[pickSelectionne].setVisible(true);
       pickSelectionne = -1;
       return true;
   }
   return false;
}

void Joueur::dessinerPicks(int x, int y) const {
   for(int i = 0; i < 4; i++) {
       picks[i].dessiner(x + i * (LARGEUR_CARTE + 10), y, i == pickSelectionne);
   }
}

void Joueur::dessinerStock(int x, int y) const {
   Carte dos(0);
   dos.dessiner(x, y);
   // Afficher nombre de cartes
   char texte[10];
   sprintf(texte, "%d", stock.getNbCartes());
   setColor(XAM_WHITE);
   textOut(x + LARGEUR_CARTE/2 - 10, y - 20, texte);
}
