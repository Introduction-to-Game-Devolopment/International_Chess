#include "board.h"
#include "general.h"
#include "piece.h"
#include <vector>


// Constructor cho class cell
cell::cell(XY pos, int16_t type, size frame_size=FRAME_SIZE, int padding) {
    this->Piece = init_piece(type);
    this->pos = pos;                // Vị trí ô (ví dụ {0,1}, {0,2}..., {0,8})

    this->length = (frame_size.h - 2*padding)/8;
    this->Piece = init_piece(type);
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
    if (this->Piece.get_type() == 0 || this->Piece.get_is_exist() == 0) return;
    draw_picture(this->Piece.get_texture(), this->get_rect());
}

// ------- Modify thêm ---------
void cell::change_piece(piece Piece) {
    this->Piece = Piece;
}
void cell::hover() {
    this->is_hover = true;
}

void cell::unhover() {
    this->is_hover = false;
}

void cell::choose(int8_t turn, Vector2 mouse_pos) {
    if (mouse_pos < this->get_rect() && turn*this->Piece.get_type() > 0){
        this->is_chosen = true;
        return;
    }
    this->is_chosen = false;
}

void cell::unchoose() {
    this->is_chosen = false;
}

board::board(std::string white_player, std::string black_player) {
    this->white_player = player(white_player, 1);
    this->black_player = player(black_player, -1);
    this->turn = 1;
    this->en_passant = {-1, -1};
    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            this->board_game[i][j] = cell({i, j}, 0);
        }
    }

    //Quân tốt
    for (int j = 0; j < 8; j++) {
        this->board_game[1][j] = cell({1,j}, -1);
        this->board_game[6][j] = cell({6,j}, 1);
    }

    // Quân tượng
    this->board_game[0][2] = cell({0,2}, -2);
    this->board_game[0][5] = cell({0,5}, -2);
    this->board_game[7][2] = cell({7,2}, 2);
    this->board_game[7][5] = cell({7,5}, 2);

    // Quân mã
    this->board_game[0][1] = cell({0,1}, -3);
    this->board_game[0][6] = cell({0,6}, -3);
    this->board_game[7][1] = cell({7,1}, 3);
    this->board_game[7][6] = cell({7,6}, 3);

    // Quân xe
    this->board_game[0][0] = cell({0,0}, -4);
    this->board_game[0][7] = cell({0,7}, -4);
    this->board_game[7][0] = cell({7,0}, 4);
    this->board_game[7][7] = cell({7,7}, 4);

    // Quân hậu
    this->board_game[0][3] = cell({0,3}, -5);
    this->board_game[7][3] = cell({7,3}, 5);

    // Quân Vua
    this->board_game[0][4] = cell({0,4}, -6);
    this->board_game[7][4] = cell({7,4}, 6);
}

void board::draw_board() {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            this->board_game[i][j].draw_cell();
        }
    }
}

bool board::is_blocked(XY pos) {
    return this->board_game[pos.y][pos.x].is_exist_piece();
}

bool board::is_captured(XY pos) {
    return this->turn * this->board_game[pos.y][pos.x].get_type_piece() < 0;
}

bool board::is_en_passant(XY pos) {
    return !is_blocked(pos) && pos + PAWN_MOVE[this->turn > 0 ? 4 : 1] == this->en_passant;
}

std::vector<XY> board::get_move(cell square) {
    if (!square.is_exist_piece()) return std::vector<XY>(0);

    std::vector<XY> next_move(0);
    XY pos = square.get_pos();
    XY next_pos;
    int dir;
    switch (square.get_type_piece() * this->turn) {
        case 1:
        //promotion
            dir = this->turn > 0 ? 1 : 4;
            next_pos = pos - PAWN_MOVE[dir];
            if (is_inside(next_pos) && !is_blocked(next_pos)) {
                next_move.emplace_back(next_pos);
                /*
                if (is_promotion(next_pos) {}
                */
                if (!square.is_moved_piece() && is_inside(next_pos -= PAWN_MOVE[dir]) && !is_blocked(next_pos)) {
                    next_move.emplace_back(next_pos);
                }
            }
            for (int capture_dir : {dir - 1, dir + 1}) {
                next_pos = pos - PAWN_MOVE[capture_dir];
                if (is_inside(next_pos)) {
                    if (is_captured(next_pos)) {
                        next_move.emplace_back(next_pos);
                    }
                    if (is_en_passant(next_pos)) {
                        next_move.emplace_back(next_pos);
                    }
                }
            }
            break;
        case 2:
            for (int dir = 0; dir < 4; ++dir) {
                next_pos = pos;
                while (is_inside(next_pos += BISHOP_MOVE[dir])) {
                    if (is_blocked(next_pos)) {
                        if (is_captured(next_pos)) {
                            next_move.emplace_back(next_pos);
                        }
                        break;
                    }
                    else {
                        next_move.emplace_back(next_pos);
                    }
                }
            }
            break;
        case 3:
            for (int dir = 0; dir < 8; ++dir) {
                next_pos = pos + KNIGHT_MOVE[dir];
                if (is_inside(next_pos)) {
                    next_move.emplace_back(next_pos);
                }
            }
            break;
        case 4:
        //unable to castle if moved
            for (int dir = 0; dir < 4; ++dir) {
                next_pos = pos;
                while (is_inside(next_pos += ROOK_MOVE[dir])) {
                    if (is_blocked(next_pos)) {
                            if (is_captured(next_pos)) {
                                next_move.emplace_back(next_pos);
                        }
                        break;
                    }
                    else {
                        next_move.emplace_back(next_pos);
                    }
                }
            }
            break;
        case 5:
            for (int dir = 0; dir < 8; ++dir) {
                next_pos = pos;
                while (is_inside(next_pos += QUEEN_MOVE[dir])) {
                    if (is_blocked(next_pos)) {
                        if (is_captured(next_pos)) {
                            next_move.emplace_back(next_pos);
                        }
                        break;
                    }
                    else {
                        next_move.emplace_back(next_pos);
                    }
                }
            }
            break;
        case 6:
        //king capture king
        //king capture protected pieces
        //no castling while being checked
        //no move to captured place
        //no castle when moved
            for (int dir = 0; dir < 8; ++dir) {
                next_pos = pos + KING_MOVE[dir];
                if (is_inside(next_pos)) {
                    if (is_blocked(next_pos)) {
                        //if (is_opponent_king())
                        //if (is_protected())
                        if (is_captured(next_pos)) {
                            next_move.emplace_back(next_pos);
                        }
                    }
                    else {
                        //if (is_check());
                        next_move.emplace_back(next_pos);
                    }
                }
            }
            //when move a piece, set was_move to {true}
            if (!square.is_moved_piece()) {
                for (int dir : {0, 4}) {
                    bool castle = true;
                    next_pos = pos;
                    for (int step = 1; step <= 3; ++step) castle &= !is_blocked(next_pos += KING_MOVE[dir]);
                    if (!castle) continue;

                    for (XY rook_pos = next_pos + KING_MOVE[dir]; is_inside(rook_pos); rook_pos += KING_MOVE[dir]) {
                        if (is_blocked(rook_pos)) {
                            cell& rook_square = this->board_game[rook_pos.x][rook_pos.y];
                            if (!rook_square.is_moved_piece() && rook_square.get_type_piece() * this->turn == 4) {
                                next_move.emplace_back(next_pos);
                            }
                            //chose king and rook to castle
                            break;
                        }
                    }
                }
            }
            break;
    }
    return next_move;
}

void board::wait_for_event(Vector2 mouse_pos) {
    std::vector<XY> next_move;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            this->board_game[i][j].choose(this->turn, mouse_pos);
            this->board_game[i][j].unhover();
            if (this->board_game[i][j].get_chosen()) {
                next_move = this->get_move(this->board_game[i][j]);
                    if (next_move.empty())
                    printf("chosen %d" , int(this->board_game[i][j].get_chosen()));
            }
        }
    }

    for (std::vector<XY>::size_type i = 0; i < next_move.size(); i++) {
        int X = next_move[i].x, Y = next_move[i].y;
        this->board_game[Y][X].hover();
    }
}

void board::swap_cell(cell &cell1, cell &cell2) {
    piece tmp = cell1.get_piece();
    cell1.change_piece(cell2.get_piece());
    cell2.change_piece(tmp);
    printf("PASS\n");
}
bool board::make_move(Vector2 mouse_pos) {
    XY hovered_pos = {-1, -1}, chosen_pos = {-1, -1};
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (mouse_pos < this->board_game[i][j].get_rect()) {
                if (this->board_game[i][j].get_hover()) {
                    hovered_pos = {i, j};
                }
            }
            if (this->board_game[i][j].get_chosen()) {
                chosen_pos = {i, j};
            }
            this->board_game[i][j].unchoose();
            this->board_game[i][j].unhover();
        }
    }

    printf("%d - %d | %d - %d\n", hovered_pos.y, hovered_pos.x, chosen_pos.y, chosen_pos.x);      //--------------------------------------
    
    if (!(hovered_pos == default_pos && chosen_pos == default_pos)) {
        swap_cell(this->board_game[hovered_pos.y][hovered_pos.x], this->board_game[chosen_pos.y][chosen_pos.x]);
        this->turn = -this->turn;
        return 1;
    }
    return 0;

}