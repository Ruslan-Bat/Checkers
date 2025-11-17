#pragma once
#include "Board.h"
#include "Piece.h"

class Game {
public:
    Game(Board& board);

    // Обработка клика по клетке
    void handleClick(int cellX, int cellY);

    Board& getBoard() const { return board; }

private:
    Board& board;
    bool pieceSelected=false;    // выбрана ли шашка
    int selectedX, selectedY;

    bool isValidMove(int fromX, int fromY, int toX, int toY) const;
    bool whiteTurn = true;
};
