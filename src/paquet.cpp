// paquet.cpp
#include "paquet.h"
#include <algorithm>
#include <random>

Paquet::Paquet() {
   // Création du paquet initial (52 cartes)
   for(int i = 1; i <= 13; i++) {
       cartes.push_back(Carte(i));
   }
}

void Paquet::melanger() {
   std::random_device rd;
   std::mt19937 gen(rd());
   std::shuffle(cartes.begin(), cartes.end(), gen);
}

Carte Paquet::tirerCarte() {
   if(cartes.empty()) return Carte(0);
   Carte c = cartes.back();
   cartes.pop_back();
   return c;
}

void Paquet::ajouterCarte(Carte c) {
   cartes.push_back(c);
}

int Paquet::getNbCartes() const {
   return cartes.size();
}
