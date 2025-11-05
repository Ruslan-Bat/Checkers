#include "Game.h"
#include <iostream>
#include <cmath> // для std::abs

Game::Game(Board& b)
    : board(b), pieceSelected(false), selectedX(-1), selectedY(-1)
{}

void Game::handleClick(int cellX, int cellY)
{
    // Если ничего не выбрано и в клетке есть фишка — выбираем
    if (!pieceSelected && board.hasPiece(cellX, cellY)) {
        pieceSelected = true;
        selectedX = cellX;
        selectedY = cellY;
        std::cout << "A feature has been selected: (" << cellX << ", " << cellY << ")\n";
        return;
    }

    // Если фишка выбрана и клик по пустой клетке — пробуем сделать ход
    if (pieceSelected && !board.hasPiece(cellX, cellY)) {
        if (isValidMove(selectedX, selectedY, cellX, cellY)) {
            // Если ход — удар (на 2 клетки по диагонали)
            int dx = cellX - selectedX;
            int dy = cellY - selectedY;

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
        } else {
            std::cout << "An unacceptable move.\n";
        }

        pieceSelected = false;
        selectedX = selectedY = -1;
        return;
    }

    // Если клик по другой фишке — смена выбора
    if (board.hasPiece(cellX, cellY)) {
        selectedX = cellX;
        selectedY = cellY;
        std::cout << "Another feature has been selected: (" << cellX << ", " << cellY << ")\n";
    }

    // Если клик по другой фишке, то меняем выбор
    if (board.hasPiece(cellX, cellY)) {
        selectedX = cellX;
        selectedY = cellY;
        std::cout << "Another checker is selected: (" << selectedX << ", " << selectedY << ")\n";
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

    // Ход для взятия (удар через клетку)
    if (dx == 2 && dy == 2) {
        int midX = (fromX + toX) / 2;
        int midY = (fromY + toY) / 2;

        // В середине должна быть фишка противника
        Piece* middle = board.pieceAt(midX, midY);
        Piece* fromPiece = board.pieceAt(fromX, fromY);

        if (middle && fromPiece && middle->getColor() != fromPiece->getColor()) {
            return true;
        }
    }

    return false;
}
