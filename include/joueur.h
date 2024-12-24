#ifndef JOUEUR_H
#define JOUEUR_H

#include "carte.h"
#include <string>
#include <vector>

class Joueur {
public:
    explicit Joueur(const std::string& nom);
    const std::string& getNom() const;
    void recevoirCarte(const Carte& carte);
    bool estVide() const;

private:
    std::string nom;
    std::vector<Carte> main;
};

#endif
