#include "general.h"
#include "board.h"
#include "piece.h"
#include "player.h"

int main(void)
{   
    InitWindow(FRAME_SIZE.w, FRAME_SIZE.h, "raylib [core] example - basic window");
    init_piece_texture();

    SetTargetFPS(60);
    board Board = board();
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Board.draw_board();

        EndDrawing();
    }

    destroy_piece_texture();
    CloseWindow();

    return 0;
}