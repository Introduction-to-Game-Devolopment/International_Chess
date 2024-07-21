#include "board.h"
#include "raylib/src/raylib.h"

// Constructor cho class cell
cell::cell(XY pos, int16_t type, size frame_size=FRAME_SIZE, int padding) {
    this->Piece = init_piece(type);
    this->pos = pos;                // Vị trí ô (ví dụ {0,1}, {0,2}..., {0,8})
    
    this->length = (frame_size.h - 2*padding)/8;
    point position = {padding + pos.x*length, padding + pos.y*length};
    
    this->rec.x = position.x;
    this->rec.y = position.y;
    this->rec.height = this->length;
    this->rec.width = this->length;
    
    this->is_hover = false;
    this->is_chosen = false;
}

void cell::draw_cell() {
    Color color;
    if (this->is_chosen)
        color = CHOSEN_CELL_COLOR;
    else if (this->is_hover)
        color = HOVERED_CELL_COLOR;
    else if ((this->pos.x + this->pos.y) & 1) {
        color = ODD_CELL_COLOR;
    } else color = EVEN_CELL_COLOR;
    DrawRectangleRec(this->rec, color);
    if (this->Piece.get_type() == 0) return;
    draw_picture(this->Piece.get_texture(), this->get_rect());
}

// ------- Modify thêm ---------
void cell::hover(piece _piece) {
    int16_t type = _piece.get_type();
    if (type*this->Piece.get_type() < 0 ||type == 0)
        this->is_hover = true;
}

void cell::unhover() {
    this->is_hover = false;
}

void cell::choose(int8_t turn) {
    if (turn*this->Piece.get_type() > 0)
        this->is_chosen = true;
}

void cell::unchoose() {
    this->is_chosen = false;
}

board::board(std::string white_player, std::string black_player) {
    this->white_player = player(white_player, 1);
    this->black_player = player(black_player, -1);
    this-> turn = 1;
    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            this->board_game[i][j] = cell({i, j}, 0); 
        }
    }
    
    //Quân tốt
    for (int j = 0; j < 8; j++) {
        board_game[1][j] = cell({1,j}, -1);
        board_game[6][j] = cell({6,j}, 1);
    }
    
    // Quân tượng
    board_game[0][1] = cell({0,2}, -2);
    board_game[0][6] = cell({0,5}, -2);
    board_game[7][1] = cell({7,2}, 2);
    board_game[7][6] = cell({7,5}, 2);
    
    // Quân mã
    board_game[0][2] = cell({0,1}, -3);
    board_game[0][5] = cell({0,6}, -3);
    board_game[7][2] = cell({7,1}, 3);
    board_game[7][5] = cell({7,6}, 3);
    
    // Quân xe
    board_game[0][0] = cell({0,0}, -4);
    board_game[0][7] = cell({0,7}, -4);
    board_game[7][0] = cell({7,0}, 4);
    board_game[7][7] = cell({7,7}, 4);
    
    // Quân hậu
    board_game[0][3] = cell({0,3}, -5);
    board_game[7][3] = cell({7,3}, 5);
    
    // Quân Vua
    board_game[0][4] = cell({0,4}, -6);
    board_game[7][4] = cell({7,4}, 6);
}

void board::draw_board() {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            this->board_game[i][j].draw_cell();
        }
    }
}




