#include "board.h"

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

    this->en_passant = {-1, -1};
}

void board::draw_board() {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            this->board_game[i][j].draw_cell();
        }
    }
}

bool board::is_blocked(XY pos) {
    return this->board_game[pos.y][pos.x].get_type_piece() != 0;
}

bool board::is_captured(XY pos) {
    return this->turn * this->board_game[pos.y][pos.x].get_type_piece() < 0;
}

bool board::is_en_passant(XY pos) {
    return !is_blocked(pos) && pos + PAWN_MOVE[this->turn > 0 ? 1 : 4] == this->en_passant;
}

bool board::is_in_check() {
    XY king_pos;
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (this->turn * this->board_game[x][y].get_type_piece() == 6) {
                king_pos = {y, x};
                printf("king at (%d %d)\n", king_pos.y, king_pos.x);
            }
        }
    }
    int pawn_dir = this->turn > 0 ? 1 : 4;
    XY piece_pos;
    for (int capture_dir : {pawn_dir - 1, pawn_dir + 1}) {
        piece_pos = king_pos + PAWN_MOVE[capture_dir];
        if (is_inside(piece_pos) && this->turn * this->board_game[piece_pos.y][piece_pos.x].get_type_piece() == -1) {
            printf("pawn attack\n");
            return true;
        }
    }
    for (int dir = sizeof(BISHOP_MOVE) / sizeof(BISHOP_MOVE[0]) - 1; dir >= 0; --dir) {
        piece_pos = king_pos;
        while (is_inside(piece_pos += BISHOP_MOVE[dir])) {
            if (is_blocked(piece_pos)) {
                int16_t piece_type = this->turn * this->board_game[piece_pos.y][piece_pos.x].get_type_piece();
                if (piece_type == -2 || piece_type == -5) {
                    printf("%s attack\n", piece_type == -2 ? "bishop" : "queen");
                    return true;
                }
                printf("bishop_dir/piece at (%d %d): type=%d %d\n", piece_pos.x, piece_pos.y, piece_type, is_blocked(piece_pos));
                break;
            }
        }
    }
    for (int dir = sizeof(KNIGHT_MOVE) / sizeof(KNIGHT_MOVE[0]) - 1; dir >= 0; --dir) {
        piece_pos = king_pos + KNIGHT_MOVE[dir];
        if (is_inside(piece_pos) && this->turn * this->board_game[piece_pos.y][piece_pos.x].get_type_piece() == -3) {
            printf("knight attack\n");
            return true;
        }
    }
    for (int dir = sizeof(ROOK_MOVE) / sizeof(ROOK_MOVE[0]) - 1; dir >= 0; --dir) {
        piece_pos = king_pos;
        while (is_inside(piece_pos += ROOK_MOVE[dir])) {
            if (is_blocked(piece_pos)) {
                int16_t piece_type = this->turn * this->board_game[piece_pos.y][piece_pos.x].get_type_piece();
                if (piece_type == -4 || piece_type == -5) {
                    printf("%s attack\n", piece_type == -4 ? "rook" : "queen");
                    return true;
                }
                printf("rook_dir/piece at (%d %d): type=%d\n", piece_pos.x, piece_pos.y, piece_type);
                break;
            }
            else {
                printf("rook_dir/empty at (%d %d): type=%d\n", piece_pos.x, piece_pos.y, this->board_game[piece_pos.y][piece_pos.x].get_type_piece());
            }
        }
    }
    for (int dir = sizeof(KING_MOVE) / sizeof(KING_MOVE[0]) - 1; dir >= 0; --dir) {
        piece_pos = king_pos + KING_MOVE[dir];
        if (is_inside(piece_pos) && this->turn * this->board_game[piece_pos.y][piece_pos.x].get_type_piece() == -6) {
            printf("king attack\n");
            return true;
        }
    }
    return false;
}

bool board::is_valid_move(XY pos, XY next_pos, int move_type) {
    bool is_valid = true;
    int16_t old_piece_type[2] = {
        this->board_game[pos.y][pos.x].get_type_piece(),
        this->board_game[next_pos.y][next_pos.x].get_type_piece(),
    };
    this->board_game[pos.y][pos.x] = cell(pos, 0);
    this->board_game[next_pos.y][next_pos.x] = cell(next_pos, old_piece_type[0]);
    switch (move_type) {
        case 0:
            if (is_in_check()) is_valid = false;
            break;
        case 1:
            this->board_game[this->en_passant.y][this->en_passant.x] = cell(this->en_passant, 0);
            if (is_in_check()) is_valid = false;
            this->board_game[this->en_passant.y][this->en_passant.x] = cell(this->en_passant, -old_piece_type[0]);
            break;
        case 2:
            XY old_rook_pos = {next_pos.y == 2 ? 0 : 7, next_pos.x};
            XY new_rook_pos = {next_pos.y == 2 ? 3 : 5, next_pos.x};
            this->board_game[old_rook_pos.y][old_rook_pos.x] = cell(old_rook_pos, 0);
            this->board_game[new_rook_pos.y][new_rook_pos.x] = cell(new_rook_pos, this->turn*4);
            if (is_in_check()) is_valid = false;
            this->board_game[old_rook_pos.y][old_rook_pos.x] = cell(old_rook_pos, this->turn*4);
            this->board_game[new_rook_pos.y][new_rook_pos.x] = cell(new_rook_pos, 0);
            break;
    }
    this->board_game[pos.y][pos.x] = cell(pos, old_piece_type[0]);
    this->board_game[next_pos.y][next_pos.x] = cell(next_pos, old_piece_type[1]);
    return is_valid;
}

std::vector<XY> board::get_move(cell square) {
    if (!square.is_exist_piece()) return std::vector<XY>(0);

    std::vector<XY> next_move(0);
    XY pos = square.get_pos();
    XY next_pos;
    int dir;
    int piece_type = square.get_type_piece() * this->turn;
    switch (piece_type) {
        case 1:
            dir = this->turn > 0 ? 4 : 1;
            next_pos = pos + PAWN_MOVE[dir];
            if (is_inside(next_pos) && !is_blocked(next_pos)) {
                next_move.emplace_back(next_pos);
                if (!square.is_moved_piece() && is_inside(next_pos += PAWN_MOVE[dir]) && !is_blocked(next_pos)) {
                    next_move.emplace_back(next_pos);
                }
            }
            for (int capture_dir : {dir - 1, dir + 1}) {
                next_pos = pos + PAWN_MOVE[capture_dir];
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
            for (int dir = sizeof(BISHOP_MOVE) / sizeof(BISHOP_MOVE[0]) - 1; dir >= 0; --dir) {
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
            for (int dir = sizeof(KNIGHT_MOVE) / sizeof(KNIGHT_MOVE[0]) - 1; dir >= 0; --dir) {
                next_pos = pos + KNIGHT_MOVE[dir];
                if (is_inside(next_pos)) {
                    if (!is_blocked(next_pos) || is_captured(next_pos)) {
                        next_move.emplace_back(next_pos);
                    }
                }
            }
            break;
        case 4: 
            for (int dir = sizeof(ROOK_MOVE) / sizeof(ROOK_MOVE[0]) - 1; dir >= 0; --dir) {
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
            for (int dir = sizeof(QUEEN_MOVE) / sizeof(QUEEN_MOVE[0]) - 1; dir >= 0; --dir) {
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
            for (int dir = sizeof(KING_MOVE) / sizeof(KING_MOVE[0]) - 1; dir >= 0; --dir)  {
                next_pos = pos + KING_MOVE[dir];
                if (is_inside(next_pos)) {
                    if (is_blocked(next_pos)) {
                        if (is_captured(next_pos)) {
                            next_move.emplace_back(next_pos);
                        }
                    }
                    else {
                        next_move.emplace_back(next_pos);
                    }
                }
            }
            if (!square.is_moved_piece() && !is_in_check()) {
                printf("OK\n");
                for (int dir : {0, 4}) {
                    next_pos = pos;
                    if (!is_blocked(next_pos += KING_MOVE[dir]) && !is_blocked(next_pos += KING_MOVE[dir])) {
                        for (XY rook_pos = next_pos + KING_MOVE[dir]; is_inside(rook_pos); rook_pos += KING_MOVE[dir]) {
                            if (is_blocked(rook_pos)) {
                                printf("rook %d %d %d %d\n", rook_pos.y, rook_pos.x, this->board_game[rook_pos.y][rook_pos.x].get_type_piece(), this->turn);
                                if (!this->board_game[rook_pos.y][rook_pos.x].is_moved_piece() && this->board_game[rook_pos.y][rook_pos.x].get_type_piece() * this->turn == 4) {
                                    next_move.emplace_back(next_pos);
                                }
                                break;
                            }
                            else {
                                printf("empty %d %d\n", rook_pos.y, rook_pos.x);
                            }
                        }
                    }
                }
            }
            break;
    }
    /*
    std::vector<XY> valid_move(0);
    for (XY new_pos : next_move) {
        if (is_valid_move(pos, new_pos, piece_type)) {
            valid_move.emplace_back(new_pos);
        }
    }
    return valid_move;
    */
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
    
    if (!(hovered_pos == default_pos || chosen_pos == default_pos)) {
        swap_cell(this->board_game[hovered_pos.y][hovered_pos.x], this->board_game[chosen_pos.y][chosen_pos.x]);
        this->turn = -this->turn;
        return 1;
    }
    return 0;

}