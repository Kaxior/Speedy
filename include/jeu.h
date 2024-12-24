#ifndef JEU_H
#define JEU_H

#include "joueur.h"
#include "carte.h"
#include <vector>
#include <memory>

class Jeu {
public:
    Jeu();
    void lancer(); // Lance le jeu
private:
    std::vector<Joueur> joueurs;
    std::vector<Carte> pioche;
    void initialiser();
    void afficherInstructions() const;
    void bouclePrincipale();
};

#endif
