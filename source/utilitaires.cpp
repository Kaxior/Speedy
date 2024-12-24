
#include "utilitaires.h"

void melangerPioche(std::vector<Carte>& pioche) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(pioche.begin(), pioche.end(), g);
}
