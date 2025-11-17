#include "Player.h"
#include "Piece.h"
#include <cmath>

// Временная реализация: ищет первый доступный ход для черных.
std::vector<int> Player::getAIMove(const Board& board) {
    // Проходим по всей доске
    for (int y = 0; y < Board::SIZE; ++y) {
        for (int x = 0; x < Board::SIZE; ++x) {
            int piece = board.getCell(x, y);

            // Ищем черную шашку или дамку
            if (Piece::isBlack(piece)) {
                // Проверяем возможные ходы для этой шашки
                for (int newY = 0; newY < Board::SIZE; ++newY) {
                    for (int newX = 0; newX < Board::SIZE; ++newX) {
                        
                        // Проверка направления для обычной шашки
                        if (Piece::isMan(piece) && newY < y) {
                            continue; // Обычная черная шашка ходит только вниз
                        }

                        // Простое перемещение
                        if (std::abs(x - newX) == 1 && std::abs(y - newY) == 1 && !board.hasPiece(newX, newY)) {
                            return {x, y, newX, newY}; // Возвращаем простой ход
                        }
                        // Ход со взятием
                        if (std::abs(x - newX) == 2 && std::abs(y - newY) == 2 && !board.hasPiece(newX, newY)) {
                            int midX = (x + newX) / 2;
                            int midY = (y + newY) / 2;
                            int midPiece = board.getCell(midX, midY);
                            if (Piece::isWhite(midPiece)) {
                                return {x, y, newX, newY, midX, midY}; // Возвращаем ход со взятием
                            }
                        }
                    }
                }
            }
        }
    }
    return {}; // Если ходов не найдено, возвращаем пустой вектор
}
