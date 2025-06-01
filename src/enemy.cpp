#include "includes/enemy.hpp"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <queue>
#include <climits>

void initEnemies(std::vector<Enemy>& enemies, std::vector<Cell> &map) {
    for(Enemy& enemy: enemies) {
        Cell* cell = nullptr;
        while(!cell) {
            int index = std::rand() % map.size();
            if(map[index].value == 0) {
                cell = &map[index];
            }
        }

        enemy.position = cell->positions;
        enemy.gridPos = cell->positions;
        enemy.direction = Direction::NONE;
    }
}

std::vector<Cell> createValuedMap(std::vector<Cell> &map, Player &player) {
    std::vector<Cell> valuedMap = map; // Copie de la carte pour y stocker les coûts

    int maxX = map.back().positions.x;
    int maxY = map.back().positions.y;

    // Initialiser les coûts avec une valeur infinie
    for (auto &cell : valuedMap) {
        cell.value = INT_MAX; // Valeur infinie par défaut
    }

    // Initialiser la position du joueur avec un coût de 0
    glm::vec2 playerPos = player.gridPos;
    auto it = std::find_if(valuedMap.begin(), valuedMap.end(), [&playerPos](const Cell &cell) {
        return cell.positions == playerPos;
    });
    if (it != valuedMap.end()) {
        it->value = 0;
    }

    // File pour la propagation des coûts
    std::queue<Cell> queue;
    queue.push({playerPos, 0}); // Ajouter la position du joueur avec un coût de 0

    // Propagation des coûts
    while (!queue.empty()) {
        Cell currentCell = queue.front();
        queue.pop();

        // Vérifier les voisins (UP, RIGHT, DOWN, LEFT)
        for (int i = 0; i < 4; i++) {
            int x = currentCell.positions.x;
            int y = currentCell.positions.y;

            switch (i) {
                case 0: y++; break; // UP
                case 1: x++; break; // RIGHT
                case 2: y--; break; // DOWN
                case 3: x--; break; // LEFT
                default: break;
            }

            // Vérifier si la position voisine est valide
            if (x >= 0 && x <= maxX && y >= 0 && y <= maxY) {
                // Vérifier si la cellule voisine est un obstacle (valeur 1 dans la carte d'origine)
                auto mapIt = std::find_if(map.begin(), map.end(), [x, y](const Cell &cell) {
                    return cell.positions.x == x && cell.positions.y == y;
                });
                if(mapIt != map.end() && mapIt->value == 1) {
                    continue; // Ignorer les cellules avec des obstacles
                }

                // Trouver la cellule correspondante dans valuedMap
                auto neighborIt = std::find_if(valuedMap.begin(), valuedMap.end(), [x, y](const Cell &cell) {
                    return cell.positions.x == x && cell.positions.y == y;
                });

                if (neighborIt != valuedMap.end()) {
                    unsigned int newValue = currentCell.value + 1;

                    // Mettre à jour si un coût plus faible est trouvé
                    if (newValue < neighborIt->value) {
                        neighborIt->value = newValue;
                        queue.push({{x, y}, newValue});
                    }
                }
            }
        }
    }

    return valuedMap;
}

std::vector<CellDirection> createDirectedMap(std::vector<Cell> &map) {
    std::vector<CellDirection> directedMap;

    for (const auto &cell : map) {
        CellDirection currentCell;
        currentCell.positions = cell.positions;

        int x = cell.positions.x;
        int y = cell.positions.y;

        // Si la cellule a une valeur de 0, attribuer la direction NONE
        if (cell.value == 0 || cell.value == INT_MAX) {
            currentCell.direction = Direction::NONE;
            directedMap.push_back(currentCell);
            continue; // Passer à la cellule suivante
        }

        // Initialiser la direction par défaut
        Direction bestDirection = Direction::NONE;
        int minValue = INT_MAX;

        // Vérifier les voisins (UP, RIGHT, DOWN, LEFT)
        for (int i = 0; i < 4; i++) {
            int neighborX = x;
            int neighborY = y;

            switch (i) {
                case 0: neighborY++; break; // UP
                case 1: neighborX++; break; // RIGHT
                case 2: neighborY--; break; // DOWN
                case 3: neighborX--; break; // LEFT
                default: break;
            }

            // Trouver la cellule voisine
            auto neighborIt = std::find_if(map.begin(), map.end(), [neighborX, neighborY](const Cell &cell) {
                return cell.positions.x == neighborX && cell.positions.y == neighborY;
            });

            // Si la cellule voisine existe et a une valeur plus petite
            if (neighborIt != map.end() && neighborIt->value < minValue) {
                minValue = neighborIt->value;

                // Mettre à jour la meilleure direction
                switch (i) {
                    case 0: bestDirection = Direction::UP; break;
                    case 1: bestDirection = Direction::RIGHT; break;
                    case 2: bestDirection = Direction::DOWN; break;
                    case 3: bestDirection = Direction::LEFT; break;
                }
            }
        }

        // Attribuer la meilleure direction à la cellule actuelle
        currentCell.direction = bestDirection;

        // Ajouter la cellule avec sa direction au tableau final
        directedMap.push_back(currentCell);
    }

    return directedMap;
}

void updateEnemies(std::vector<Enemy>& enemies, const std::vector<CellDirection>& directedMap, float deltaTime) {
    for (Enemy& enemy : enemies) {
        // Trouver la direction associée à la position de l'ennemi
        auto it = std::find_if(directedMap.begin(), directedMap.end(), [&enemy](const CellDirection& cellDir) {
            return cellDir.positions == enemy.gridPos;
        });

        if (it != directedMap.end()) {
            Direction dir = it->direction;
            glm::vec2 movement(0.f);

            switch (dir) {
                case Direction::UP:    movement.y = 1.f; break;
                case Direction::DOWN:  movement.y = -1.f; break;
                case Direction::LEFT:  movement.x = -1.f; break;
                case Direction::RIGHT: movement.x = 1.f; break;
                default:  break;
            }

            enemy.position += movement * enemy.speed * deltaTime;
            enemy.direction = dir;

            enemy.gridPos = glm::vec2(std::floor(enemy.position.x), std::floor(enemy.position.y));
        }
    }
}
