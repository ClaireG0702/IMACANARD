# 🦆 IMACANARD

IMACANARD est un jeu créé dans le cadre des cours *Programmation et algorithmique C++* et *Synthèse d'images I*.

Il a été implémenté par Emilie DESOMBRE, Océane DRAPEAU et Claire GOBERT. Il fonctionne sur Windows et Linux.

## Règles du jeu

IMACANARD se joue au clavier. Le joueur se déplace à l'aide des flèches du clavier <kbd>↑</kbd> <kbd>↓</kbd> <kbd>←</kbd> <kbd>→</kbd>.

Il mine un bloc plein avec <kbd>a</kbd>.

Il peut quitter le jeu à tout moment avec <kbd>q</kbd>.

## Spécifications

### Joueur
Le joueur possède une seule vie et perd si l'ennemi arrive sur sa case ou s'il tombe dans un piège. Il est représenté par un canard. Lorsqu'il mine, il y a un pourcentage de chances pour obtenir un objet.

### Ennemis
Les ennemis cherchent à atteindre le joueur. Ils sont représentés par des loutres.

### Carte
Les cases vides où le joueur peut se déplacer sont représentées par de l'eau

Les cases pleines que le joueur peut détruire sont representées par des nénuphars.

L'objet à collecter est des ombres.

L'obstacle qui ne peut pas être détruit est un rocher. 

Le piège est un tourbillon dans l'eau.

Ces cases sont implémentés à l'aide d'une struct qui prend en paramètre un type de bloc, sa position.

### Génération de la carte
La carte est généré avec l'algorithme de génération procédurale cellular automata. Dans un premier temps, on y génère des cases vides ou pleines (0 ou 1) avec 50% de chances chacune. Dans un second temps, on regarde les cases adjacentes individuellement, et on y ajuste leur caractère plein ou vide en fonction des voisins.

Puis on y ajoute des objets, des cases pièges, et des obstacles.

Le joueur et les ennemis sont ajoutés après la carte.

### Flow field pathfinding
Nous utilisons l'algorithme de recherche de chemin suggéré. 

### Interface graphique
Pour réaliser le menu et l'écran de fin de jeu, nous avons utilisé la librairie imgui.


## Post mortem

### Ce qui a bien fonctionné

### Problèmes rencontrés

#### Affichage de la carte
L'affichage de base de la map a été un problème. Nous avons cherché à recréer nous même les objets mais nous nous sommes trompés de GL_TRIANGLE_FAN et ça nous a renvoyé quelques erreurs ![image](./doc/screenshots/screendisplay.png)

Puis ce fut au tour des buffers. En rentrant un mauvais nombre de coordonnées, notre carte s'est déformée.

![image](/doc/screenshots/screendisplay2.png)

#### Les textures
L'affichage des textures a été très compliqué. Nous voulions les initialiser, puis les utiliser en les stockant dans une liste. Cependant, les textures, de par leur composition "s'auto détruisaient", lorsqu'on cherchait à y accéder, elles ne s'affichaient pas. De plus, aucun message d'erreur n'était renvoyé, ne permettant pas de comprendre ce qui n'allait pas.

Après jours d'expérimentation, nous avons fini par demander de l'aide à Enguerrand De Smet. La solution a finalement été de rajouter des ligneslignes manquantes au fichier de texture. 

#### Les sprites

L'affichage des sprites était également compliqué. En effet, il fallait normaliser les images qui par défaut n'étaient pas dans le même repère que les textures. Par ailleurs, le lien entre le fait que les coordonnées de la texture dans le buffer correspondait aux coordonnées du sprite à utiliser a été compliqué à comprendre. C'est par hasard que nous avons fait la correspondance. Lorsque nous avons utilisé le mauvais de tableau de coordonnées dans le buffer.

![image](/doc/screenshots/screendisplay6.png)

Par ailleurs, les images étaient à l'envers. Nous avons ajouté ```cpp stbi_set_flip_vertically_on_load(true);```
pour retourner les images.

### Avec plus de temps

Avec plus de temps, nous aurions aimé approfondir un peu plus le jeu. Notamment, rajouter des animations ou améliorer la génération de la carte.

<!-- Ajoutez enfin une partie "Post mortem" pour analyser le travail fourni, qu'est ce qui a bien fonctionné, quels ont été les problèmes rencontrés, comment vous les avez surmontés, auriez-vous fait différemment ? Avec plus de temps, qu'est ce que vous pourriez ajouter ? -->


<!-- pour mettre des captures d'écran : créer dossier screenshots puis ![image](./screenshots/pouet.png)-->
 
<!-- Éventuellement, si vous souhaitez mettre en avant un bout de code pour sa performance ou parce qu’il s’agit d’une idée intéressante, vous pouvez l'intégrer dans le rapport (mais rester succinct). -->