#include "CheckersWindow.h"
#include "Graph_lib/Graph.h"
#include "Graph_lib/Simple_window.h"

using namespace Graph_lib;

CheckersWindow::CheckersWindow(Point xy, int w, int h, const std::string& title)
    : Window{xy, w, h, title}, board(Board::CELL_SIZE) // размер клетки из константы
{
    // Рисуем доску и все шашки
    board.draw(*this);
}