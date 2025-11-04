#pragma once
#include "Graph_lib/Graph.h"
#include "Graph_lib/Window.h"
#include <string>

enum class PieceColor { WHITE, BLACK };

// Класс одной шашки
class Piece {
public:
    // Конструктор: задаёт цвет, позицию и размер клетки
    Piece(PieceColor color, Graph_lib::Point position, int cell_size);

    // Отрисовать шашку в окне
    void draw(Graph_lib::Window& win);

    // Узнать цвет
    PieceColor getColor() const { return color; }

private:
    PieceColor color;               // Цвет шашки
    Graph_lib::Point position; // Верхний левый угол шашки на экране
    int cell_size;             // Размер клетки (для масштабирования изображения)

    Graph_lib::Image image;    // Графическое представление шашки
};