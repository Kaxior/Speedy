#include "../include/game.h"
#include <algorithm>
#include <random>

void Game::startGame() {
    // Mélanger et distribuer les cartes
    std::vector<Card> deck;
    std::string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    std::string values[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

    for (const auto& suit : suits) {
        for (const auto& value : values) {
            deck.emplace_back(suit, value);
        }
    }

    std::shuffle(deck.begin(), deck.end(), std::mt19937{std::random_device{}()});

    std::vector<Card> stock1(deck.begin(), deck.begin() + 26);
    std::vector<Card> stock2(deck.begin() + 26, deck.end());

    player1 = Player(stock1); 
    player2 = Player(stock2);

    // Initialiser les piles centrales avec une carte de chaque joueur
    centralPiles[0].push_back(player1.stock.back());
    player1.stock.pop_back();
    centralPiles[1].push_back(player2.stock.back());
    player2.stock.pop_back();
}

void Game::playCard(Player& player, int pickIndex, int pileIndex) {
    if (!player.picks.empty() && !centralPiles[pileIndex].empty()) {
        Card topCard = centralPiles[pileIndex].back();
        Card pickCard = player.picks[pickIndex];

        // Vérifier si la carte peut être jouée
        if (canPlayCard(topCard, pickCard)) {
            centralPiles[pileIndex].push_back(pickCard);
            player.replacePick(pickIndex);
        }
    }
}

bool Game::canPlayCard(const Card& topCard, const Card& pickCard) {
    // Vérifier si la carte peut être jouée (par exemple, une carte de rang immédiatement supérieur ou inférieur)
    // Implémenter la logique de vérification ici
    return true; // Remplacer par la condition réelle
}

void Game::autoDraw() {
    // Logique pour piocher automatiquement des cartes
    if (/* condition pour piocher automatiquement */) {
        // Piocher une carte pour chaque joueur et la placer sur les piles centrales
        if (!player1.stock.empty()) {
            centralPiles[0].push_back(player1.stock.back());
            player1.stock.pop_back();
        }
        if (!player2.stock.empty()) {
            centralPiles[1].push_back(player2.stock.back());
            player2.stock.pop_back();
        }
    }
}