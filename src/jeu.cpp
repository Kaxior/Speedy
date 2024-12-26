#include "jeu.h"
#include "XamGraph.h"  // Assurez-vous que c'est le bon chemin

// Constructeur modifié
Jeu::Jeu(int largeur, int hauteur, const std::string& titre) 
    : XamGraph(largeur, hauteur, titre),
      slamPossible(false), 
      partieTerminee(false) {
   
   // Utiliser les méthodes de XamGraph
   setDrawingArea(DRAWING_BITMAP);  // Ajustez selon la définition réelle
   
   // Gestionnaire d'événements
   connectKeyDownEvent([this](int key, int mod) { 
       gererTouche(key);
   });
   
   // Distribution initiale
   Paquet paquetComplet;
   paquetComplet.melanger();
   
   // 26 cartes par joueur
   for(int i = 0; i < 26; i++) {
       if(i < 4) { // 4 premiers picks visibles
           Carte c = paquetComplet.tirerCarte();
           c.setVisible(true);
           
           if(i < 2) {
               joueur1.ajouterCarte(c);  // Méthode générique
           } else {
               joueur2.ajouterCarte(c);
           }
       } else { // Le reste dans le stock
           joueur1.ajouterCarteStock(paquetComplet.tirerCarte());
           joueur2.ajouterCarteStock(paquetComplet.tirerCarte());
       }
   }
}

void Jeu::gererTouche(int touche) {
   // Implémentation similaire à votre version précédente
   // Utilisez les méthodes disponibles de vos classes Joueur
}

void Jeu::verifierSlam() {
   // Logique de vérification de slam
}

void Jeu::dessiner() {
   clearScreen(COLOR_GREEN);  // Ajustez selon les définitions de votre bibliothèque
   
   // Dessiner joueurs
   joueur1.dessinerStock(50, HAUTEUR_FENETRE/2);
   joueur1.dessinerPicks(150, HAUTEUR_FENETRE/2);
   
   joueur2.dessinerStock(LARGEUR_FENETRE-150, HAUTEUR_FENETRE/2);
   joueur2.dessinerPicks(LARGEUR_FENETRE-450, HAUTEUR_FENETRE/2);
   
   // Dessiner slots
   slots[0].dessiner(LARGEUR_FENETRE/2-110, HAUTEUR_FENETRE/2);
   slots[1].dessiner(LARGEUR_FENETRE/2+10, HAUTEUR_FENETRE/2);
   
   // Si slam possible, afficher l'alerte
   if(slamPossible) {
       drawImage(LARGEUR_FENETRE/2-50, HAUTEUR_FENETRE/2-50, "assets/ui/slam.png");
   }
   
   updateScreen();
}

void Jeu::demarrer() {
   while(!partieTerminee) {
       dessiner();
       verifierSlam();
       wait(16);  // Attente de 16 ms
   }
}