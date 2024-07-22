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
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Board.draw_board();
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mouse_pos = GetMousePosition();
            bool flag = Board.make_move(mouse_pos);
            if (!flag) Board.wait_for_event(mouse_pos);
            printf("Mouse left button pressed at [%f, %f]\n", mouse_pos.x, mouse_pos.y);
        }

        EndDrawing();
    }

    destroy_piece_texture();
    CloseWindow();

    return 0;
}