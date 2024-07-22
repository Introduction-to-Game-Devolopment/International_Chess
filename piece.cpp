#include "piece.h"

piece init_piece(int16_t type, int length) {
    Image image;
    switch (type) {
        case  1: return pawn(1, 1);
        case  2: return bishop(1, 1);
        case  3: return knight(1, 1);
        case  4: return rook(1, 1);
        case  5: return queen(1, 1);
        case  6: return king(1, 1);

        case -1: return pawn(1, -1);
        case -2: return bishop(1, -1);
        case -3: return knight(1, -1);
        case -4: return rook(1, -1);
        case -5: return queen(1, -1);
        case -6: return king(1, -1);
    }
    return none();
}

std::vector<XY> pawn::get_move(XY pos) {
    int dir = this->get_type() > 0 ? 1 : -1;
    std::vector<XY> next_move(0);
    XY new_pos;
    for (int i = 0; i < 3; ++i) {
        new_pos = pos + PAWN_MOVE[i]*dir;
        if (is_inside(new_pos)) {
            next_move.emplace_back(new_pos);
        }
    }
    if (!this->moved) {
        new_pos = pos + PAWN_MOVE[1]*dir*2;
        if (is_inside(new_pos))
        next_move.emplace_back(new_pos);
    }
    return next_move;
}

std::vector<XY> bishop::get_move(XY pos) {
    std::vector<XY> next_move(0);
    XY new_pos;
    for (int i = 0; i < 4; ++i) {
        for (int l = 1; l <= 8; ++l) {
            new_pos = pos + BISHOP_MOVE[i]*l;
            if (!is_inside(new_pos)) break;
            next_move.emplace_back(new_pos);
        }
    }
    return next_move;
}

std::vector<XY> knight::get_move(XY pos) {
    std::vector<XY> next_move(0);
    XY new_pos;
    for (int i = 0; i < 8; ++i) {
        new_pos = pos + KNIGHT_MOVE[i];
        if (is_inside(new_pos)) {
            next_move.emplace_back(new_pos);
        }
    }
    return next_move;
}

std::vector<XY> rook::get_move(XY pos) {
    std::vector<XY> next_move(0);
    XY new_pos;
    for (int i = 0; i < 4; ++i) {
        for (int l = 1; l <= 8; ++l) {
            new_pos = pos + ROOK_MOVE[i]*l;
            if (is_inside(new_pos)) {
                next_move.emplace_back(new_pos);
            }
        }
    }
    return next_move;
}

std::vector<XY> queen::get_move(XY pos) {
    std::vector<XY> next_move(0);
    XY new_pos;
    for (int i = 0; i < 8; ++i) {
        for (int l = 1; l <= 8; ++l) {
            new_pos = pos + QUEEN_MOVE[i]*l;
            if (!is_inside(new_pos)) break;
            next_move.emplace_back(new_pos);
        }
    }
    return next_move;
}

std::vector<XY> king::get_move(XY pos) {
    std::vector<XY> next_move(0);
    XY new_pos;
    for (int i = 0; i < 8; ++i) {
        new_pos = pos + KING_MOVE[i];
        if (is_inside(new_pos)) {
            next_move.emplace_back(new_pos);
        }
    }
    return next_move;
}

bool is_inside(XY pos) {
    return 0 <= pos.x && pos.x < 8 && 0 <= pos.y && pos.y < 8;
}