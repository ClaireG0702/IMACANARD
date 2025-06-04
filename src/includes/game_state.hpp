#pragma once

enum class Page {
    MAIN_MENU,
    SETTINGS,
    RULES,
    GAME,
    END_SCREEN
};
extern Page currentPage;

enum class GameStatus {
    WIN,
    LOSE
};
extern GameStatus gameStatus;

extern int width;
extern float cellSize;
extern float characterSize;

extern int numberOfEnemies;