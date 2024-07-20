#include "piece.h"

class player {
    protected:
        std::string name;
        // int time;
        int8_t type;
        std::vector<piece> piece_killed;
    public:
        player(): name("Anonymous"), type(1) {}
        player(std::string name, int8_t type): name(name), type(type) {}
        void kill(piece was_kill);
        std::vector<piece> get_piece_kill();
        
};