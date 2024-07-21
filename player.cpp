#include "player.h"

// Ăn quân cờ đối phương
void player::capture(piece captured){
    this->piece_captured.push_back(captured);
}

// Lấy số quân cờ đã ăn
std::vector<piece> player::get_piece_captured(){
    return this->piece_captured;
}
