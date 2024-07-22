#ifndef PIECE_H
#define PIECE_H

#include "general.h"

class piece {
    protected:
        bool is_exist;
        int16_t type;
        Texture2D texture;
    public:
        piece(): is_exist(0), type(0) {}
        piece(bool is_exist, int16_t type, Texture2D texture): is_exist(is_exist), type(type), texture(texture) {}
        int16_t get_type() { return this->type; }
        Texture2D get_texture() { return this->texture; }
        void change_exist();

        virtual ~piece() = default; // Destructor ảo để lớp dẫn xuất có thể dọn dẹp tài nguyên nếu 
        virtual std::vector<XY> get_move() { return std::vector<XY>(0); }
};

class pawn: public piece {
    protected:
        bool en_passant;
        bool moved;
    public:
        pawn(): piece(0, 1, WHITE_PAWN), en_passant(false), moved(false) {};
        pawn(bool is_exist, int16_t type): piece(is_exist, type*1, type>0?WHITE_PAWN:BLACK_PAWN), en_passant(false), moved(false) {}

        virtual ~pawn() = default;
        virtual std::vector<XY> get_move(XY pos);
};      // Quân tốt
class bishop: public piece {
    public:
        bishop(): piece(0, 1, WHITE_BISHOP) {};
        bishop(bool is_exist, int16_t type): piece(is_exist, type*2, type>0?WHITE_BISHOP:BLACK_BISHOP) {}

        virtual ~bishop() = default;
        virtual std::vector<XY> get_move(XY pos);
};    // Quân tượng
class knight: public piece {
    public:
        knight(): piece(0, 1, WHITE_KNIGHT) {};
        knight(bool is_exist, int16_t type): piece(is_exist, type*3, type>0?WHITE_KNIGHT:BLACK_KNIGHT) {}

        virtual ~knight() = default;
        virtual std::vector<XY> get_move(XY pos);
};    // Quân Mã
class rook: public piece {
    protected:
        bool moved;
    public:
        rook(): piece(0, 1, WHITE_ROOK), moved(false) {};
        rook(bool is_exist, int16_t type): piece(is_exist, type*4, type>0?WHITE_ROOK:BLACK_ROOK), moved(false) {}

        virtual ~rook() = default;
        virtual std::vector<XY> get_move(XY pos);
};      // Quân xe
class queen: public piece {
    public:
        queen(): piece(0, 1, WHITE_QUEEN) {};
        queen(bool is_exist, int16_t type): piece(is_exist, type*5, type>0?WHITE_QUEEN:BLACK_QUEEN) {}

        virtual ~queen() = default;
        virtual std::vector<XY> get_move(XY pos);
};     // Quân hậu
class king: public piece {
    protected:
        bool moved;
    public:
        king(): piece(0, 1, WHITE_KING), moved(false) {};
        king(bool is_exist, int16_t type): piece(is_exist, type*6, type>0?WHITE_KING:BLACK_KING), moved(false) {}

        virtual ~king() = default;
        virtual std::vector<XY> get_move(XY pos);
};      // Quân vua
class none: public piece {
    public:
        none(): piece() {}
};  // Không có quân

piece init_piece(int16_t type);
bool is_inside(XY pos);

#endif
