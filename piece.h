#ifndef PIECE_H
#define PIECE_H

#include "general.h"

class piece {
    protected:
        bool is_exist;
        int16_t type;
        Texture2D texture;
        bool was_moved;
    public:
        piece(): is_exist(false), type(0), was_moved(false) {}
        piece(bool is_exist, int16_t type, Texture2D texture): is_exist(is_exist), type(type), texture(texture), was_moved(false) {}
        int16_t get_type(void) { return this->type; }
        Texture2D get_texture(void) { return this->texture; }
        void exist();
        void unexist();
        void change_exist() {return this->is_exist?this->unexist():this->exist();}
        void moved();

        virtual ~piece() = default; // Destructor ảo để lớp dẫn xuất có thể dọn dẹp tài nguyên nếu 
        
        bool get_is_exist(void) { return this->is_exist; }
        bool get_was_moved(void) { return this->was_moved; }
};

class pawn: public piece {
    public:
        pawn(): piece() {}
        pawn(bool is_exist, int16_t type): piece(is_exist, type*1, type>0?WHITE_PAWN:BLACK_PAWN) {}
};      // Quân tốt
class bishop: public piece {
    public:
        bishop(): piece() {};
        bishop(bool is_exist, int16_t type): piece(is_exist, type*2, type>0?WHITE_BISHOP:BLACK_BISHOP) {}
};    // Quân tượng
class knight: public piece {
    public:
        knight(): piece() {};
        knight(bool is_exist, int16_t type): piece(is_exist, type*3, type>0?WHITE_KNIGHT:BLACK_KNIGHT) {}
};    // Quân Mã
class rook: public piece {
    public:
        rook(): piece() {};
        rook(bool is_exist, int16_t type): piece(is_exist, type*4, type>0?WHITE_ROOK:BLACK_ROOK) {}
};      // Quân xe
class queen: public piece {
    public:
        queen(): piece() {};
        queen(bool is_exist, int16_t type): piece(is_exist, type*5, type>0?WHITE_QUEEN:BLACK_QUEEN) {}
};     // Quân hậu
class king: public piece {
    public:
        king(): piece() {};
        king(bool is_exist, int8_t type): piece(is_exist, type*6, type>0?WHITE_KING:BLACK_KING) {}
};      // Quân vua
class none: public piece {
    public:
        none(): piece() {}
};  // Không có quân

piece init_piece(int16_t);

#endif
