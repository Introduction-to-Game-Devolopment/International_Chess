#ifndef BOARD_H
#define BOARD_H

#include "general.h"
#include "piece.h"
#include "player.h"

// Represent a cell in boardchess 
class cell {        
    protected:
        piece       Piece;      // Piece
        XY          pos;        // Row and col
        Rectangle   rec;        // Rectangle bounded around cell
        float       length;     // Size
        bool        is_chosen;  // For make_move and get_move
        bool        is_hover;   // For make_move and get_move
    public:
        cell(): pos({0,0}), rec({0,0,0,0}), length(0) , is_chosen(0), is_hover(0){}
        cell(XY, int16_t, size frame_size=FRAME_SIZE, int padding=PADDING);
        
        piece       get_piece(void)         { return this->Piece; }
        int16_t     get_type_piece(void)    { return this->Piece.get_type(); }
        XY          get_pos(void)           { return this->pos; }
        Rectangle   get_rect(void)          { return this->rec; }
        float       get_len(void)           { return this->length; }
        bool        is_exist_piece(void)    { return this->Piece.get_is_exist(); }
        bool        is_moved_piece(void)    { return this->Piece.get_was_moved(); }
        bool        get_hover(void)         { return this->is_hover; }
        bool        get_chosen(void)        { return this->is_chosen; }
        
        void exist(void)        { return this->Piece.exist(); }
        void unexist(void)      { return this->Piece.unexist(); }
        void change_exist(void) { return this->Piece.change_exist();}
        void moved(void)        { return this->Piece.moved(); }
        void change_piece(piece);
        void hover(void);
        void unhover(void);
        void choose(int8_t, Vector2);
        void unchoose(void);
        
        void draw_cell(border radius={0,0,0,0}, int border_width=2, Color border_color=BORDER_COLOR);
};

// Represent a boardchess 
class boardchess {
    protected:
        player  white_player;
        player  black_player;
        cell    board[8][8];
        int8_t  turn;
        XY      en_passant;
        int8_t  fifty_moves;
    public:
        boardchess(std::string, std::string);
        boardchess(): boardchess("Anonymous", "Anonymous") {}
        
        int8_t  is_end_match();
        bool    is_blocked(XY);
        bool    is_captured(XY);
        bool    is_en_passant(XY);
        bool    is_in_check(void);
        bool    is_valid_move(XY, XY);
        
        void    draw_board(void);
        void    wait_for_event(Vector2);
        bool    swap_cell(cell&, cell&);
        bool    swap_test(cell&, cell&, bool undo=false);
        bool    make_move(Vector2);
        void    end_match(int8_t);
        
        std::vector<XY> get_move(cell);
        // ...
};

#endif