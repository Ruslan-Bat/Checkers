#include "Player.h"
#include "Piece.h"
#include <cmath>

std::vector<int> Player::getAIMove(const Board& board) {
    // Проходим по всей доске
    for (int y = 0; y < Board::SIZE; ++y) {
        for (int x = 0; x < Board::SIZE; ++x) {
            int piece = board.getCell(x, y);

            // Ищем черную шашку или дамку
            if (!Piece::isBlack(piece)) {
                continue;
            }

            // Проверяем все возможные конечные точки
            for (int newY = 0; newY < Board::SIZE; ++newY) {
                for (int newX = 0; newX < Board::SIZE; ++newX) {
                    if (board.hasPiece(newX, newY)) continue; // Клетка занята

                    int dx_abs = std::abs(x - newX);
                    int dy_abs = std::abs(y - newY);

                    if (dx_abs != dy_abs) continue; // Не диагональ

                    // --- Логика для обычных шашек ---
                    if (Piece::isMan(piece)) {
                        if (newY < y) continue; // Черные ходят только вниз

                        if (dx_abs == 1) return {x, y, newX, newY}; // Простой ход

                        if (dx_abs == 2) {
                            int midX = (x + newX) / 2;
                            int midY = (y + newY) / 2;
                            int midPiece = board.getCell(midX, midY);
                            if (Piece::isWhite(midPiece)) {
                                return {x, y, midX, midY, newX, newY}; // Взятие
                            }
                        }
                    }
                    // --- Логика для дамок ---
                    else if (Piece::isKing(piece)) {
                        int stepX = (newX > x) ? 1 : -1;
                        int stepY = (newY > y) ? 1 : -1;
                        
                        int path_len = dx_abs;
                        int captured_count = 0;
                        int captured_x = -1, captured_y = -1;

                        for (int i = 1; i < path_len; ++i) {
                            int path_x = x + i * stepX;
                            int path_y = y + i * stepY;
                            int path_piece = board.getCell(path_x, path_y);
                            if (Piece::isPiece(path_piece)) {
                                if (Piece::isWhite(path_piece)) {
                                    captured_count++;
                                    captured_x = path_x;
                                    captured_y = path_y;
                                } else { // Своя шашка на пути
                                    captured_count = 2; // Делаем путь невалидным
                                    break;
                                }
                            }
                        }

                        if (captured_count == 0) {
                            return {x, y, newX, newY}; // Простой ход дамкой
                        }
                        if (captured_count == 1) {
                            return {x, y, captured_x, captured_y, newX, newY}; // Взятие дамкой
                        }
                    }
                }
            }
        }
    }
    return {}; // Ходов не найдено
}
