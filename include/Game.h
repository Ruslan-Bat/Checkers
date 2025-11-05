#pragma once
#include "Board.h"

class Game {
public:
    Game(Board& board);

    // Обработка клика по клетке
    void handleClick(int cellX, int cellY);

private:
    Board& board;          // ссылка на доску (используем существующий объект)
    bool pieceSelected;    // выбрана ли шашка
    int selectedX, selectedY;

    bool isValidMove(int fromX, int fromY, int toX, int toY) const;
};
