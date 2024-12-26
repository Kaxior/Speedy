#ifndef CONSTANTES_H
#define CONSTANTES_H

// LARGEUR_FENETRE et HAUTEUR_FENETRE définissent la résolution par défaut de la fenêtre de jeu.
const int LARGEUR_FENETRE = 1024;
const int HAUTEUR_FENETRE = 768;

// Dimensions cartes
const int LARGEUR_CARTE = 100;
const int HAUTEUR_CARTE = 140;

// Touches joueur 1
const int TOUCHES_J1[] = {'j', 'k', 'l', 'm'};  // Sélection picks
const int POSE_GAUCHE_J1 = 'i';    // Poser sur slot gauche
const int POSE_DROITE_J1 = 'o';    // Poser sur slot droit
const int SLAM_J1 = 'n';           // Touche slam

// Touches joueur 2
const int TOUCHES_J2[] = {'q', 's', 'd', 'f'};  // Sélection picks
const int POSE_GAUCHE_J2 = 'z';    // Poser sur slot gauche
const int POSE_DROITE_J2 = 'e';    // Poser sur slot droit
const int SLAM_J2 = 'w';           // Touche slam

#endif