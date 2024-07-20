#include "piece.h"

piece init_piece(int16_t type) {
    switch (type) {
        case 1:
            return pawn(1, 1);
            break;
        case 2:
            return bishop(1,1);
            break;
        case 3:
            return knight(1,1);
            break;
        case 4:
            return rook(1, 1);
            break;
        case 5:
            return queen(1,1);
            break;
        case 6:
            return king(1, 1);
            break;

        case -1:
            return pawn(1, -1);
            break;
        case -2:
            return bishop(1, -1);
            break;
        case -3:
            return knight(1, -1);
            break;
        case -4:
            return rook(1, -1);
            break;
        case -5:
            return queen(1, -1);
            break;
        case -6:
            return king(1, -1);
            break;
        default:
            return none();
            break;
    }
}
