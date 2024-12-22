#include "./XamGraph/src/xamgraph.h"
#include "../include/game.h"
#include <iostream>

void handleKeyPress(Game& game, char key) {
    // Logique pour gérer les touches du clavier
    // Exemple : '1' pour jouer la première carte du joueur 1 sur la première pile centrale
    if (key == '1') {
        game.playCard(game.player1, 0, 0);
    } else if (key == '2') {
        game.playCard(game.player1, 1, 0);
    } else if (key == '3') {
        game.playCard(game.player1, 2, 0);
    } else if (key == '4') {
        game.playCard(game.player1, 3, 0);
    } else if (key == 'q') {
        game.playCard(game.player2, 0, 1);
    } else if (key == 'w') {
        game.playCard(game.player2, 1, 1);
    } else if (key == 'e') {
        game.playCard(game.player2, 2, 1); 
    } else if (key == 'r') {
        game.playCard(game.player2, 3, 1); 
    }
}

int main() {
    // Initialiser XamGraph
    XamGraphInit(1020, 780, "Speedy");

    // Initialiser le jeu
    Game game;
    game.startGame();

    // Boucle principale du jeu
    while (true) {
        // Gérer les événements clavier
        if (XamGraphKbhit()) {
            char key = XamGraphGetch();
            handleKeyPress(game, key);
        }

        // Mettre à jour l'affichage
        XamGraphClear();
        // Afficher les cartes et les piles centrales
        XamGraphUpdate();
    }

    // Fermer XamGraph
    XamGraphClose();
    return 0;
}