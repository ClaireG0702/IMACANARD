#pragma once

enum class GameStatus {
    WIN,
    LOSE
};
extern GameStatus gameStatus;

extern int width;
extern float cellSize;
extern float characterSize;

extern int numberOfEnemies;