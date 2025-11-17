#include "Piece.h"

namespace Piece {

bool isPiece(int value) {
    return value == BLACK_MAN || value == BLACK_KING ||
           value == WHITE_MAN || value == WHITE_KING;
}

bool isBlack(int value) {
    return value == BLACK_MAN || value == BLACK_KING;
}

bool isWhite(int value) {
    return value == WHITE_MAN || value == WHITE_KING;
}

bool isMan(int value) {
    return value == BLACK_MAN || value == WHITE_MAN;
}

bool isKing(int value) {
    return value == BLACK_KING || value == WHITE_KING;
}

PieceColor getColor(int value) {
    if (isBlack(value)) return PieceColor::BLACK;
    if (isWhite(value)) return PieceColor::WHITE;
    return PieceColor::NONE;
}

PieceType getType(int value) {
    if (isMan(value)) return PieceType::MAN;
    if (isKing(value)) return PieceType::KING;
    return PieceType::NONE;
}

} // namespace Piece
