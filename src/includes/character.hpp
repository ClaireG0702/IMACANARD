#pragma once

// ce qui y a en commun avec enemy et player
// déplacement ?
// ne pas passer à travers les blocs pleins
// position de départ
// action avec les blocs en bonus

/*Gestion des collisions
- je suis dans une tuile si au moins des 4 corners est lui aussi dans la grille
- pour vérifier : prendre flottant --> arrondir au précédent (avec un round ou un float)

je calcule position possible + est ce que dans les 4 tuiles possibles y en a 1 qui est pleine ?
    - si oui : j'avance
    - si non : je bouge pas ou action
*/