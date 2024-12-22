#ifndef CARD_H
#define CARD_H

#include <string>

class Card {
public:
    std::string suit;
    std::string value;

    Card(std::string suit, std::string value) : suit(suit), value(value) {}
};

#endif // CARD_H 