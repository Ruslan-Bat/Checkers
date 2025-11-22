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
            
            int piece = board.cellAt(selectedX, selectedY);
            int dx_abs = std::abs(cellX - selectedX);

            // Если ход — удар (на 2+ клетки по диагонали для дамки, или на 2 для шашки)
            if (dx_abs >= 2) {
                // Для шашки взятие простое
                if (Piece::isMan(piece)) {
                    int midX = (selectedX + cellX) / 2;
                    int midY = (selectedY + cellY) / 2;
                    board.removePiece(midX, midY);
                    std::cout << "A chip was eaten on (" << midX << ", " << midY << ")\n";
                }
                // Для дамки ищем, кого съели
                else if (Piece::isKing(piece)) {
                    int stepX = (cellX > selectedX) ? 1 : -1;
                    int stepY = (cellY > selectedY) ? 1 : -1;
                    for (int i = 1; i < dx_abs; ++i) {
                        int currX = selectedX + i * stepX;
                        int currY = selectedY + i * stepY;
                        if (Piece::isPiece(board.cellAt(currX, currY))) {
                            board.removePiece(currX, currY);
                            std::cout << "King ate a piece on (" << currX << ", " << currY << ")\n";
                            break; // Предполагаем только одно взятие за ход
                        }
                    }
                }
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
    int toX = move.at(move.size() - 2);
    int toY = move.at(move.size() - 1);

    // Перемещаем фигуру
    board.movePiece(fromX, fromY, toX, toY);
    std::cout << "Computer move: (" << fromX << ", " << fromY << ") -> (" 
              << toX << ", " << toY << ")\n";

    // Если были съедены фигуры, удаляем их
    if (move.size() > 2) {
        for (size_t i = 2; i < move.size() - 2; i += 2) {
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
    if (!Piece::isPiece(piece)) return false; // на стартовой клетке нет фигуры
    if (board.hasPiece(toX, toY)) return false; // конечная клетка занята

    int dx_abs = std::abs(toX - fromX);
    int dy_abs = std::abs(toY - fromY);

    if (dx_abs != dy_abs) return false; // Ход не по диагонали

    // --- Логика для обычных шашек ---
    if (Piece::isMan(piece)) {
        int dy_direction = toY - fromY;
        if (Piece::isWhite(piece) && dy_direction == 1) return false; // Белые ходят только вверх

        // Обычный ход
        if (dx_abs == 1) {
            return true;
        }
        // Ход со взятием
        if (dx_abs == 2) {
            int midX = (fromX + toX) / 2;
            int midY = (fromY + toY) / 2;
            int midPiece = board.cellAt(midX, midY);
            if (Piece::isPiece(midPiece) && Piece::getColor(midPiece) != Piece::getColor(piece)) {
                return true;
            }
        }
        return false; // Для шашек другие ходы невозможны
    }

    // --- Логика для дамок ---
    if (Piece::isKing(piece)) {
        int stepX = (toX > fromX) ? 1 : -1;
        int stepY = (toY > fromY) ? 1 : -1;

        int capturedPiecesCount = 0;
        int pathLength = dx_abs;

        // Проверяем все клетки на пути
        for (int i = 1; i < pathLength; ++i) {
            int currX = fromX + i * stepX;
            int currY = fromY + i * stepY;
            if (Piece::isPiece(board.cellAt(currX, currY))) {
                capturedPiecesCount++;
            }
        }

        // Если на пути нет фигур - это обычный ход
        if (capturedPiecesCount == 0) {
            return true;
        }
        // Если на пути одна фигура - это может быть взятие
        if (capturedPiecesCount == 1) {
            // Проверяем, что эта фигура - вражеская
            for (int i = 1; i < pathLength; ++i) {
                int currX = fromX + i * stepX;
                int currY = fromY + i * stepY;
                int pathPiece = board.cellAt(currX, currY);
                if (Piece::isPiece(pathPiece) && Piece::getColor(pathPiece) != Piece::getColor(piece)) {
                    return true; // Найдена вражеская фигура для взятия
                }
            }
        }
        // Если на пути больше одной фигуры или одна своя - ход невозможен
    }

    return false;
}
