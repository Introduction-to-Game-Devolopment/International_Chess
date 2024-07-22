#ifndef PLAYER_H
#define PLAYER_H

#include "piece.h"

class player {
    protected:
        std::string name;
        // int time;
        int8_t type;
        std::vector<piece> piece_captured;
    public:
        player(): name("Anonymous"), type(1) {}
        player(std::string name, int8_t type): name(name), type(type) {}
        void capture(piece);
        std::vector<piece> get_piece_captured(void);
        
};

#endif