#ifndef CARTE_H
#define CARTE_H

class Carte {
public:
    explicit Carte(int valeur);
    int getValeur() const;

private:
    int valeur;
};

#endif
