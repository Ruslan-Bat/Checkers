#pragma once

// Определяем возможные цвета шашек
enum class PieceColor {
    NONE,
    WHITE,
    BLACK
};

// Определяем типы шашек
enum class PieceType {
    NONE,
    MAN,
    KING
};

// Используем namespace Piece для определения констант и вспомогательных функций
namespace Piece {
    // Числовые значения для различных типов и цветов шашек
    const int EMPTY = 0; // Пустая светлая клетка (неиспользуемая)
    const int PLAYABLE_EMPTY = 1; // Пустая игровая клетка (темная)

    const int BLACK_MAN = 2;
    const int BLACK_KING = 3;
    const int WHITE_MAN = 4;
    const int WHITE_KING = 5;

    // Вспомогательные функции для интерпретации числовых значений
    bool isPiece(int value);
    bool isBlack(int value);
    bool isWhite(int value);
    bool isMan(int value);
    bool isKing(int value);
    PieceColor getColor(int value);
    PieceType getType(int value);
}
