#include "board.h"
#include "general.h"
#include "piece.h"

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
    for (int i = 1; i < 7; i++) {
        for (int j = 0; j < 8; j++) {
            this->board_game[i][j] = cell({i, j}, 0);
        }
    }
    
    // for debuging
    // this->board_game[1][3] = cell({1,3}, -1);
    // this->board_game[3][2] = cell({3,2}, 1);
    // this->board_game[4][4] = cell({4,4}, 6);
    // this->board_game[2][1] = cell({2,1}, -1);
    
    // Quân tốt
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
    for(int i = 1; i < 7; i++) {
        for(int j = 0; j < 8; j++) {
            this->board_game[i][j].draw_cell({0,0,0,0}, 2);
        }
    }
    for(int j = 1; j < 7; j++) {
        this->board_game[0][j].draw_cell({0,0,0,0}, 2);
    }
    for(int j = 1; j < 7; j++) {
        this->board_game[7][j].draw_cell({0,0,0,0}, 2);
    }
    this->board_game[0][0].draw_cell({10,0,0,0}, 2);
    this->board_game[0][7].draw_cell({0,10,0,0}, 2);
    this->board_game[7][7].draw_cell({0,0,10,0}, 2);
    this->board_game[7][0].draw_cell({0,0,0,10}, 2);
    draw_rectangle_with_rounded({PADDING, PADDING, float(this->board_game[0][0].get_len()*8), float(this->board_game[0][0].get_len()*8)}, {0,0,0,0}, {10,10,10,10}, 8, BORDER_COLOR);
}

bool board::is_blocked(XY pos) {
    return this->board_game[pos.y][pos.x].get_type_piece() != 0 && this->board_game[pos.y][pos.x].is_exist_piece();
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
                king_pos = {x, y};
                printf("king at (%d %d)\n", king_pos.y, king_pos.x);
            }
        }
    }
    int pawn_dir = this->turn > 0 ? 4 : 1;
    XY piece_pos;
    for (int capture_dir : {pawn_dir - 1, pawn_dir + 1}) {
        piece_pos = king_pos + PAWN_MOVE[capture_dir];
        if (is_inside(piece_pos) && is_blocked(piece_pos) && this->turn * this->board_game[piece_pos.y][piece_pos.x].get_type_piece() == -1) {
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
        if (is_inside(piece_pos) && is_blocked(piece_pos) && this->turn * this->board_game[piece_pos.y][piece_pos.x].get_type_piece() == -3) {
            printf("knight attack: (%d - %d) @ %d\n", piece_pos.y, piece_pos.x, this->board_game[piece_pos.y][piece_pos.x].get_type_piece());
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
        if (is_inside(piece_pos) && is_blocked(piece_pos) && this->turn * this->board_game[piece_pos.y][piece_pos.x].get_type_piece() == -6) {
            printf("king attack\n");
            return true;
        }
    }
    return false;
}

// bool board::is_valid_move(XY pos, XY next_pos, int move_type) {
//     return true;
//     bool is_valid = true;
//     int16_t old_piece_type[2] = {
//         this->board_game[pos.y][pos.x].get_type_piece(),
//         this->board_game[next_pos.y][next_pos.x].get_type_piece(),
//     };
//     this->board_game[pos.y][pos.x] = cell(pos, 0);
//     this->board_game[next_pos.y][next_pos.x] = cell(next_pos, old_piece_type[0]);
//     switch (move_type) {
//         case 0:
//             if (is_in_check()) is_valid = false;
//             break;
//         case 1:
//             this->board_game[this->en_passant.y][this->en_passant.x] = cell(this->en_passant, 0);
//             if (is_in_check()) is_valid = false;
//             this->board_game[this->en_passant.y][this->en_passant.x] = cell(this->en_passant, -old_piece_type[0]);
//             break;
//         case 2:
//             // XY old_rook_pos = {next_pos.y == 2 ? 0 : 7, next_pos.x};
//             // XY new_rook_pos = {next_pos.y == 2 ? 3 : 5, next_pos.x};
//             // this->board_game[old_rook_pos.y][old_rook_pos.x] = cell(old_rook_pos, 0);
//             // this->board_game[new_rook_pos.y][new_rook_pos.x] = cell(new_rook_pos, this->turn*4);
//             // if (is_in_check()) is_valid = false;
//             // this->board_game[old_rook_pos.y][old_rook_pos.x] = cell(old_rook_pos, this->turn*4);
//             // this->board_game[new_rook_pos.y][new_rook_pos.x] = cell(new_rook_pos, 0);
            
            
//             break;
//     }
//     this->board_game[pos.y][pos.x] = cell(pos, old_piece_type[0]);
//     this->board_game[next_pos.y][next_pos.x] = cell(next_pos, old_piece_type[1]);
//     return is_valid;
// }

bool board::is_valid_move(XY old_pos, XY new_pos, int16_t type) {
    // return true;
    bool is_valid = true;
    bool flag = swap_test(this->board_game[old_pos.y][old_pos.x], this->board_game[new_pos.y][new_pos.x]);
    printf("%d - %d - %d || %d - %d - %d\n", this->board_game[old_pos.y][old_pos.x].get_type_piece(), old_pos.y, old_pos.x, this->board_game[new_pos.y][new_pos.x].get_type_piece(), new_pos.y, new_pos.x);
    printf("-----%d\n", flag);
    switch (type) {
        case 0:
            if (is_in_check()) is_valid = false;
            break;
        case 1:
            if (is_in_check()) is_valid = false;
            break;
        case 2:
            if (is_in_check()) is_valid = false;
            break;
    }
    
    swap_test(this->board_game[old_pos.y][old_pos.x], this->board_game[new_pos.y][new_pos.x], flag);
    return is_valid;
}

std::vector<XY> board::get_move(cell square) {
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
                printf("OK\n");
                for (int dir : {0, 4}) {
                    next_pos = pos;
                    if (!is_blocked(next_pos += KING_MOVE[dir]) && !is_blocked(next_pos += KING_MOVE[dir])) {
                        for (XY rook_pos = next_pos + KING_MOVE[dir]; is_inside(rook_pos); rook_pos += KING_MOVE[dir]) {
                            if (is_blocked(rook_pos)) {
                                printf("rook %d %d %d %d\n", rook_pos.y, rook_pos.x, this->board_game[rook_pos.y][rook_pos.x].get_type_piece(), this->turn);
                                if (!this->board_game[rook_pos.y][rook_pos.x].is_moved_piece() && this->board_game[rook_pos.y][rook_pos.x].get_type_piece() * this->turn == 4) {
                                    next_move.emplace_back(std::make_pair(next_pos, 2));
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
    std::vector<XY> valid_move(0);
    for (std::pair<XY, int> moves : next_move) {
        if (is_valid_move(pos, moves.first, moves.second)) {
            valid_move.emplace_back(moves.first);
        }
    }
    for (XY i : valid_move) printf("valid_move: (%d, %d)\n", i.y, i.x);
    return valid_move;
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

bool board::swap_cell(cell &my_cell, cell &your_cell) {
    if (int(my_cell.get_type_piece())*your_cell.get_type_piece() < 0) {
        your_cell.unexist();
        your_cell.change_piece(my_cell.get_piece());
        my_cell.change_piece(none());
        return true;
    }
    piece tmp = my_cell.get_piece();
    my_cell.change_piece(your_cell.get_piece());
    your_cell.change_piece(tmp);

    printf("PASS\n");
    return false;
}

bool board::swap_test(cell &my_cell, cell &your_cell, bool undo) {
    bool flag = false;
    
    if (int(my_cell.get_type_piece())*your_cell.get_type_piece() < 0) {
        if (!undo) {
            your_cell.unexist();
            flag = true;
        } else {
            my_cell.exist();
        }
    }

    if (abs(my_cell.get_type_piece()) == 1 && abs(my_cell.get_pos().x - your_cell.get_pos().x) == abs(my_cell.get_pos().y - your_cell.get_pos().y) && your_cell.get_type_piece() == 0 && !undo) {
        printf("HEHE (%d, %d) %d", en_passant.y, en_passant.x, this->board_game[this->en_passant.y][this->en_passant.x].is_exist_piece());
        this->board_game[this->en_passant.y][this->en_passant.x].unexist();
        flag = true;
    } 
    if (abs(your_cell.get_type_piece()) == 1 && abs(my_cell.get_pos().x - your_cell.get_pos().x) == abs(my_cell.get_pos().y - your_cell.get_pos().y) && my_cell.get_type_piece() == 0 && undo) {
        printf("PASSSSSSSSSSSSSSSSSSSSSSS\n");
        this->board_game[this->en_passant.y][this->en_passant.x].exist();
    }
    piece tmp = my_cell.get_piece();
    my_cell.change_piece(your_cell.get_piece());
    your_cell.change_piece(tmp);

    printf("PASS\n");
    return flag;
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
    
    if (!(hovered_pos == DEFAULT_POS || chosen_pos == DEFAULT_POS)) {
        this->board_game[chosen_pos.y][chosen_pos.x].moved();
            printf("UNPASS - KING: %d\n", this->board_game[chosen_pos.y][chosen_pos.x].get_type_piece());
        if(abs(this->board_game[chosen_pos.y][chosen_pos.x].get_type_piece()) == 6) {
            if (hovered_pos.x - chosen_pos.x == 2) {
                this->swap_cell(this->board_game[chosen_pos.y][7], this->board_game[chosen_pos.y][5]);
            } else if (hovered_pos.x - chosen_pos.x == -2) {
                this->swap_cell(this->board_game[chosen_pos.y][0], this->board_game[chosen_pos.y][3]);
            }
        }
        if (abs(this->board_game[chosen_pos.y][chosen_pos.x].get_type_piece()) == 1) {
            if (abs(chosen_pos.y - hovered_pos.y) == 2) {
                this->en_passant = hovered_pos;
            } else if (abs(chosen_pos.y - hovered_pos.y) == abs(chosen_pos.x - hovered_pos.x) && !is_blocked(hovered_pos)) {
                this->swap_cell(this->board_game[chosen_pos.y][chosen_pos.x], this->board_game[this->en_passant.y][this->en_passant.x]);
                this->swap_cell(this->board_game[this->en_passant.y][this->en_passant.x], this->board_game[hovered_pos.y][hovered_pos.x]);
                this->turn = -this->turn;
                this->en_passant = DEFAULT_POS;
                return 1;
            } else if (is_promotion(hovered_pos)) {
                this->board_game[chosen_pos.y][chosen_pos.x].change_piece(queen(1, this->turn));
            }
        }
        this->swap_cell(this->board_game[chosen_pos.y][chosen_pos.x], this->board_game[hovered_pos.y][hovered_pos.x]);
        this->turn = -this->turn;
        return 1;
    }
    return 0;
}