#ifndef BOARD_H
#define BOARD_H

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
        cell(): pos({0,0}), rec({0,0,0,0}), is_chosen(0), is_hover(0), length(0) {}
        cell(XY, int16_t, size frame_size, int padding=PADDING);
        int16_t get_type_piece(void) { return this->Piece.get_type(); }
        XY get_pos(void) { return this->pos; }
        Rectangle get_rect(void) { return this->rec; }
        int get_len(void) { return this->length; }
        bool get_hover(void) { return this->is_hover; }
        bool get_chosen(void) { return this->is_chosen; }
        
        void draw_cell();
        void hover(piece);
        void unhover();
        void choose(int8_t);
        void unchoose();
        // void change_piece(int16_t);

        bool is_exist_piece(void) { return this->Piece.get_is_exist(); }
        bool is_moved_piece(void) { return this->Piece.get_was_moved(); }
};

// Class bàn cờ
class board {
    protected:
        player white_player;
        player black_player;
        cell board_game[8][8];
        int8_t turn;
        XY en_passant;
        //std::vector<XY> check_piece;
    public:
        board(std::string, std::string);
        board(): board("Anonymous", "Anonymous") {};
        void draw_board();
        bool is_valid_move();
        int8_t is_end_match();
        void make_move();

        bool is_inside(XY);
        bool is_blocked(XY);
        bool is_captured(XY);
        bool is_en_passant(XY);
        bool is_promotion(XY);
        std::vector<XY> get_move(cell);
        // ...
};

#endif