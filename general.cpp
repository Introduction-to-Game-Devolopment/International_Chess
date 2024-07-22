#include "general.h"

bool operator==(const Vector2& lv, const Vector2& rv) {
    return (lv.x == rv.x) && (lv.y == rv.y);
}

bool operator==(const XY& p, const XY& q) {
    return p.x == q.x && p.y == q.y;
}

XY operator+(const XY& u, const XY& v) {
    return {u.x + v.x, u.y + v.y};
}

XY& operator+=(XY& u, const XY& v) {
    u.x += v.x;
    u.y += v.y;
    return u;
}

XY operator*(const XY& u, const int& d) {
    return {u.x * d, u.y * d};
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
