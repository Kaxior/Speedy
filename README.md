
Speedy 


1. Concept général :

Speedy est un jeu de rapidité et de réflexes qui se joue à deux joueurs sur un seul clavier.
Chaque joueur a un stock de cartes et 4 cartes visibles (appelées picks).
Le but est d’être le premier à se débarrasser de toutes ses cartes ou de réussir trois 'slam'.
Le jeu se joue en temps réel, pas en tour par tour.



2. Règles principales : 

Pose des cartes :
Les picks peuvent être posées sur l’un des deux zones centraux nommés "les slots" (zone commune entre les deux joueurs).
Les cartes suivent un ordre cyclique (As, 2,..., Roi, As, etc.), avec une rotation dans les deux sens.
- Pour poser une carte pick sur un slot, elle doit être adjacente (précédente ou suivante dans l'ordre) à la dernière carte visible sur le slot, créant ainsi une pile.
Les slots sont les zones où les joueurs peuvent poser les cartes qu'ils possèdent en passant uniquement par leurs 4 cartes visibles (picks) et non directement depuis leur stock.
Dans le jeu 'Speedy', la couleur et les formes des cartes n'ont pas d'importance.

Le Slam :
Le slam se trouve au centre, entre les deux slots.
Si les deux slots contiennent des cartes de même valeur, une image s'affiche : c'est le slam alert.
Le premier joueur à appuyer sur sa touche slam inflige une pénalité à l'autre joueur.
Le joueur pénalisé récupère toutes les cartes des deux slots dans son stock, mélangées.

Conditions de victoire :
Condition 1 : Un joueur réussit 3 slams.
Condition 2 : Un joueur n’a plus aucune carte à poser (ni dans son stock, ni parmi ses picks).


3. Fonctionnement du piochage :

Lorsqu'un joueur pose une carte pick, une nouvelle carte est automatiquement piochée de son stock pour remplacer le pick utilisé.

Lors du début de partie, après un slam, ou en cas de blocage, un piochage relance la partie avec une carte du stock de chaque joueur et pose chaque carte sur un slot (ou deux cartes du même joueur si l'autre n’a plus de stock).


4. Configuration des touches :

Joueur 1 :
Sélectionner une carte pick : J, K, L, M (de gauche à droite).
Poser la carte : I (pour le slot gauche), O (pour le slot droit).
Slam : N.

Joueur 2 :
Sélectionner les picks : Q, S, D, F (de gauche à droite).
Poser la carte : Z (pour le slot gauche), E (pour le slot droit).
Slam : W.

Mécanique de sélection :
Une touche parmi les 4 attribuées à chaque joueur sélectionne un pick (visualisé par un cadre autour de la carte choisie).
La carte sélectionnée peut être changée en appuyant sur une autre touche de selection de carte pick.
Une fois la carte choisie, le joueur utilise une touche pour indiquer le slot (gauche ou droit) sur lequel poser la carte.


5. Aspects techniques :

Langage et organisation :
Le jeu est programmé en C++.

Speedy/
├── assets/                # Contient les fichiers graphiques (cartes, icônes, etc.)
├── xamgraph/              # Contient les fichiers de la bibliothèque XamGraph
├── include/               # Contient tous les fichiers d'en-tête (.h)
│   ├── game.h             # Déclarations des constantes et fonctions principales
│   ├── player.h           # Déclarations des fonctions et structures liées aux joueurs
│   ├── card.h             # Gestion des cartes
│   └── utils.h            # Fonctions utilitaires (e.g., tirages aléatoires)
├── src/                   # Contient tous les fichiers source (.cpp)
│   ├── main.cpp           # Point d'entrée principal
│   ├── game.cpp           # Implémentation des fonctions principales
│   ├── player.cpp         # Implémentation des fonctions liées aux joueurs
│   ├── card.cpp           # Implémentation des fonctions de gestion des cartes
│   └── utils.cpp          # Implémentation des fonctions utilitaires
├── Makefile               # Fichier de compilation
└── README.md              # Documentation du projet

L'utilisateur utilise un poste tournant sous Windows 10 avec MSYS2 MINGW64 shell comme terminal, GCC pour la compilation, et Visual Studio Code comme environnement de travail. Toute réécriture de code pour le jeu *Speedy* doit prendre en compte cet environnement.

La conception du jeu se fera avec l'utilisation d'une bibliothèque graphique 2D basée sur SDL appelée XamGraph.

L'utilisateur fournit trois fichiers essentiels pour le développement du jeu *Speedy* : 
1. **/assets** : Contient les images nécessaires pour le jeu (cartes, fond vert, slam alert, etc.). 
2. **/XamGraph** : Mini-bibliothèque graphique 2D basée sur SDL2, à vocation pédagogique. Il est impératif de lire et comprendre le dossier `XamGraph\doc` pour maîtriser son fonctionnement et ses fonctions disponibles. 
3. **Makefile** : Gère la compilation du projet via la commande `make`. L'utilisateur y ajoutera les noms des fichiers créés après la conception. Ces fichiers serviront de base pour le développement.


6. Chronologie des événements :

Début de la partie :
Déroulement du jeu 'Speedy' au début :
- Le pack de 52 cartes est mélangé et distribué de dos, 26 cartes à chaque joueur.
- Chaque joueur dispose de 26 cartes cachées, dont 4 sont tirées et posées face visible devant eux.
- Chaque joueur se retrouve avec un 'stock' de 21 cartes cachées et 4 cartes visibles (appelées 'picks') posées côte-à-côte devant eux, tirées de leur stock.
- Le système lance un 'piochage' avec un décompte de trois secondes, puis pioche une seule carte de chaque stock (une par joueur) et les pose sur chaque slot, face visible. Ce 'piochage' pourra être réutilisé dans différents scénarios où le jeu est bloqué.

Pendant la partie :
Les joueurs jouent simultanément pour poser leurs picks sur les slots selon les règles.
Si un slam est déclenché, les cartes des slots sont attribuées au joueur pénalisé.
Si les joueurs sont bloqués, un piochage relance les slots.

Fin de partie :
Un joueur gagne s'il réussit 3 slams ou s'il n’a plus de cartes à jouer.


7.Solutions aux scénarios de bugs :

1. Si les deux joueurs sont bloqués et ne peuvent poser de pick en respectant l’ordre des cartes, le système peut relancer le jeu avec un piochage.
2. Un système de détection précis en millisecondes attribuera le slam au bon joueur, évitant les conflits de clics simultanés.
3. Si, lors du piochage, un joueur a un stock vide, le système prendra une carte supplémentaire du stock de l'autre joueur pour poser une carte sur chaque slot.
4. Contrôle automatique pour vérifier qu'une carte posée est bien adjacente à la dernière carte du slot. Si un coup non conforme est tenté, il sera rejeté et entraînera une pénalité d'une seconde.
5. Lorsqu'une carte pick est posée, une nouvelle carte la remplace après un délai de 0,3 seconde.
6. Le slam est une option : les joueurs peuvent choisir de réaliser un slam lorsqu’il est possible, ou continuer à poser leurs picks sans effectuer de slam.

Cas de blocage :
Si aucun joueur ne peut poser de pick (aucune carte valide sur les slots), le jeu relance automatiquement les slots en piochant une nouvelle carte dans le stock de chaque joueur (ou deux cartes d'un seul joueur si l’autre n’a plus de stock).


8. Graphismes :

Écran de jeu du jeu 'Speedy' (vue de dessus en 2D) :
1. Joueur 1 à gauche, Joueur 2 à droite.
2. Disposition Joueur 1 (du bord gauche jusqu'au centre de la fenetre) :
   - Stock de cartes avec un compteur au-dessus (indiquant la quantité).
   - 4 picks côte à côte à droite du stock.
   - Picks encadrés en rouge s'ils sont sélectionnés.
3. Disposition Joueur 2 : Symétrique à celle du Joueur 1, à droite.
4. Au centre :
   - Deux slots côte à côte.
   - Entre les slots, une image apparaît lorsque un slam est possible. Le *slam alert* reste au même endroit.
5. Esthétique : Simple, fond vert avec cartes classiques.
Tout est disponible dans le dossier assets.
