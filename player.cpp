#include "player.h"

// Capture opponent's piece
void player::capture(piece was_captured) {
    this->piece_captured.push_back(was_captured);
}

// Get pieces that captured
std::vector<piece> player::get_piece_captured(void) {
    return this->piece_captured;
}
