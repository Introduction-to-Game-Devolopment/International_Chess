#include "menu.h"

bool menu(void) {   
    int flag = 0;
    SetTargetFPS(FPS);
    button play_game = button({(FRAME_SIZE.width - 600.0f)/2,300,600,100}, "PLAY GAME", WHITE, FONT, 70);
    button setting = button({(FRAME_SIZE.width - 600.0f)/2,450,600,100}, "SETTING", WHITE, FONT, 70);
    button about = button({(FRAME_SIZE.width - 600.0f)/2,600,600,100}, "ABOUT", WHITE, FONT, 70);
    button quit = button({(FRAME_SIZE.width - 600.0f)/2,750,600,100}, "QUIT", WHITE, FONT, 70);
    
    Vector2 name_game_size = MeasureTextEx(BOLD_FONT, "CHESS", 200, 10);
    Texture2D logo = LoadTexture("../asset/logo-removebg.png");
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
            DrawTexture(BACKGROUND, 0, 0, WHITE);
            DrawTextEx(BOLD_FONT, "CHESS", {50 + (FRAME_SIZE.width - name_game_size.x)/2, 50}, 225, 15, {42, 135, 120, 255}); //
            draw_picture(logo, {300, 45, 200, 200});
            
            play_game.draw_button();
            setting.draw_button();
            about.draw_button();
            quit.draw_button();
            
            play_game.is_hovered();
            setting.is_hovered();
            about.is_hovered();
            quit.is_hovered();
            if (play_game.is_clicked()) {
                flag = 1;
                break;
            }
            if (quit.is_clicked()) {
                flag = 0;
                break;
            }
        EndDrawing();
    }
    UnloadTexture(logo);
    switch (flag) {
        case 1:
            flag = get_name();
            break;
        default:
            return 0;
    }
    return flag;
}

bool get_name() {
    int flag = 0;
    SetTargetFPS(FPS);
    button back = button({225, 800, 300, 125}, "BACK", WHITE, FONT, 70);
    button play = button({FRAME_SIZE.width - 225.f - 300.f, 800, 300, 125}, "PLAY", WHITE, FONT, 70);
    
    // button play = button({})
    InputText player1 = InputText((FRAME_SIZE.width - 1000)/2, 350, 1000, 100, "Unknown");
    InputText player2 = InputText((FRAME_SIZE.width - 1000)/2, 550, 1000, 100, "Unknown");
    // InputText player1;
    Vector2 name_game_size = MeasureTextEx(BOLD_FONT, "CHESS", 200, 10);
    Texture2D logo = LoadTexture("../asset/logo-removebg.png");
    
    Rectangle rec = {(FRAME_SIZE.width - 1200.f)/2, 250, 1200, 500};
    Texture2D texture = blur_texture_in_background(rec);
    
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
            DrawTexture(BACKGROUND, 0, 0, WHITE);
            DrawTexture(texture, rec.x, rec.y, WHITE);
            DrawTextEx(BOLD_FONT, "CHESS", {50 + (FRAME_SIZE.width - name_game_size.x)/2, 50}, 225, 15, {42, 135, 120, 255}); 
            draw_picture(logo, {300, 45, 200, 200});
            DrawTextEx(FONT, "WHITE PLAYER:", {(FRAME_SIZE.width - 1000.f)/2, 270}, 70.0f, 10, WHITE);
            DrawTextEx(FONT, "BLACK PLAYER:", {(FRAME_SIZE.width - 1000.f)/2, 470}, 70.0f, 10, WHITE);
            
            player1.update();
            player2.update();
            player1.draw();
            player2.draw();
            
            back.draw_button();
            play.draw_button();
            
            back.is_hovered();
            play.is_hovered();
            
            if (back.is_clicked()) {
                flag = 1;
                break;
            }
            if (play.is_clicked()) {
                flag = 1;
                playgame(player1.get_text(), player2.get_text());
                break;
            }
        EndDrawing();
    }

    return flag;
}

bool playgame(std::string player1, std::string player2) {
    boardchess Board = boardchess(player1, player2);
    SetTargetFPS(FPS);
    Rectangle rec = {FRAME_SIZE.height + PADDING, PADDING, FRAME_SIZE.width - 2*PADDING - FRAME_SIZE.height, FRAME_SIZE.height - 2*PADDING};
    Texture2D texture = blur_texture_in_background(rec);

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
            
            DrawTexture(BACKGROUND, 0, 0, WHITE);
            DrawTexture(texture, rec.x, rec.y, WHITE);
            draw_rectangle_with_border(rec, {255,255,255,150}, 2, WHITE, 10);
            Board.draw_board();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mouse_pos = GetMousePosition();
                bool flag = Board.make_move(mouse_pos);
                if (!flag) Board.wait_for_event(mouse_pos);
                int8_t result = Board.is_end_match();
                if(result != 2) {
                    Board.end_match(result);
                }
                printf("Mouse left button pressed at [%f, %f]\n", mouse_pos.x, mouse_pos.y);
            }
        EndDrawing();
    }
    return 0;
}
