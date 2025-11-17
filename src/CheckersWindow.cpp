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
            if (Piece::isPiece(val)) {
                std::string file = "";
                if (Piece::isBlack(val)) {
                    file = "resources/pieces/black.png";
                } else if (Piece::isWhite(val)) {
                    file = "resources/pieces/white.png";
                }
                
                auto img = std::make_unique<Image>(Point{x * cell_size, y * cell_size}, file);
                attach(*img);
                images.push_back(std::move(img));

                // Если это дамка, рисуем корону поверх
                if (Piece::isKing(val)) {
                    auto king_img = std::make_unique<Image>(Point{x * cell_size, y * cell_size}, "resources/pieces/king_overlay.png");
                    attach(*king_img);
                    images.push_back(std::move(king_img));
                }
            }
        }
    }
}

std::string CheckersWindow::getImageFile(int val) const{
    // Этот метод больше не нужен в таком виде, но оставим его на случай будущих изменений
    switch (val) {
        case Piece::BLACK_MAN:
            return "resources/pieces/black.png";
        case Piece::WHITE_MAN:
            return "resources/pieces/white.png";
        case Piece::BLACK_KING: // Для отладки можно оставить
        case Piece::WHITE_KING:
            return "resources/pieces/king_overlay.png";
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