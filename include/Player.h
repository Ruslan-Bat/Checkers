#pragma once
#include <vector>
#include "Board.h"

class Player {
public:
    // Статический метод для получения хода от AI.
    // Не требует создания объекта Player.
    // Возвращает: {fromX, fromY, toX, toY, capturedX, capturedY, ...}
    static std::vector<int> getAIMove(const Board& board);
};
