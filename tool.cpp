
#include "tool.h"
#include "general.h"
#include <sstream>

button::button(Rectangle bound, const std::string text, Color text_color, Font font, float font_size)
    : bound(bound), content(text), content_color(text_color), font(font), font_size(font_size) {
    Vector2 size = MeasureTextEx(this->font, this->content.c_str(), this->font_size,0);
    Vector2 position = {(bound.width - size.x)/2, (bound.height - size.y)/2};
    printf("x=%f, y=%f, w=%f, h=%f, size=(%f, %f)--> (%f, %f)", bound.x, bound.y, bound.width, bound.height, size.x, size.y, position.x, position.y);
    Image image = LoadImage("../asset/background.png");
    ImageCrop(&image, this->bound);
    ImageBlurGaussianAndRounded(&image, 10, 20);
    ImageDrawTextEx(&image, this->font, this->content.c_str(), position, this->font_size, 10, WHITE);
    // ImageDrawTextEx(&image, this->font, "|", {position.x + size.x, position.y}, this->font_size, 10, WHITE); // for debuging
    
    this->texture = LoadTextureFromImage(image);
    UnloadImage(image);
}

void button::draw_button() {
    DrawTexture(texture, this->bound.x, this->bound.y, WHITE);
    if (this->is_hover) {
        DrawRectangleRounded(this->bound, 0.35f, 32, {200,200,200,90});
    } else {
        DrawRectangleRounded(this->bound, 0.35f, 32, {50,50,50,125});
    }
}

void button::is_hovered() {
    this->is_hover = CheckCollisionPointRec(GetMousePosition(), bound);  
}


bool button::is_clicked() {
    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), bound);
}


void InputText::draw() {
    DrawRectangleRounded(this->textBox, 0.20f, 64, LIGHTGRAY);
    DrawRectangleRoundedLinesEx(this->textBox, 0.2f, 64, 3, mouseOnText ? BLACK : GRAY);
    if (this->text.empty() && !this->mouseOnText) {
        DrawTextEx(FONT, this->placeholder.c_str(), {this->textBox.x + 16, this->textBox.y + 12}, 70.0f, 10, BLACK);
    } else {
        DrawTextEx(FONT, this->text.c_str(), {this->textBox.x + 16, this->textBox.y + 12}, 70.0f, 10, BLACK);
    }
    // Nháy con trỏ
    if (this->mouseOnText) {
        if (this->text.length() < 30) {
            if (((this->framesCounter / 20) % 2) == 0) {
                DrawTextEx(FONT, "_", {this->textBox.x + 16 + MeasureTextEx(FONT, this->text.c_str(), 70,10).x, this->textBox.y + 12}, 70, 10, BLACK);
            }
        }
    }
}
    
void InputText::update() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), this->textBox)) {
            this->mouseOnText = true;
        } else {
            this->mouseOnText = false;
        }
    }
    if (this->mouseOnText) {
        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (this->text.length() < 30)) {
                this->text += (char)key;
            }
            if (CheckCollisionPointRec(GetMousePosition(), this->textBox)) {
                this->mouseOnText = true;
            } else {
                this->mouseOnText = false;
            }
            if (this->mouseOnText) {
                int key = GetCharPressed();
                while (key > 0) {
                    if ((key >= 32) && (key <= 125) && (text.length() < 30)) {
                        this->text += (char)key;
                    }
        
                    key = GetCharPressed();
                }
        
                // Xử lý phím backspace
                if (IsKeyPressed(KEY_BACKSPACE)) {
                    if (!this->text.empty()) {
                        this->text.pop_back();
                    }
                }
            }
        
            this->framesCounter++;
            key = GetCharPressed();
        }

        // Xử lý phím backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (!this->text.empty()) {
                this->text.pop_back();
            }
        }
    }

    this->framesCounter++;
}

TextBox::TextBox(Rectangle bound, std::string str) 
    :  bound(bound), text(str), text_box_padding(10), scroll_offset(0), scroll_dragging(false), scroll_thumb_offset(0.0f) {
    font = GetFontDefault();
    this->text_height = MeasureTextEx(font, this->text.c_str(), 20, 2).y;
    this->max_scroll = this->text_height - this->bound.height + 2 * this->text_box_padding;

    this->scroll_bar = { this->bound.x + this->bound.width - 20, this->bound.y, 20, this->bound.height };
    this->scroll_thumb = { this->scroll_bar.x, this->scroll_bar.y, this->scroll_bar.width, this->scroll_bar.height * (this->text_height>this->bound.height?(this->bound.height / this->text_height):1) };
}

void TextBox::update() {
    update_scroll();
    // Update scroll offset with mouse wheel
    if (IsKeyDown(KEY_DOWN)) this->scroll_offset += 4;
    if (IsKeyDown(KEY_UP)) this->scroll_offset -= 4;
    if (GetMouseWheelMove() != 0) this->scroll_offset -= GetMouseWheelMove() * 20;

    if (this->scroll_offset < 0) this->scroll_offset = 0;
    if (this->scroll_offset > this->max_scroll) this->scroll_offset = this->max_scroll;

    // Update scroll thumb position based on scroll offset
    this->scroll_thumb.y = this->scroll_bar.y + (this->scroll_offset * (this->scroll_bar.height - this->scroll_thumb.height) / this->max_scroll);

    // Auto scroll to bottom if text height changes
    float newtext_height = MeasureTextEx(font, this->text.c_str(), 20, 2).y;
    if (newtext_height != this->text_height) {
        this->text_height = newtext_height;
        this->max_scroll = this->text_height - this->bound.height + 2 * this->text_box_padding;
        this->scroll_offset = this->max_scroll;
        this->scroll_thumb.y = this->scroll_bar.y + (this->scroll_offset * (this->scroll_bar.height - this->scroll_thumb.height) / this->max_scroll);
    }
}

void TextBox::update_scroll() {
    Vector2 mousePosition = GetMousePosition();

    // Scrollbar logic
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, this->scroll_thumb)) {
        this->scroll_dragging = true;
        this->scroll_thumb_offset = mousePosition.y - this->scroll_thumb.y;
    }

    if (this->scroll_dragging) {
        if (IsMouseButtonUp(MOUSE_LEFT_BUTTON)) {
            this->scroll_dragging = false;
        } else {
            this->scroll_thumb.y = mousePosition.y - this->scroll_thumb_offset;
            if (this->scroll_thumb.y < this->scroll_bar.y) this->scroll_thumb.y = this->scroll_bar.y;
            if (this->scroll_thumb.y + this->scroll_thumb.height > this->scroll_bar.y + this->scroll_bar.height) this->scroll_thumb.y = this->scroll_bar.y + this->scroll_bar.height - this->scroll_thumb.height;

            // Calculate scroll offset based on scroll thumb position
            this->scroll_offset = (this->scroll_thumb.y - this->scroll_bar.y) * this->max_scroll / (this->scroll_bar.height - this->scroll_thumb.height);
        }
    }
}

void TextBox::draw() {
    // Draw text box
    DrawRectangleRec(this->bound, LIGHTGRAY);
    BeginScissorMode(this->bound.x + this->text_box_padding, this->bound.y + this->text_box_padding, this->bound.width - 2 * this->text_box_padding, this->bound.height - 2 * this->text_box_padding);
    DrawTextEx(font, this->text.c_str(), (Vector2){ this->bound.x + this->text_box_padding, this->bound.y + this->text_box_padding - this->scroll_offset }, 20, 2, BLACK);
    EndScissorMode();
    DrawRectangleLinesEx(this->bound, 1, DARKGRAY);

    // Draw scrollbar
    DrawRectangleRec(this->scroll_bar, DARKGRAY);
    DrawRectangleRec(this->scroll_thumb, GRAY);
}

void TextBox::update_data(move_in_chess tmp, bool special_move) {
    char line[30] = {0};
    sprintf(line, "(%d, %d) --> (%d, %d)%s", tmp.src.y+1, tmp.src.x+1, tmp.dest.y+1, tmp.dest.x+1, special_move?" | ":"\n");
    this->text += line;
    this->scroll_thumb = { this->scroll_bar.x, this->scroll_bar.y, this->scroll_bar.width, this->scroll_bar.height * (this->text_height>this->bound.height?(this->bound.height / this->text_height):1) };
    printf("update_data: %s\n", line);
}



