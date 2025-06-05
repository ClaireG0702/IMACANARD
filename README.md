# 🦆 IMACANARD

IMACANARD est un jeu créé dans le cadre des cours *Programmation et algorithmique C++* et *Synthèse d'images I*.

Il a été implémenté par Emilie DESOMBRE, Océane DRAPEAU et Claire GOBERT. Il fonctionne sur Windows et Linux.

## 🎲 Règles du jeu 

IMACANARD se joue au clavier. Le joueur se déplace à l'aide des flèches du clavier <kbd>↑</kbd> <kbd>↓</kbd> <kbd>←</kbd> <kbd>→</kbd>.

Il mine un bloc plein (nénuphar) avec <kbd>a</kbd>.

Il peut quitter le jeu à tout moment avec <kbd>q</kbd>.

## 🎨 Thème du jeu

Le joueur incarne un canard qui cherche à échapper à des loutres. Il se déplace dans une étendue d'eau et peut détruire des nénuphars. Lorsqu'il détruit des nénuphars, il peut collecter des ombres. Cependant, il ne peut pas aller sur les rochers. De plus, s'il prend un tourbillon, il perd la partie.

## 🧐 Spécifications

### Joueur
Le joueur possède une seule vie et perd si l'ennemi arrive sur sa case ou s'il tombe dans un piège. Il est représenté par un canard jaune. Lorsqu'il mine, il y a un pourcentage de chances pour obtenir un objet.

### Ennemis
Les ennemis cherchent à atteindre le joueur. Ils sont représentés par des loutres.

### Les blocs
Nous avons utilisé la répartition des cases suivantes : 
- **Bloc vide (0):** représenté par un bloc bleu (sans textures) et un bloc d'eau (avec textures). Le joueur peut aller dessus.

- **Obstacle (non minable) (1) :** représenté par un bloc vert très foncé (sans textures) et un rocher sur un bloc d'eau (avec textures). Le joueur ne peut jamais aller dessus.

- **Bloc plein (minable) (2):** représenté par un bloc vert (sans textures) et des fleurs sur un bloc d'eau (avec textures). Le joueur ne peut pas aller dessus à moins de détruire le bloc.

- **Objet (4):** représenté par un bleu très foncé (sans textures) et une ombre (avec textures). Le joueur peut collecter des objets.

- **Piège (5):** représenté par un bloc bleu saturé (sans textures) et un bloc d'eau et un tourbillon (avec textures). Le joueur peut aller dessus mais c'est la fin de la partie.

- **Accélérateur (3):** représenté par un bloc jaune-vert (sans textures) et des fleurs. Il donne au joueur un court boost de vitesse.

- **Ralentisseur (6):** représenté par un bloc violet (sans textures) et des roseaux. Il diminue temporairement la vitesse du joueur.

Ces cases sont implémentés à l'aide d'une struct constituée de positions et d'une valeur correspondant au type de bloc. 

### Génération de la carte
La carte est généré avec l'algorithme de génération procédurale cellular automata. Dans un premier temps, on y génère des cases vides ou pleines (0 ou 1) avec 50% de chances chacune. Dans un second temps, on regarde les cases adjacentes individuellement, et on y ajuste leur caractère plein ou vide en fonction des voisins.

Puis on y ajoute des objets, des cases pièges, des obstacles, et des modificateurs de vitesse.

Le joueur et les ennemis sont ajoutés après la carte.

### Flow field pathfinding
La direction des ennemis est définie grace à l'algorithme de recherche Breadth-First Search.

### Interface graphique
Pour réaliser le menu et l'écran de fin de jeu, nous avons utilisé la librairie imgui.
Le jeu contient donc un menu via lequel le joueur peut accéder à une page de règles et une page de paramètres. Il peut s'il le souhaite changer la taille de la grille de jeu et le nombre d'ennemis présents.

## Post mortem

### Ce qui a bien fonctionné

L'algorithme de cellular automata a été assez simple à implémenter, notamment par son explication détaillée dans le sujet du projet.

La cohésion au sein du groupe a permis également de communiquer nos problèmes entre nous et de vérifier les codes des unes et des autres pour les régler.

### Problèmes rencontrés

### Ennemis

La fonction de déplacement des ennemis n'est pas optimisée suffisament, une meilleure gestion dans l'espace leur permettraient de suivre le joueur en permanence. Actuellement, il arrive que les ennemis se bloquent aux coins des murs.

#### Compilation

Il y a eu plusieurs problèmes de compilation : d'abord quand on a ajouté les textures, une de nous avait la fenêtre OpenGL qui n'arrivait pas à charger. Ce qu'on croyait d'abord être dû à un manque de capacité de l'ordinateur était en fait causé par l'utilisation de Clang au lieu de GCC.

#### Affichage de la carte
L'affichage de base de la map a été un problème. Nous avons cherché à recréer nous même les objets mais nous nous sommes trompés de GL_TRIANGLE_FAN et ça nous a renvoyé quelques erreurs ![image](./doc/screenshots/screendisplay.png)

Puis ce fut au tour des buffers. En rentrant un mauvais nombre de coordonnées, notre carte s'est déformée.

![image](/doc/screenshots/screendisplay2.png)

#### Les textures
L'affichage des textures a été très compliqué. Nous voulions les initialiser, puis les utiliser en les stockant dans une liste. Cependant, les textures, de par leur composition "s'auto détruisaient", lorsqu'on cherchait à y accéder, elles ne s'affichaient pas. De plus, aucun message d'erreur n'était renvoyé, ne permettant pas de comprendre ce qui n'allait pas.

Après des jours d'expérimentation, nous avons fini par demander de l'aide à Enguerrand De Smet. La solution a finalement été de rajouter des lignes manquantes à un fichier de la librairie. 

#### Les sprites

L'affichage des sprites était également compliqué. En effet, il fallait normaliser les images qui par défaut n'étaient pas dans le même repère que les textures. Par ailleurs, le lien entre le fait que les coordonnées de la texture dans le buffer correspondait aux coordonnées du sprite à utiliser a été compliqué à comprendre. C'est par hasard que nous avons fait la correspondance, lorsque nous avons utilisé le mauvais de tableau de coordonnées dans le buffer.

![image](/doc/screenshots/screendisplay6.png)

Par ailleurs, les images étaient à l'envers. Nous avons ajouté ```cpp stbi_set_flip_vertically_on_load(true);```
pour retourner les images.

### Avec plus de temps

Avec plus de temps, nous aurions aimé approfondir un peu plus le jeu et le rendre plus efficace. 

En synthèse d'images, nous aurions voulu rajouter des animations et rectifier l'affichage des sprites qui est pour le moment positionné à la main.

En algorithmique, nous aurions voulu améliorer la génération de la carte et, en général, reorganiser notre code pour être plus efficace, notamment en utilisant plus de références.