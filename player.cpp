#include "player.h"

// Ăn quân cờ đối phương
void player::kill(piece was_kill){
    this->piece_killed.push_back(was_kill);
}

// Lấy số quân cờ đã ăn
std::vector<piece> player::get_piece_kill(){
    return this->piece_killed;
}
