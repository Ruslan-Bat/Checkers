#include "CheckersWindow.h"
#include "Graph_lib/Graph.h"
#include "Graph_lib/Simple_window.h"
#include <iostream>

using namespace Graph_lib;

CheckersWindow::CheckersWindow(Point xy, int w, int h, const std::string& title)
    : Window{xy, w, h, title}, board(Board::CELL_SIZE) // размер клетки из константы
{
    // Рисуем доску и все шашки
    board.draw(*this);
}



int CheckersWindow::handle(int event)
{
    switch (event)
    {
    case FL_PUSH: // Нажатие кнопки мыши
    {
        int x = Fl::event_x();
        int y = Fl::event_y();

        // можно перевести в координаты клетки доски:
        int cellX = x / Board::CELL_SIZE;
        int cellY = y / Board::CELL_SIZE;

        if (board.hasPiece(cellX, cellY)) {
            std::cout << "In kletka (" << cellX << ", " << cellY << ") has Piese\n";
        } else {
            std::cout << "kletka (" << cellX << ", " << cellY << ") pusta\n";
}

        return 1; // сигнал FLTK, что событие обработано
    }
    default:
        return Window::handle(event); // другие события обрабатываются стандартно
    }
}