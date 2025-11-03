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