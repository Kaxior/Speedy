#ifndef GAME_H
#define GAME_H

#include <vector>
#include "player.h"
#include "card.h"

class Game {
public:
    Player player1;
    Player player2;
    std::vector<Card> centralPiles[2];

    Game() : player1(std::vector<Card>()), player2(std::vector<Card>()) {}

    void startGame();
    void playCard(Player& player, int pickIndex, int pileIndex);
    void autoDraw();
    bool canPlayCard(const Card& topCard, const Card& pickCard);
};

#endif // GAME_H 