#include "CheckersWindow.h"
#include "Graph_lib/Graph.h"
#include "Graph_lib/Simple_window.h"
#include <iostream>

using namespace Graph_lib;


CheckersWindow::CheckersWindow(Point xy, int w, int h, const std::string& title, Game& game_ref)
    : Window{xy, w, h, title}, game(game_ref)
{
    draw_board();
    draw_all_pieces();
    
}

void CheckersWindow::draw_board() {
    int SIZE = Board::SIZE;
    int cell_size = Board::CELL_SIZE;
    
    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            Color color = ((x + y) % 2 == 0) ? Color::white : Color::dark_green;
            Rectangle *r = new Rectangle{Point{x * cell_size, y * cell_size}, cell_size, cell_size};
            r->set_fill_color(color);
            attach(*r);
        }
    }
}

void CheckersWindow::draw_all_pieces(){
    int SIZE = Board::SIZE;
    int cell_size = Board::CELL_SIZE;

    // удаляем старые изображения
    for (auto& img : images) {
        detach(*img);
    }
    images.clear();

    for (int y = 0; y < SIZE; ++y) {
        for (int x = 0; x < SIZE; ++x) {
            int val = game.getBoard().getCell(x, y);
            if (val >= 2 && val <= 5) { // шашка
                std::string file = getImageFile(val);
                auto img = std::make_unique<Image>(Point{x * cell_size, y * cell_size}, file);
                attach(*img);
                images.push_back(std::move(img));
            }
        }
    }
}

std::string CheckersWindow::getImageFile(int val) const{
    switch (val) {
        case Piece::BLACK_MAN: // черная 
            return "resources/pieces/black.png";
        case Piece::BLACK_KING: // черная дамка
            return "resources/pieces/king_overlay.png"; // можно заменить на дамку
        case Piece::WHITE_MAN: // белая шашка 
            return "resources/pieces/white.png";
        case Piece::WHITE_KING: // белая дамка
            return "resources/pieces/king_overlay.png"; // можно заменить на дамку
        default:
            return "";
    }
}




int CheckersWindow::handle(int event)
{
    switch (event)
    {
    case FL_PUSH:
    {
        int x = Fl::event_x();
        int y = Fl::event_y();

        // можно перевести в координаты клетки доски:
        int cellX = x / Board::CELL_SIZE;
        int cellY = y / Board::CELL_SIZE;

        game.handleClick(cellX, cellY);
        draw_all_pieces();
        redraw();


        return 1; // сигнал FLTK, что событие обработано
    }
    default:
        return Window::handle(event); // другие события обрабатываются стандартно
    }
}