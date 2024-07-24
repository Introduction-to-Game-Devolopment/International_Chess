#ifndef GENERAL_H
#define GENERAL_H

// These are libraries that using in this code
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <vector>
#include <string>
//#include "../raylib/src/raylib.h"
#include "raylib.h"

// Define structs using in this code
typedef struct { float x, y; } point;                                             // Represent a point in frame
typedef struct { int y, x; } XY;                                                // Represen cell_pos in the board
typedef struct { int width, height; } size;                                     // Represent a size of rectangle (or something else...)
typedef struct { int top_left, top_right, bottom_right, bottom_left; } border;  // Represent four corner in a rectangle

// Operator and check_function using in this code
bool operator==(const Vector2&, const Vector2&);
bool operator==(const XY&, const XY&);
XY operator+(const XY&, const XY&);
XY operator-(const XY&, const XY&);
XY operator*(const XY&, const int&);
XY& operator+=(XY&, const XY&);
bool operator<(Vector2, Rectangle);       // Check if a point lies inside a rectangle

bool is_inside(XY);                             // Check if a cell position is within the bounds of a board 
bool is_promotion(XY);                          // Check is a moving is promotion

// Define const variables
// For frame_window 
const size FRAME_SIZE = { 1650, 980 };
const int     PADDING = 40;
const int         FPS = 60;
// For chessboard
const XY DEFAULT_POS = {-1, -1};
const Color     ODD_CELL_COLOR = { 211, 176, 131, 255 };
const Color    EVEN_CELL_COLOR = { 217, 217, 217, 255 };
const Color  CHOSEN_CELL_COLOR = { 165, 111, 111, 255 };
const Color HOVERED_CELL_COLOR = { 165, 111, 111, 125 };
const Color       BORDER_COLOR = {   0,   0,   0, 255 };

const XY   PAWN_MOVE[] = { {1, -1}, {1,  0}, { 1, 1}, {-1,  1}, {-1, 0}, {-1, -1}                     };
const XY BISHOP_MOVE[] = { {1, -1}, {1,  1}, {-1, 1}, {-1, -1}                                        };
const XY KNIGHT_MOVE[] = { {1, -2}, {2, -1}, { 2, 1}, { 1,  2}, {-1, 2}, {-2,  1}, {-2, -1}, {-1, -2} };
const XY   ROOK_MOVE[] = { {0, -1}, {1,  0}, { 0, 1}, {-1,  0}                                        };
const XY  QUEEN_MOVE[] = { {0, -1}, {1, -1}, { 1, 0}, { 1,  1}, { 0, 1}, {-1,  1}, {-1,  0}, {-1, -1} };
const XY   KING_MOVE[] = { {0, -1}, {1, -1}, { 1, 0}, { 1,  1}, { 0, 1}, {-1,  1}, {-1,  0}, {-1, -1} };

extern Texture2D BLACK_PAWN;
extern Texture2D BLACK_BISHOP;
extern Texture2D BLACK_KNIGHT;
extern Texture2D BLACK_ROOK;
extern Texture2D BLACK_QUEEN;
extern Texture2D BLACK_KING;

extern Texture2D WHITE_PAWN;
extern Texture2D WHITE_BISHOP;
extern Texture2D WHITE_KNIGHT;
extern Texture2D WHITE_ROOK;
extern Texture2D WHITE_QUEEN;
extern Texture2D WHITE_KING;

void init_piece_texture(void);
void destroy_piece_texture(void);

// GUI 
const Vector2 ORIGIN = { 0.0f, 0.0f };

void draw_picture(const char*, Rectangle, float rotation=0.0f, Vector2 origin=ORIGIN, Color color=ORANGE);
void draw_picture(Texture2D, Rectangle, float rotation=0.0f, Vector2 origin=ORIGIN, Color color=WHITE);
void draw_rectangle_with_border(Rectangle, Color, int, Color, int border_radius=0);
void draw_rectangle_with_rounded(Rectangle, Color, border, int, Color);

#endif