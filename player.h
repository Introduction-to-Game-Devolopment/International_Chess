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
        void capture(piece was_captured);
        std::vector<piece> get_piece_captured();
        
};