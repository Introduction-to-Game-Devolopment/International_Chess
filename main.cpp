#include "board.h"

int main(void)
{
    InitWindow(FRAME_SIZE.w, FRAME_SIZE.h, "raylib [core] example - basic window");

    SetTargetFPS(60);
    board Board = board();
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        Board.draw_board();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
