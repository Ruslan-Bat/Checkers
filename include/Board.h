#pragma once
#include "Graph_lib/Graph.h"
#include "Piece.h"
#include <vector>
#include <memory>
#include <iostream>



// Класс, отвечающий за представление и отрисовку доски
class Board {
public:
    static constexpr int SIZE = 8;  // Размер доски
    static constexpr int CELL_SIZE = 80; // Размер клетки

    Board(int cell_size = CELL_SIZE);

    int getCellSize() const { return cell_size; }

    bool hasPiece(int x, int y) const;

    bool movePiece(int fromX, int fromY, int toX, int toY);

    int cellAt(int x, int y) const;

    void removePiece(int x, int y);
    
    int getCell(int x, int y) const;

    void setPiece(int x, int y, int piece);
    
private:
    int cell_size;  // Размер одной клетки в пикселях
    std::vector<int> cells;  // Матрица фигур
};