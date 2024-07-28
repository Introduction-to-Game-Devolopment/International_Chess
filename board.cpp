#include "board.h"

// Constructor for class cell
cell::cell(XY pos, int8_t type, size frame_size, int padding) {
    this->Piece = init_piece(type);
    this->pos = pos;
    this->length = (frame_size.height - 2*padding)/8;
    this->is_hover = false;
    this->is_chosen = false;
    point position = {padding + pos.x*length, padding + pos.y*length};
    this->rec = {position.x, position.y, this->length, this->length};
}

void cell::draw_cell(border radius, int border_width, Color border_color) {
    Color color;
    if (this->is_chosen)
        color = CHOSEN_CELL_COLOR;
    else if (this->is_hover)
        color = HOVERED_CELL_COLOR;
    else if ((this->pos.x + this->pos.y) & 1) {
        color = ODD_CELL_COLOR;
    } else color = EVEN_CELL_COLOR;
    draw_rectangle_with_rounded(this->rec, color, radius, border_width, border_color);
    if (this->Piece.get_type() == 0 || this->Piece.get_is_exist() == 0) return;
    draw_picture(this->Piece.get_texture(), this->rec);
}

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
    if (mouse_pos < this->rec && this->Piece.get_type() * turn > 0){
        this->is_chosen = true;
        return;
    }
    this->is_chosen = false;
}

void cell::unchoose() {
    this->is_chosen = false;
}

boardchess::boardchess(const std::string white_player, const std::string black_player) {
    this->white_player = player(white_player, 1);
    this->black_player = player(black_player, -1);
    this->turn = 1;
    this->en_passant = DEFAULT_POS;
    this->moving = TextBox({FRAME_SIZE.height + PADDING*3/2, (FRAME_SIZE.height - 450)/2, FRAME_SIZE.width - 3*PADDING - FRAME_SIZE.height , 450}, "");
    
    // Nonexistance
    for (int i = 1; i < 7; i++) {
        for (int j = 0; j < 8; j++) {
            this->board[i][j] = cell({i, j}, 0);
        }
    }
    
    // for debuging
    // this->board[1][3] = cell({1,3}, -1);
    // this->board[3][2] = cell({3,2}, 1);
    // this->board[4][4] = cell({4,4}, 6);
    // this->board[2][1] = cell({2,1}, -1);
    
    // for debuging
    // this->board[0][0] = cell({0,0}, -6);
    // this->board[2][7] = cell({2,3}, 6);
    // this->board[7][1] = cell({7,1}, 5);
    
    // Quân tốt
    for (int j = 0; j < 8; j++) {
        this->board[1][j] = cell({1,j}, -1);
        this->board[6][j] = cell({6,j}, 1);
    }

    // Quân tượng
    this->board[0][2] = cell({0,2}, -2);
    this->board[0][5] = cell({0,5}, -2);
    this->board[7][2] = cell({7,2}, 2);
    this->board[7][5] = cell({7,5}, 2);

    // Quân mã
    this->board[0][1] = cell({0,1}, -3);
    this->board[0][6] = cell({0,6}, -3);
    this->board[7][1] = cell({7,1}, 3);
    this->board[7][6] = cell({7,6}, 3);

    // Quân xe
    this->board[0][0] = cell({0,0}, -4);
    this->board[0][7] = cell({0,7}, -4);
    this->board[7][0] = cell({7,0}, 4);
    this->board[7][7] = cell({7,7}, 4);

    // Quân hậu
    this->board[0][3] = cell({0,3}, -5);
    this->board[7][3] = cell({7,3}, 5);

    // Quân Vua
    this->board[0][4] = cell({0,4}, -6);
    this->board[7][4] = cell({7,4}, 6);
    
}

void boardchess::draw_board() {
    for(int i = 1; i < 7; i++) {
        for(int j = 0; j < 8; j++) {
            this->board[i][j].draw_cell({0,0,0,0}, 2);
        }
    }
    for(int j = 1; j < 7; j++) {
        this->board[0][j].draw_cell({0,0,0,0}, 2);
    }
    for(int j = 1; j < 7; j++) {
        this->board[7][j].draw_cell({0,0,0,0}, 2);
    }
    this->board[0][0].draw_cell({10,0,0,0}, 2);
    this->board[0][7].draw_cell({0,10,0,0}, 2);
    this->board[7][7].draw_cell({0,0,10,0}, 2);
    this->board[7][0].draw_cell({0,0,0,10}, 2);
    
    draw_rectangle_with_border({PADDING, PADDING, this->board[0][0].get_len()*8, this->board[0][0].get_len()*8}, {0,0,0,0}, 24, BORDER_COLOR, 10);
    this->moving.update();
    this->moving.draw();
}

bool boardchess::is_blocked(XY pos) {
    return this->board[pos.y][pos.x].get_type_piece() != 0 && this->board[pos.y][pos.x].is_exist_piece();
}

bool boardchess::is_captured(XY pos) {
    return this->board[pos.y][pos.x].get_type_piece() * this->turn < 0;
}

bool boardchess::is_en_passant(XY pos) {
    return this->board[pos.y][pos.x].get_type_piece() == 0 && pos + PAWN_MOVE[this->turn > 0 ? 1 : 4] == this->en_passant;
}

bool boardchess::is_in_check() {
    XY king_pos;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (this->turn * this->board[y][x].get_type_piece() == 6) {
                king_pos = {y, x};
                // printf("is_in_check: king at (%d %d)\n", king_pos.y, king_pos.x);   // for debuging
            }
        }
    }
    int pawn_dir = this->turn > 0 ? 4 : 1;
    XY piece_pos;
    for (int capture_dir : {pawn_dir - 1, pawn_dir + 1}) {
        piece_pos = king_pos + PAWN_MOVE[capture_dir];
        if (is_inside(piece_pos) && is_blocked(piece_pos) && this->turn * this->board[piece_pos.y][piece_pos.x].get_type_piece() == -1) {
            // printf("is_in_check: pawn at (%d, %d) attack\n", piece_pos.y, piece_pos.x);   // for debuging
            return true;
        }
    }
    for (int dir = sizeof(BISHOP_MOVE) / sizeof(BISHOP_MOVE[0]) - 1; dir >= 0; --dir) {
        piece_pos = king_pos;
        while (is_inside(piece_pos += BISHOP_MOVE[dir])) {
            if (is_blocked(piece_pos)) {
                int8_t piece_type = this->turn * this->board[piece_pos.y][piece_pos.x].get_type_piece();
                if (piece_type == -2 || piece_type == -5) {
                    // printf("is_in_check: %s at (%d, %d) attack\n", piece_type == -2 ? "bishop" : "queen", piece_pos.y, piece_pos.x); //for debuging
                    return true;
                }
                // printf("is_in_check: bishop_dir/piece at (%d %d): type=%d - %d\n", piece_pos.x, piece_pos.y, piece_type, is_blocked(piece_pos)); //for debuging
                break;
            }
        }
    }
    for (int dir = sizeof(KNIGHT_MOVE) / sizeof(KNIGHT_MOVE[0]) - 1; dir >= 0; --dir) {
        piece_pos = king_pos + KNIGHT_MOVE[dir];
        if (is_inside(piece_pos) && is_blocked(piece_pos) && this->turn * this->board[piece_pos.y][piece_pos.x].get_type_piece() == -3) {
            // printf("is_in_check: knight at (%d - %d) attack: type=%d\n", piece_pos.y, piece_pos.x, this->board[piece_pos.y][piece_pos.x].get_type_piece()); // for debuging
            return true;
        }
    }
    for (int dir = sizeof(ROOK_MOVE) / sizeof(ROOK_MOVE[0]) - 1; dir >= 0; --dir) {
        piece_pos = king_pos;
        while (is_inside(piece_pos += ROOK_MOVE[dir])) {
            if (is_blocked(piece_pos)) {
                int8_t piece_type = this->turn * this->board[piece_pos.y][piece_pos.x].get_type_piece();
                if (piece_type == -4 || piece_type == -5) {
                    // printf("is_in_check: %s at (%d, %d) attack\n", piece_type == -4 ? "rook" : "queen", piece_pos.y, piece_pos.x);  // for debuging
                    return true;
                }
                // printf("is_in_check: rook_dir/piece at (%d %d): type=%d\n", piece_pos.x, piece_pos.y, piece_type);   // for debuging 
                break;
            }
            else {
                // printf("is_in_check: rook_dir/empty at (%d %d): type=%d\n", piece_pos.x, piece_pos.y, this->board[piece_pos.y][piece_pos.x].get_type_piece());  //for debuging
            }
        }
    }
    for (int dir = sizeof(KING_MOVE) / sizeof(KING_MOVE[0]) - 1; dir >= 0; --dir) {
        piece_pos = king_pos + KING_MOVE[dir];
        if (is_inside(piece_pos) && is_blocked(piece_pos) && this->turn * this->board[piece_pos.y][piece_pos.x].get_type_piece() == -6) {
            // printf("is_in_check: king at (%d, %d) attack\n", piece_pos.y, piece_pos.x);
            return true;
        }
    }
    return false;
}

bool boardchess::is_valid_move(XY old_pos, XY new_pos, int8_t type) {
    // return true;
    bool is_valid = true;
    bool flag = swap_test(this->board[old_pos.y][old_pos.x], this->board[new_pos.y][new_pos.x]);
    // printf("is_valid_move: old_pos: type=%d, (%d - %d) || new_pos: type=%d, (%d - %d)\n", this->board[old_pos.y][old_pos.x].get_type_piece(), old_pos.y, old_pos.x, this->board[new_pos.y][new_pos.x].get_type_piece(), new_pos.y, new_pos.x); //for debuging
    if (is_in_check()) is_valid = false;    
    // printf("is_valid_move: flag=%d, is_attacked=%d\n", flag, is_valid);   //for debuging
    swap_test(this->board[old_pos.y][old_pos.x], this->board[new_pos.y][new_pos.x], flag);
    return is_valid;
}

std::vector<XY> boardchess::get_move(cell square) {
    if (!square.is_exist_piece()) return std::vector<XY>(0);
    
    std::vector<std::pair<XY, int>> next_move(0);
    XY pos = square.get_pos();
    XY next_pos;
    int pawn_dir;
    int piece_type = square.get_type_piece() * this->turn;
    
    switch (piece_type) {
        case 1:
            pawn_dir = this->turn > 0 ? 4 : 1;
            next_pos = pos + PAWN_MOVE[pawn_dir];
            if (is_inside(next_pos) && !is_blocked(next_pos)) {
                next_move.emplace_back(std::make_pair(next_pos, 0));
                if (!square.is_moved_piece() && is_inside(next_pos += PAWN_MOVE[pawn_dir]) && !is_blocked(next_pos)) {
                    next_move.emplace_back(std::make_pair(next_pos, 0));
                }
            }
            for (int capture_dir : {pawn_dir - 1, pawn_dir + 1}) {
                next_pos = pos + PAWN_MOVE[capture_dir];
                if (is_inside(next_pos)) {
                    if (is_captured(next_pos)) {
                        next_move.emplace_back(std::make_pair(next_pos, 0));
                    }
                    if (is_en_passant(next_pos)) {
                        next_move.emplace_back(std::make_pair(next_pos, 1));
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
                            next_move.emplace_back(std::make_pair(next_pos, 0));
                        }
                        break;
                    }
                    else {
                        next_move.emplace_back(std::make_pair(next_pos, 0));
                    }
                }
            }
            break;
        case 3:
            for (int dir = sizeof(KNIGHT_MOVE) / sizeof(KNIGHT_MOVE[0]) - 1; dir >= 0; --dir) {
                next_pos = pos + KNIGHT_MOVE[dir];
                if (is_inside(next_pos)) {
                    if (!is_blocked(next_pos) || is_captured(next_pos)) {
                        next_move.emplace_back(std::make_pair(next_pos, 0));
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
                                next_move.emplace_back(std::make_pair(next_pos, 0));
                        }
                        break;
                    }
                    else {
                        next_move.emplace_back(std::make_pair(next_pos, 0));
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
                            next_move.emplace_back(std::make_pair(next_pos, 0));
                        }
                        break;
                    }
                    else {
                        next_move.emplace_back(std::make_pair(next_pos, 0));
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
                            next_move.emplace_back(std::make_pair(next_pos, 0));
                        }
                    }
                    else {
                        next_move.emplace_back(std::make_pair(next_pos, 0));
                    }
                }
            }
            if (!square.is_moved_piece() && !is_in_check()) {
                // printf("get_move - case 6: OK\n");  // for debuging 
                for (int dir : {0, 4}) {
                    next_pos = pos;
                    if (!is_blocked(next_pos += KING_MOVE[dir]) && !is_blocked(next_pos += KING_MOVE[dir])) {
                        for (XY rook_pos = next_pos + KING_MOVE[dir]; is_inside(rook_pos); rook_pos += KING_MOVE[dir]) {
                            if (is_blocked(rook_pos)) {
                                // printf("get_move - case 6: rook at (%d %d), type=%d, turn=%d\n", rook_pos.y, rook_pos.x, this->board[rook_pos.y][rook_pos.x].get_type_piece(), this->turn); // for debuging
                                if (!this->board[rook_pos.y][rook_pos.x].is_moved_piece() && this->board[rook_pos.y][rook_pos.x].get_type_piece() * this->turn == 4) {
                                    next_move.emplace_back(std::make_pair(next_pos, 2));
                                }
                                break;
                            }
                            else {
                                // printf("get_move - case 6: empty at (%d %d)\n", rook_pos.y, rook_pos.x);    // for debuging
                            }
                        }
                    }
                }
            }
            break;
    }
    std::vector<XY> valid_move(0);
    for (std::pair<XY, int> moves : next_move) {
        if (is_valid_move(pos, moves.first, moves.second)) {
            valid_move.emplace_back(moves.first);
        }
    }
    // for (XY i : valid_move) printf("valid_move: (%d, %d)\n", i.y, i.x); // for debuging 
    return valid_move;
}

void boardchess::wait_for_event(Vector2 mouse_pos) {
    std::vector<XY> next_move;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            this->board[i][j].choose(this->turn, mouse_pos);
            this->board[i][j].unhover();
            if (this->board[i][j].get_chosen()) {
                next_move = get_move(this->board[i][j]);
                // if (next_move.empty()) printf("wait_for_event: chosen return %d at (%d, %d)" , int(this->board[i][j].get_chosen()), i, j); // for debuging
            }
        }
    }

    for (std::vector<XY>::size_type i = 0; i < next_move.size(); i++) {
        int X = next_move[i].x, Y = next_move[i].y;
        this->board[Y][X].hover();
    }
}

bool boardchess::swap_cell(cell &my_cell, cell &your_cell, bool is_special) {
    if (is_special) save_move(my_cell.get_pos(), your_cell.get_pos(), 1); else save_move(my_cell.get_pos(), your_cell.get_pos(), 0);
    if (int(my_cell.get_type_piece())*your_cell.get_type_piece() < 0) {
        your_cell.unexist();
        your_cell.change_piece(my_cell.get_piece());
        my_cell.change_piece(none());
        if (this->turn > 0) {
            white_player.capture(your_cell.get_piece());
        } else {
            black_player.capture(your_cell.get_piece());
        }
        return true;
    }
    piece tmp = my_cell.get_piece();
    my_cell.change_piece(your_cell.get_piece());
    your_cell.change_piece(tmp);
    // printf("swap_cell: capture successfully\n");    // for debuging
    return false;
}

bool boardchess::swap_test(cell &my_cell, cell &your_cell, bool undo) {
    bool flag = false;
    
    if (int(my_cell.get_type_piece())*your_cell.get_type_piece() < 0) {
        if (!undo) {
            your_cell.unexist();
            flag = true;
        } else {
            my_cell.exist();
        }
    }
    // case 1
    if (abs(my_cell.get_type_piece()) == 1 && abs(my_cell.get_pos().x - your_cell.get_pos().x) == abs(my_cell.get_pos().y - your_cell.get_pos().y) && your_cell.get_type_piece() == 0 && !undo) {
        // printf("swap_test - case 1: (%d, %d) - %d\n", en_passant.y, en_passant.x, this->board[this->en_passant.y][this->en_passant.x].is_exist_piece()); // for debuging
        this->board[this->en_passant.y][this->en_passant.x].unexist();
        flag = true;
    } 
    // case 2
    if (abs(your_cell.get_type_piece()) == 1 && abs(my_cell.get_pos().x - your_cell.get_pos().x) == abs(my_cell.get_pos().y - your_cell.get_pos().y) && my_cell.get_type_piece() == 0 && undo) {
        // printf("swap_test - case 2: PASS\n");   // for debuging
        this->board[this->en_passant.y][this->en_passant.x].exist();
    }
    piece tmp = my_cell.get_piece();
    my_cell.change_piece(your_cell.get_piece());
    your_cell.change_piece(tmp);

    return flag;
}

bool boardchess::make_move(Vector2 mouse_pos) {
    XY hovered_pos = {-1, -1}, chosen_pos = {-1, -1};
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (mouse_pos < this->board[i][j].get_rect()) {
                if (this->board[i][j].get_hover()) {
                    hovered_pos = {i, j};
                }
            }
            if (this->board[i][j].get_chosen()) {
                chosen_pos = {i, j};
            }
            this->board[i][j].unchoose();
            this->board[i][j].unhover();
        }
    }
    
    printf("make_move: hovered_pos: (%d - %d):%d | chosen_pos (%d - %d):%d\n", hovered_pos.y, hovered_pos.x, this->board[hovered_pos.y][hovered_pos.x].get_type_piece(), chosen_pos.y, chosen_pos.x, this->board[chosen_pos.y][chosen_pos.x].get_type_piece()); // for debuging
    printf("%d-%d\n", this->en_passant.y, this->en_passant.x);
    if (!(hovered_pos == DEFAULT_POS || chosen_pos == DEFAULT_POS)) {
            this->board[chosen_pos.y][chosen_pos.x].moved();
            switch (abs(this->board[chosen_pos.y][chosen_pos.x].get_type_piece())) {
                case 1:
                printf("------\n");
                
                    if (is_en_passant(hovered_pos)) {
                        swap_cell(this->board[chosen_pos.y][chosen_pos.x], this->board[this->en_passant.y][this->en_passant.x]);
                        swap_cell(this->board[this->en_passant.y][this->en_passant.x], this->board[hovered_pos.y][hovered_pos.x]);
                        this->turn = -this->turn;
                        this->en_passant = DEFAULT_POS;
                        is_end_match();
                        return 1;
                        
                    }
                    if (abs(chosen_pos.y - hovered_pos.y) == 2) {
                        printf("----3--\n");
                        this->en_passant = hovered_pos;
                        swap_cell(this->board[chosen_pos.y][chosen_pos.x], this->board[hovered_pos.y][hovered_pos.x]);
                        this->turn = -this->turn;
                        is_end_match();
                        return 1;
                    }
                    if (is_promotion(hovered_pos)) {
                        this->board[chosen_pos.y][chosen_pos.x].change_piece(queen(1, this->turn));
                    }
                break;
                case 6:
                    if (hovered_pos.x - chosen_pos.x == 2) {
                        swap_cell(this->board[chosen_pos.y][7], this->board[chosen_pos.y][5]);
                    } else if (hovered_pos.x - chosen_pos.x == -2) {
                        swap_cell(this->board[chosen_pos.y][0], this->board[chosen_pos.y][3]);
                    }
                break;
            }
            // this->en_passant = DEFAULT_POS;
            swap_cell(this->board[chosen_pos.y][chosen_pos.x], this->board[hovered_pos.y][hovered_pos.x]);
            this->turn = -this->turn;
            is_end_match();
            return 1;
        }
    return 0;
}

int8_t boardchess::is_end_match() {
    int8_t flag = 2;
    flag = is_in_check();
    for(int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if(this->board[i][j].get_type_piece() * this->turn > 0) {
                if(abs(this->board[i][j].get_type_piece())) {
                    std::vector<XY>next_move = get_move(this->board[i][j]);
                    if (!next_move.empty()) {
                        flag = 2;
                        return flag;
                    }
                }
            }
        }
    }
    // printf("is_end_match: is_in_check=%d, flag=%d, turn=%d\n", is_in_check(), flag, this->turn);
    flag *= -this->turn;
    return flag;
}

void boardchess::end_match(int8_t turn) {
    // printf("\n%d\n", turn);
    if(turn) {
        printf("PLAYER %s WIN", turn>0?this->white_player.get_name().c_str():this->black_player.get_name().c_str());
    } else {
        printf("DRAW\n");
    }
}

void boardchess::save_move(XY src, XY dest, bool is_special_move) {
    this->data.push_back({src, this->board[src.y][src.x].get_type_piece(), dest, this->board[src.y][dest.x].get_type_piece()});
    this->moving.update_data({src, this->board[src.y][src.x].get_type_piece(), dest, this->board[src.y][dest.x].get_type_piece()}, is_special_move);
}
