#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "card.h"

class Player {
public:
    std::vector<Card> stock;
    std::vector<Card> picks;

    Player(std::vector<Card> stock) : stock(stock) {
        for (int i = 0; i < 4; ++i) {
            picks.push_back(stock.back());
            stock.pop_back();
        }
    }

    void replacePick(int index) {
        if (!stock.empty()) {
            picks[index] = stock.back();
            stock.pop_back();
        }
    }
}; 

#endif // PLAYER_H