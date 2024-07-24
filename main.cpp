#include "general.h"
#include "board.h"
#include "piece.h"
#include "player.h"

int main(void)
{   
    InitWindow(FRAME_SIZE.width, FRAME_SIZE.height, "3AESN - CHESS");
    init_piece_texture();

    SetTargetFPS(FPS);
    boardchess Board = boardchess();
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Board.draw_board();
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mouse_pos = GetMousePosition();
            bool flag = Board.make_move(mouse_pos);
            if (!flag) Board.wait_for_event(mouse_pos);
            if (int8_t result = Board.is_end_match()) {
                Board.end_match(result);
                // break;
            }
            printf("Mouse left button pressed at [%f, %f]\n", mouse_pos.x, mouse_pos.y);
        }

        EndDrawing();
    }

    destroy_piece_texture();
    CloseWindow();

    return 0;
}