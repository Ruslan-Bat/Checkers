#include "Board.h"
#include "Piece.h"
#include "Graph_lib/Graph.h"
#include <memory>

using namespace Graph_lib;

Board::Board(int cell_size)
    : cell_size(cell_size)
{
    // Инициализация сетки 8x8
    grid.resize(SIZE);
    for (int y = 0; y < SIZE; ++y) {
        grid[y].resize(SIZE);
    }

    // Расстановка чёрных шашек (ряды 0-2)
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            if ((x + y) % 2 != 0) { // тёмные клетки
                grid[y][x] = std::make_unique<Piece>(PieceColor::WHITE,
                                                     Point{x * cell_size, y * cell_size},
                                                     cell_size);
            }
        }
    }

    // Расстановка белых шашек (ряды 5-7)
    for (int y = 5; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            if ((x + y) % 2 != 0) { // тёмные клетки
                grid[y][x] = std::make_unique<Piece>(PieceColor::BLACK,
                                                     Point{x * cell_size, y * cell_size},
                                                     cell_size);
            }
        }
    }
}

void Board::draw(Window& win) {
    draw_cells(win);

    // Рисуем все фигуры
    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            if (grid[y][x]) {
                grid[y][x]->draw(win);
            }
        }
    }
    
}

void Board::draw_cells(Window& win) {
    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            Color color = ((x + y) % 2 == 0) ? Color::white : Color::dark_green;
            Rectangle *r = new Rectangle{Point{x * cell_size, y * cell_size}, cell_size, cell_size};
            r->set_fill_color(color);
            win.attach(*r);
        }
    }
}

bool Board::hasPiece(int cellX, int cellY) const {
    // Проверяем, что координаты клетки в пределах доски
    if (cellX < 0 || cellX >= SIZE || cellY < 0 || cellY >= SIZE)
        return false;

    // Возвращаем true, если в клетке есть указатель на Piece
    return grid[cellY][cellX] != nullptr;
}