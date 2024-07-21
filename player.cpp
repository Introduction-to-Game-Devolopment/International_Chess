#include "player.h"
#include <vector>

// Ăn quân cờ đối phương
void player::capture(piece was_kill){
    this->piece_captured.push_back(was_kill);
}

// Lấy số quân cờ đã ăn
std::vector<piece> player::get_piece_captured(){
    return this->piece_captured;
}
