#include "piece.h"

piece init_piece(int8_t type) {
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

void piece::exist() {
    this->is_exist = true;
}

void piece::unexist() {
    this->is_exist = false;
}

void piece::moved() {
    this->was_moved = true;
}

