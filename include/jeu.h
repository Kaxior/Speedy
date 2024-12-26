#ifndef JEU_H
#define JEU_H

// Inclure les en-têtes nécessaires
#include "XamGraph.h"  // Assurez-vous que c'est le bon chemin
#include "constantes.h"
#include "paquet.h"
#include "joueur.h"
#include "slot.h"

class Jeu : public XamGraph {
private:
    Joueur joueur1;
    Joueur joueur2;
    Slot slots[2];
    bool slamPossible;
    bool partieTerminee;

    void gererTouche(int touche);
    void verifierSlam();
    void executerSlam(bool estJoueur1);

public:
    // Constructeur avec paramètres
    Jeu(int largeur, int hauteur, const std::string& titre);

    void demarrer();
    void dessiner();
};

#endif // JEU_H