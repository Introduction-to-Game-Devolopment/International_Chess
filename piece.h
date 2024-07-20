#include "general.h"

class piece {
    protected:
        bool is_exist;
        int16_t type;
    // Hình quân cờ
    public:
        piece(): is_exist(0), type(0) {}
        piece(bool is_exist, int16_t type): is_exist(is_exist), type(type) {}
        int16_t get_type(){
            return this->type; 
        }
        void change_exist();
        virtual ~piece() = default; // Destructor ảo để lớp dẫn xuất có thể dọn dẹp tài nguyên nếu 
};

class pawn : public piece {
    public:
        pawn(bool is_exist, int16_t type) : piece(is_exist, type*1) {}
};      // Quân tốt
class bishop : public piece {
    public:
        bishop(bool is_exist, int16_t type) : piece(is_exist, type*2) {}
};    // Quân tượng
class knight : public piece {
    public:
        knight(bool is_exist, int16_t type) : piece(is_exist, type*3) {}
};    // Quân Mã
class rook : public piece {
    public:
        rook(bool is_exist, int16_t type) : piece(is_exist, type*4) {}
};      // Quân xe
class queen : public piece {
    public:
        queen(bool is_exist, int16_t type) : piece(is_exist, type*5) {}
};     // Quân hậu
class king : public piece {
    public:
        king(bool is_exist, int16_t type) : piece(is_exist, type*6) {}
};      // Quân vua
class none : public piece {
    public:
        none(): piece(0,0) {}
};  // Không có quân

piece init_piece(int16_t type);