#include "player.h"

// Class đại diện cho ô 
class cell {        
    protected:
        piece Piece;    //Piece
        XY pos;         //Row and col
        Rectangle rec;
        bool is_chosen; 
        bool is_hover;
        int length;     //Size
    public:
        cell(): Piece(), pos({0,0}), rec({0,0,0,0}), is_chosen(0), is_hover(0), length(0) {}
        cell(XY, int16_t, size frame_size, int padding);
        int16_t get_type_piece() { return this->Piece.get_type();}
        Rectangle get_rect() { return this->rec;}
        XY get_row_col() { return this->pos;}
        int get_len() { return this->length;}
        bool get_hover() { return this->is_hover;}
        bool get_chosen() { return this->is_chosen;}
        
        void draw_cell();
        void hover(piece);
        void unhover();
        void choose(int8_t);
        void unchoose();
        // void change_piece(int16_t);
};

// Class bàn cờ
class board {
    protected:
        player white_player;
        player black_player;
        cell board_game[8][8];
        int8_t turn;
    public:
        board(std::string, std::string);
        board(): board("Anonymous", "Anonymous") {};
        void draw_board();
        bool is_valid_move();
        int8_t is_end_match();
        void make_move();
        // ...
};