#pragma once
#include "Graph_lib/Graph.h"
#include "Piece.h"
#include <vector>
#include <memory>
#include <iostream>

// Класс, отвечающий за представление и отрисовку доски
class Board {
public:
    static constexpr int SIZE = 8;  // Размер доски 8x8
    static constexpr int CELL_SIZE = 80; // Размер клетки

    int getCellSize() const { return cell_size; }

    Board(int cell_size = CELL_SIZE);  // Конструктор: принимает размер одной клетки

    // Отрисовать доску в окне
    void draw(Graph_lib::Window& win);

    bool hasPiece(int cellX, int cellY) const;

    bool movePiece(int fromX, int fromY, int toX, int toY);

    Piece* pieceAt(int x, int y) const;
    void removePiece(int x, int y);

private:
    int cell_size;  // Размер одной клетки (в пикселях)
    std::vector<std::vector<std::unique_ptr<Piece>>> grid;  // Матрица фигур

    void draw_cells(Graph_lib::Window& win);  // Внутренний метод отрисовки клеток

};