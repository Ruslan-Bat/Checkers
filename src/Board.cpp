#include "Board.h"
#include "Piece.h"
#include "Graph_lib/Graph.h"
#include <memory>
#include <iostream>

using namespace Graph_lib;

Board::Board(int cell_size)
    : cell_size(cell_size)
{
    // Инициализация сетки 8x8
    cells.resize(SIZE * SIZE, 0);
    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x<SIZE; ++x){
            if ((x+y) % 2 !=0) {
                if (y < 3) {
                    cells[y * SIZE + x] = Piece::BLACK_MAN;
                } else if (y > 4) {
                    cells[y *SIZE + x] = Piece::WHITE_MAN;
                } else {
                    cells[y *SIZE + x] = Piece::PLAYABLE_EMPTY;
                }
            } else {
                cells[y * SIZE + x] = Piece::EMPTY;
            }
        }
    }
}

bool Board::hasPiece(int x, int y) const {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) return false;
    int val = cells[y * SIZE + x];
    return Piece::isPiece(val);
}


bool Board::movePiece(int fromX, int fromY, int toX, int toY) {
    if (!hasPiece(fromX, fromY)) return false;   // нет фишки
    if (hasPiece(toX, toY)) return false;        // место занято

    std::swap(cells[fromY * SIZE + fromX], cells[toY * SIZE + toX]);
    return true;
}

int Board::cellAt(int x, int y) const {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) return 0;
    return cells[y * SIZE + x];
}

void Board::removePiece(int x, int y) {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) return;
    int val = cells[y * SIZE + x];
    if (Piece::isPiece(val))
        cells[y * SIZE + x] = Piece::PLAYABLE_EMPTY;
}

int Board::getCell(int x, int y) const {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) return 0;
    return cells[y * SIZE + x];
}

void Board::setPiece(int x, int y, int piece) {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) return;
    cells[y * SIZE + x] = piece;
}
