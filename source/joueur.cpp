#include "joueur.h"
#include <iostream>

Joueur::Joueur(const std::string& nom) : nom(nom) {}

const std::string& Joueur::getNom() const {
    return nom;
}

void Joueur::recevoirCarte(const Carte& carte) {
    main.push_back(carte);
    std::cout << nom << " a reçu la carte : " << carte.getValeur() << "\n";
}

bool Joueur::estVide() const {
    return main.empty();
}
