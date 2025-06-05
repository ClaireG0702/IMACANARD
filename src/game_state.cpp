#include "includes/game_state.hpp"

Page currentPage = Page::MAIN_MENU;
GameStatus gameStatus = GameStatus::WIN;

int width = 20;
float cellSize = 1.0f / width;
float characterSize = 1.0f / (width * 2);

int numberOfEnemies = 2;
int numberOfObjects = width / 2;

int probObjectSpawn = 30;
int probTrap = 30;
int probMinableBloc = 5;