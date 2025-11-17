#pragma once
#include "Graph_lib/Graph.h"
#include "Graph_lib/Simple_window.h"
#include "Board.h"
#include "Game.h"
#include "Piece.h"


class CheckersWindow : public Graph_lib::Window {
public:
    CheckersWindow(Graph_lib::Point xy, int w, int h, const std::string& title, Game& game);
    void draw_board();
    void draw_all_pieces();
    void highlight_cell(int x, int y);
    int handle(int event) override;

private:
    Game& game;
    std::string getImageFile(int val) const;
    std::vector<std::unique_ptr<Graph_lib::Image>> images;
};