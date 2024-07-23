#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include "player.h"
#include <cstdint>
#include <vector>

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
        void change_piece(piece);
        piece get_piece() {return this->Piece; }
        int16_t get_type_piece(void) { return this->Piece.get_type(); }
        XY get_pos(void) { return this->pos; }
        Rectangle get_rect(void) { return this->rec; }
        int get_len(void) { return this->length; }
        bool get_hover(void) { return this->is_hover; }
        bool get_chosen(void) { return this->is_chosen; }
        
        void draw_cell();
        void hover(void);
        void unhover(void);
        void choose(int8_t, Vector2);
        void unchoose(void);

        bool is_exist_piece(void) { return this->get_type_piece()?1:0; }
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
    public:
        board(std::string, std::string);
        board(): board("Anonymous", "Anonymous") {};
        void draw_board(void);
        int8_t is_end_match();
        void wait_for_event(Vector2);
        void swap_cell(cell&, cell&);
        bool make_move(Vector2);

        bool is_blocked(XY);
        bool is_captured(XY);
        bool is_en_passant(XY);
        bool is_promotion(XY);
        bool is_in_check();
        bool is_valid_move(XY, XY, int);
        std::vector<XY> get_move(cell);
        // ...
};

#endif