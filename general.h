#ifndef GENERAL_H
#define GENERAL_H

#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <vector>
#include <string>
#include "../raylib/src/raylib.h"

typedef struct {
    int x;
    int y;
} point;

typedef struct {
    int y;
    int x;
} XY;

const XY default_pos = {-1, -1};

typedef struct {
    int w;
    int h;
} size;

bool operator==(const Vector2&, const Vector2&);
bool operator==(const XY&, const XY&);
XY operator+(const XY&, const XY&);
XY operator-(const XY&, const XY&);

XY& operator+=(XY&, const XY&);

XY operator*(const XY&, const int&);
bool operator<(Vector2 point, Rectangle rec);

template <class T>
void swap(T &a, T &b);

const Color     ODD_CELL_COLOR = { 211, 176, 131, 255 };
const Color    EVEN_CELL_COLOR = { 217, 217, 217, 255 };
const Color  CHOSEN_CELL_COLOR = { 165, 111, 111, 255 };
const Color HOVERED_CELL_COLOR = { 165, 111, 111, 125 };

const size FRAME_SIZE = { 1650, 980 };
const int PADDING = 40;

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

const Vector2 ORIGIN = { 0.0f, 0.0f };

void draw_picture(const char* file_path, Rectangle dest_rect, float rotation=0.0f, Vector2 origin=ORIGIN, Color color=ORANGE);
void draw_picture(Texture2D texture, Rectangle dest_rect, float rotation=0.0f, Vector2 origin=ORIGIN, Color color=WHITE);


bool is_inside(XY pos);
bool is_promotion(XY pos);

#endif