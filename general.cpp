#include "general.h"

bool operator==(const Vector2& lv, const Vector2& rv) {
    return (lv.x == rv.x) && (lv.y == rv.y);
}

bool operator==(const XY& u, const XY& v) {
    return u.y == v.y && u.x == v.x;
}

XY operator+(const XY& u, const XY& v) {
    return { u.y + v.y, u.x + v.x};
}

XY& operator+=(XY& u, const XY& v) {
    u.y += v.y;
    u.x += v.x;
    return u;
}

XY operator*(const XY& u, const int& d) {
    return {u.y * d, u.x * d};
}

XY& operator-=(XY& u, const XY& v) {
    u.x -= v.x;
    u.y -= v.y;
    return u;
}

bool operator<(Vector2 point, Rectangle rec) {
    if (point.x < (rec.x + rec.width) && point.x > rec.x && point.y < (rec.y + rec.height) && point.y > rec.y) return 1;
    return 0;
}

bool is_inside(XY pos) {
    return 0 <= pos.x && pos.x < 8 && 0 <= pos.y && pos.y < 8;
}

bool is_promotion(XY pos) {
    return pos.y == 0 || pos.y == 7;
}


Texture2D BLACK_PAWN;
Texture2D BLACK_BISHOP;
Texture2D BLACK_KNIGHT;
Texture2D BLACK_ROOK;
Texture2D BLACK_QUEEN;
Texture2D BLACK_KING;

Texture2D WHITE_PAWN;
Texture2D WHITE_BISHOP;
Texture2D WHITE_KNIGHT;
Texture2D WHITE_ROOK;
Texture2D WHITE_QUEEN;
Texture2D WHITE_KING;

void init_piece_texture(void) {
    BLACK_PAWN = LoadTexture("asset/black_pawn.png");
    BLACK_BISHOP = LoadTexture("asset/black_bishop.png");
    BLACK_KNIGHT = LoadTexture("asset/black_knight.png");
    BLACK_ROOK = LoadTexture("asset/black_rook.png");
    BLACK_QUEEN = LoadTexture("asset/black_queen.png");
    BLACK_KING = LoadTexture("asset/black_king.png");

    WHITE_PAWN = LoadTexture("asset/white_pawn.png");
    WHITE_BISHOP = LoadTexture("asset/white_bishop.png");
    WHITE_KNIGHT = LoadTexture("asset/white_knight.png");
    WHITE_ROOK = LoadTexture("asset/white_rook.png");
    WHITE_QUEEN = LoadTexture("asset/white_queen.png");
    WHITE_KING = LoadTexture("asset/white_king.png");
}

void destroy_piece_texture(void) {
    UnloadTexture(BLACK_BISHOP);
    UnloadTexture(BLACK_PAWN);
    UnloadTexture(BLACK_KNIGHT);
    UnloadTexture(BLACK_ROOK);
    UnloadTexture(BLACK_QUEEN);
    UnloadTexture(BLACK_KING);

    UnloadTexture(WHITE_PAWN);
    UnloadTexture(WHITE_BISHOP);
    UnloadTexture(WHITE_KNIGHT);
    UnloadTexture(WHITE_ROOK);
    UnloadTexture(WHITE_QUEEN);
    UnloadTexture(WHITE_KING);
}


void draw_picture(const char* file_path, Rectangle dest_rect, float rotation, Vector2 origin, Color color) {
    Texture2D texture = LoadTexture(file_path);
    if (texture.id == 0) return;
    Rectangle src_rect = { 0.0f, 0.0f, (float)texture.width, (float)texture.height};
    DrawTexturePro(texture, src_rect, dest_rect, origin, rotation, color);
    UnloadTexture(texture);
}

void draw_picture(Texture2D texture, Rectangle dest_rect, float rotation, Vector2 origin, Color color)  {
    if (texture.id == 0) return;
    Rectangle src_rect = { 0.0f, 0.0f, (float)texture.width, (float)texture.height};
    DrawTexturePro(texture, src_rect, dest_rect, origin, rotation, color);
}

void draw_rectangle_with_rounded(Rectangle rect, Color rect_color, border radius, int border_width, Color border_color) {
    DrawRectangle(rect.x + radius.top_left , rect.y, rect.width - radius.top_left  - radius.top_right, radius.top_left , rect_color);
    DrawRectangle(rect.x, rect.y + radius.top_left , radius.bottom_left, rect.height - radius.top_left  - radius.bottom_left, rect_color);
    DrawRectangle(rect.x + radius.bottom_left, rect.y + rect.height - radius.bottom_right, rect.width - radius.bottom_left - radius.bottom_right, radius.bottom_right, rect_color);
    DrawRectangle(rect.x + rect.width - radius.top_right, rect.y + radius.top_right , radius.top_right, rect.height - radius.top_right - radius.bottom_right, rect_color);
    DrawRectangle(rect.x + radius.bottom_left, rect.y + radius.top_left , rect.width - radius.top_right - radius.bottom_left, rect.height - radius.bottom_right - radius.top_left , rect_color);

    DrawCircleSector({rect.x + radius.top_left , rect.y + radius.top_left }, radius.top_left  - border_width, 180.0f, 270.0f, 64, rect_color);
    DrawCircleSector({rect.x + rect.width - radius.top_right, rect.y + radius.top_right}, radius.top_right - border_width, 270.0f, 360.0f, 64, rect_color);
    DrawCircleSector({rect.x + rect.width - radius.bottom_right, rect.y + rect.height - radius.bottom_right}, radius.bottom_right - border_width, 0.0f, 90.0f, 64, rect_color);
    DrawCircleSector({rect.x + radius.bottom_left, rect.y + rect.height - radius.bottom_left}, radius.bottom_left - border_width, 90.0f, 180.0f, 64, rect_color);
    
    if (border_width == 0) return;
    DrawLineEx({rect.x + radius.top_left , rect.y + float(border_width)/2}, {rect.x + rect.width - radius.top_right, rect.y + float(border_width)/2}, border_width, border_color); 
    DrawLineEx({rect.x + float(border_width)/2, rect.y + radius.top_left }, {rect.x + float(border_width)/2, rect.y + rect.height - radius.bottom_left}, border_width, border_color); 
    DrawLineEx({rect.x + radius.bottom_left, rect.y + rect.height - float(border_width)/2}, {rect.x + rect.width - radius.bottom_right, rect.y + rect.height - float(border_width)/2}, border_width, border_color); 
    DrawLineEx({rect.x + rect.width - float(border_width)/2, rect.y + radius.top_right}, {rect.x + rect.width - float(border_width)/2, rect.y + rect.height - radius.bottom_right}, border_width, border_color); 
    
    DrawRing({rect.x + radius.top_left , rect.y + radius.top_left }, radius.top_left , radius.top_left  - border_width, 180.0f, 270.0f, 64, border_color);
    DrawRing({rect.x + rect.width - radius.top_right, rect.y + radius.top_right}, radius.top_right, radius.top_right - border_width, 270.0f, 360.0f, 64, border_color);
    DrawRing({rect.x + rect.width - radius.bottom_right, rect.y + rect.height - radius.bottom_right}, radius.bottom_right, radius.bottom_right - border_width, 0.0f, 90.0f, 64, border_color);
    DrawRing({rect.x + radius.bottom_left, rect.y + rect.height - radius.bottom_left}, radius.bottom_left, radius.bottom_left - border_width, 90.0f, 180.0f, 64, border_color);
}

void draw_rectangle_with_border(Rectangle rect, Color rect_color, int border_width, Color border_color, int border_radius) {
    DrawRectangleRounded(rect, 1.0*border_radius/100, 64, rect_color); 
    DrawRectangleRoundedLines(rect, 1.0*border_radius/100, 64, border_width, border_color); 
}
