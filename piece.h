#include "general.h"
#include "raylib/src/raylib.h"
#include <cstdint>

class piece {
    protected:
        bool is_exist;
        int16_t type;
        Texture2D texture;
    public:
        piece(): is_exist(0), type(0) {}
        piece(bool is_exist, int16_t type, Texture2D texture): is_exist(is_exist), type(type), texture(texture) {}
        int16_t get_type(){
            return this->type; 
        }
        Texture2D get_texture() {
            return this->texture;
        };
        void change_exist();
        virtual ~piece() = default; // Destructor ảo để lớp dẫn xuất có thể dọn dẹp tài nguyên nếu 
};

class pawn : public piece {
    public:
        pawn() : piece(0, 1, WHITE_PAWN) {};
        pawn(bool is_exist, int16_t type) : piece(is_exist, type*1, type>0?WHITE_PAWN:BLACK_PAWN) {}
};      // Quân tốt
class bishop : public piece {
    public:
        bishop() : piece(0, 1, WHITE_BISHOP) {};
        bishop(bool is_exist, int16_t type) : piece(is_exist, type*2, type>0?WHITE_BISHOP:BLACK_BISHOP) {}
};    // Quân tượng
class knight : public piece {
    public:
        knight() : piece(0, 1, WHITE_KNIGHT) {};
        knight(bool is_exist, int16_t type) : piece(is_exist, type*3, type>0?WHITE_KNIGHT:BLACK_KNIGHT) {}
};    // Quân Mã
class rook : public piece {
    public:
        rook() : piece(0, 1, WHITE_ROOK) {};
        rook(bool is_exist, int16_t type) : piece(is_exist, type*4, type>0?WHITE_ROOK:BLACK_ROOK) {}
};      // Quân xe
class queen : public piece {
    public:
        queen() : piece(0, 1, WHITE_QUEEN) {};
        queen(bool is_exist, int16_t type) : piece(is_exist, type*5, type>0?WHITE_QUEEN:BLACK_QUEEN) {}
};     // Quân hậu
class king : public piece {
    public:
        king() : piece(0, 1, WHITE_KING) {};
        king(bool is_exist, int16_t type) : piece(is_exist, type*6, type>0?WHITE_KING:BLACK_KING) {}
};      // Quân vua
class none : public piece {
    public:
        none(): piece() {}
};  // Không có quân

piece init_piece(int16_t type);