#include "utils.h"
#include <algorithm>
#include <random>

std::string getRandomCardValue() {
    // Retourne une valeur aléatoire
    return "As";
}

void shuffleDeck(std::vector<Card>& deck) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
}
 