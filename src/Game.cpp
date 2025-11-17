#include "Game.h"
#include <iostream>
#include <cmath> // для std::abs
#include "Piece.h" // Explicitly include Piece.h

Game::Game(Board& b)
    : board(b), pieceSelected(false), selectedX(-1), selectedY(-1)
{}

void Game::handleClick(int cellX, int cellY)
{
    // Ход человека (только для белых)
    if (!whiteTurn) {
        std::cout << "Now it's the computer's turn.\n";
        return;
    }

    auto pieceVal = board.getCell(cellX, cellY);
    
    // Если ничего не выбрано и клик по своей шашке — выбираем
    if (!pieceSelected && board.hasPiece(cellX, cellY)) {
        if (Piece::isWhite(pieceVal)){
            pieceSelected = true;
            selectedX = cellX;
            selectedY = cellY;
            return;
        }
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

            whiteTurn = false; // Передаем ход компьютеру
        } else {
            std::cout << "An unacceptable move.\n";
        }

        pieceSelected = false;
        selectedX = selectedY = -1;
    }

    // Если клик по другой своей фишке — смена выбора
    else if (pieceSelected && Piece::isWhite(pieceVal)) {
        selectedX = cellX;
        selectedY = cellY;
        std::cout << "Another feature has been selected: (" << cellX << ", " << cellY << ")\n";
    }

    // Если после хода человека наступил ход компьютера, вызываем его
    if (!whiteTurn) {
        promoteIfNecessary(cellX, cellY); // Проверяем на превращение в дамку
        handleAITurn();
    }
}

void Game::handleAITurn() {
    std::cout << "The computer thinks...\n";
    auto move = Player::getAIMove(board);

    if (move.empty()) {
        std::cout << "The computer has no moves, your turn.\n";
        whiteTurn = true; // Если ходов нет, передаем ход обратно
        return;
    }

    int fromX = move[0];
    int fromY = move[1];
    int toX = move[2];
    int toY = move[3];

    // Перемещаем фигуру
    board.movePiece(fromX, fromY, toX, toY);
    std::cout << "Computer move: (" << fromX << ", " << fromY << ") -> (" 
              << toX << ", " << toY << ")\n";

    // Если были съедены фигуры, удаляем их
    if (move.size() > 4) {
        for (size_t i = 4; i < move.size(); i += 2) {
            int capturedX = move[i];
            int capturedY = move[i+1];
            board.removePiece(capturedX, capturedY);
            std::cout << "Computer ate a piece on (" << capturedX << ", " << capturedY << ")\n";
        }
    }
    
    promoteIfNecessary(toX, toY); // Проверяем на превращение в дамку
    whiteTurn = true; // Передаем ход человеку
}

void Game::promoteIfNecessary(int x, int y) {
    int piece = board.getCell(x, y);

    // Белая шашка достигла верхнего края
    if (Piece::isWhite(piece) && Piece::isMan(piece) && y == 0) {
        board.setPiece(x, y, Piece::WHITE_KING);
        std::cout << "White piece promoted to King at (" << x << ", " << y << ")\n";
    }
    // Черная шашка достигла нижнего края
    else if (Piece::isBlack(piece) && Piece::isMan(piece) && y == Board::SIZE - 1) {
        board.setPiece(x, y, Piece::BLACK_KING);
        std::cout << "Black piece promoted to King at (" << x << ", " << y << ")\n";
    }
}

bool Game::isValidMove(int fromX, int fromY, int toX, int toY) const
{
    int piece = board.cellAt(fromX, fromY);
    if (!Piece::isPiece(piece)) {
        return false;
    }

    // Проверяем направление для обычной шашки
    if (Piece::isMan(piece)) {
        int dy_direction = toY - fromY;
        if (Piece::isWhite(piece) && dy_direction >= 0) {
            return false; // Белые ходят только вверх (dy < 0)
        }
        if (Piece::isBlack(piece) && dy_direction <= 0) {
            return false; // Черные ходят только вниз (dy > 0)
        }
    }
    // Для дамок (isKing) это ограничение не применяется, они могут ходить в любую сторону.

    int dx = std::abs(toX - fromX);
    int dy = std::abs(toY - fromY);

    // Обычный ход на одну клетку по диагонали
    if (dx == 1 && dy == 1) {
        return true;
    }

    // Ход со взятием на две клетки по диагонали
    if (dx == 2 && dy == 2) {
        int midX = (fromX + toX) / 2;
        int midY = (fromY + toY) / 2;

        int midVal = board.cellAt(midX, midY);

        // Взятие возможно, только если на промежуточной клетке стоит фигура противника
        if (Piece::isPiece(midVal) && Piece::getColor(midVal) != Piece::getColor(piece)) {
            return true;
        }
    }
    return false;
}
