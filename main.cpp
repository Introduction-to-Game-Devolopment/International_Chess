#include "general.h"
#include "board.h"
#include "piece.h"
#include "player.h"
#include "menu.h"

int main(void)
{   
    InitWindow(FRAME_SIZE.width, FRAME_SIZE.height, "3AESN - CHESS");
    init_piece_texture();
    init_resource();
    
    SetTargetFPS(FPS);
    
    BeginDrawing();
    DrawTexture(BACKGROUND, 0, 0, WHITE);
    
    int flag = 1;
    while (flag) {
        flag = menu();
    }
    
    EndDrawing();
    destroy_resource();
    destroy_piece_texture();
    CloseWindow();

    return 0;
}