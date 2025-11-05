#include "Piece.h"
#include "Graph_lib/Window.h"
#include <string>

using namespace Graph_lib;

Piece::Piece(PieceColor color, Point position, int cell_size)
    : color(color), position(position), cell_size(cell_size),
      image(position, color == PieceColor::WHITE ? "resources/pieces/white.png"
                                                : "resources/pieces/black.png")
{

}

void Piece::draw(Window &win) {
    win.attach(image);
}

void Piece::setPosition(Point new_pos)
{
    // Вычисляем смещение
    int dx = new_pos.x - position.x;
    int dy = new_pos.y - position.y;

    // Сдвигаем изображение
    image.move(dx, dy);

    // Обновляем позицию
    position = new_pos;
}

void Piece::detachFrom(Window& win)
{
    win.detach(image);
}