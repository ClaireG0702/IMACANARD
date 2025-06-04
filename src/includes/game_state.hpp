#pragma once

enum class GameStatus {
    WIN,
    LOSE
};
extern GameStatus gameStatus;

extern int width = 6;
extern float cellSize = 1.0f / width;
extern float characterSize = 1.0f / (width * 2);