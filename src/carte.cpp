#include "../include/carte.h"

// Constructeur par défaut
Carte::Carte() : valeur(0), couleur(""), visible(false) {}

// Constructeur avec paramètres
Carte::Carte(int val, const std::string& coul) 
    : valeur(val), couleur(coul), visible(false) {}

// Getters
int Carte::getValeur() const { return valeur; }
std::string Carte::getCouleur() const { return couleur; }
bool Carte::estVisible() const { return visible; }

// Setters
void Carte::setValeur(int val) { valeur = val; }
void Carte::setCouleur(const std::string& coul) { couleur = coul; }
void Carte::setVisible(bool vis) { visible = vis; }

// Méthode pour afficher la carte
std::string Carte::toString() const {
    if (!visible) return "Carte cachée";
    return std::to_string(valeur) + " de " + couleur;
}
