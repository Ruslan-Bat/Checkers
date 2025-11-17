#include "Game.h"
#include <iostream>
#include <cmath> // для std::abs
#include "Piece.h" // Explicitly include Piece.h

Game::Game(Board& b)
    : board(b), pieceSelected(false), selectedX(-1), selectedY(-1)
{}

void Game::handleClick(int cellX, int cellY)
{
    auto pieceVal = board.getCell(cellX, cellY);
    
    // Если ничего не выбрано и клик по своей шашке — выбираем
    if (!pieceSelected && board.hasPiece(cellX, cellY)) {
        if ( (whiteTurn && Piece::isWhite(pieceVal)) || 
            (!whiteTurn && Piece::isBlack(pieceVal)) ){
            pieceSelected = true;
            selectedX = cellX;
            selectedY = cellY;
            return;
        }
        std::cout << "It's not your turn!\n";
        return;
    }

    // Если фишка выбрана и клик по пустой клетке — пробуем сделать ход
    if (pieceSelected && !board.hasPiece(cellX, cellY)) {
        if (isValidMove(selectedX, selectedY, cellX, cellY)) {
            
            int dx = cellX - selectedX;
            int dy = cellY - selectedY;

            // Если ход — удар (на 2 клетки по диагонали)
            if (std::abs(dx) == 2 && std::abs(dy) == 2) {
                int midX = (selectedX + cellX) / 2;
                int midY = (selectedY + cellY) / 2;

                // Удаляем вражескую фишку
                board.removePiece(midX, midY);
                std::cout << "A chip was eaten on (" << midX << ", " << midY << ")\n";
            }

            // Перемещаем фишку
            board.movePiece(selectedX, selectedY, cellX, cellY);
            std::cout << "The move is completed: (" << selectedX << ", " << selectedY << ") -> (" 
                      << cellX << ", " << cellY << ")\n";

            whiteTurn = !whiteTurn;
        } else {
            std::cout << "An unacceptable move.\n";
        }

        pieceSelected = false;
        selectedX = selectedY = -1;
        return;
    }

    // Если клик по другой фишке — смена выбора
    if ( (Piece::isBlack(pieceVal) && !whiteTurn)
    || (Piece::isWhite(pieceVal) && whiteTurn) ) {
        selectedX = cellX;
        selectedY = cellY;
        std::cout << "Another feature has been selected: (" << cellX << ", " << cellY << ")\n";
    }
}
bool Game::isValidMove(int fromX, int fromY, int toX, int toY) const
{
    int dx = std::abs(toX - fromX);
    int dy = std::abs(toY - fromY);

    // Обычный ход на одну клетку по диагонали
    if (dx == 1 && dy == 1) {
        return true;
    }

    if (dx == 2 && dy == 2) {
        int midX = (fromX + toX) / 2;
        int midY = (fromY + toY) / 2;

        int fromVal = board.cellAt(fromX, fromY);
        int midVal = board.cellAt(midX, midY);

        if (Piece::isPiece(midVal) && Piece::getColor(midVal) != Piece::getColor(fromVal)) {
            return true;
        }
    }
    return false;
}
