#include "jeu.h"
#include "utilitaires.h"
#include <iostream>

Jeu::Jeu() {
    initialiser();
}

void Jeu::initialiser() {
    // Initialisation des joueurs
    joueurs.emplace_back("Joueur 1");
    joueurs.emplace_back("Joueur 2");

    // Création des cartes (exemple simple avec valeurs 1-10)
    for (int i = 1; i <= 10; ++i) {
        pioche.emplace_back(i);
    }
    melangerPioche(pioche);
}

void Jeu::lancer() {
    afficherInstructions();
    bouclePrincipale();
}

void Jeu::afficherInstructions() const {
    std::cout << "Bienvenue dans Speedy !\n";
    std::cout << "Les règles sont simples : Défaussez vos cartes en premier !\n";
}

void Jeu::bouclePrincipale() {
    while (!pioche.empty()) {
        for (auto& joueur : joueurs) {
            if (joueur.estVide()) {
                std::cout << joueur.getNom() << " a gagné !\n";
                return;
            }

            Carte carte = pioche.back();
            pioche.pop_back();
            joueur.recevoirCarte(carte);
        }
    }
    std::cout << "Fin de la partie, égalité !\n";
}
