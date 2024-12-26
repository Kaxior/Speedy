#ifndef CARTE_H
#define CARTE_H

#include <string>

class Carte {
private:
    int valeur;     // Valeur de la carte (1-13)
    std::string couleur;  // Couleur de la carte
    bool visible;   // La carte est-elle visible ?

public:
    // Constructeur par défaut
    Carte();
    
    // Constructeur avec paramètres
    Carte(int val, const std::string& coul);

    // Getters
    int getValeur() const;
    std::string getCouleur() const;
    bool estVisible() const;

    // Setters
    void setValeur(int val);
    void setCouleur(const std::string& coul);
    void setVisible(bool vis);

    // Méthode pour afficher la carte
    std::string toString() const;
};

#endif // CARTE_H