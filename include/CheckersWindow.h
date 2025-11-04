#pragma once
#include "Graph_lib/Graph.h"
#include "Graph_lib/Simple_window.h"
#include "Board.h"

// Класс окна, в котором будет отображаться игровая доска и фигуры
class CheckersWindow : public Graph_lib::Window {
public:
    // Конструктор: создаёт окно заданного размера
    CheckersWindow(Graph_lib::Point xy, int w, int h, const std::string& title);

    int handle(int event) override;

private:
    Board board;  // Игровая доска
};