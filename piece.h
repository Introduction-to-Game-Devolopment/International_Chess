#include "general.h"

class piece {
    protected:
        bool is_exist;
        int16_t type;
        Image image;
    // Hình quân cờ: https://commons.wikimedia.org/wiki/Category:SVG_chess_pieces
    public:
        piece(): is_exist(0), type(0) {}
        piece(bool is_exist, int16_t type, Image image): is_exist(is_exist), type(type), image(image) {}
        bool get_exist() {
            return this->is_exist;
        }
        int16_t get_type(){
            return this->type; 
        }
        Image get_image() {
            return this->image;
        }
        void change_exist();
        virtual ~piece() = default; // Destructor ảo để lớp dẫn xuất có thể dọn dẹp tài nguyên khi quân cờ bị ăn
};

class pawn : public piece {
    public:
        pawn(bool is_exist, int16_t type, Image image): piece(is_exist, type*1, image) {}
};      // Quân tốt
class bishop : public piece {
    public:
        bishop(bool is_exist, int16_t type, Image image): piece(is_exist, type*2, image) {}
};    // Quân tượng
class knight : public piece {
    public:
        knight(bool is_exist, int16_t type, Image image): piece(is_exist, type*3, image) {}
};    // Quân Mã
class rook : public piece {
    public:
        rook(bool is_exist, int16_t type, Image image): piece(is_exist, type*4, image) {}
};      // Quân xe
class queen : public piece {
    public:
        queen(bool is_exist, int16_t type, Image image): piece(is_exist, type*5, image) {}
};     // Quân hậu
class king : public piece {
    public:
        king(bool is_exist, int16_t type, Image image): piece(is_exist, type*6, image) {}
};      // Quân vua
class none : public piece {
    public:
        none(): piece() {}
};  // Không có quân

piece init_piece(int16_t type, int length);