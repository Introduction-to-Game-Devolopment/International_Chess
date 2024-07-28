#ifndef TOOL_H
#define TOOL_H
#include "general.h"

class button {
    private:
        Rectangle bound;
        bool is_hover;
        std::string content;
        Color content_color;
        Font font;
        float font_size;
        Texture2D texture;
    public:
        button(): button({0,0,0,0}, "BUG", BLACK, GetFontDefault(), 0) {}
        button(Rectangle, const std::string, Color, Font, float);
        void draw_button();
        bool is_clicked();
        void is_hovered();
        
};

class InputText {
    private:
        Rectangle textBox;
        std::string text;
        std::string placeholder;
        bool mouseOnText;
        int framesCounter;
    public:
        InputText(float x, float y, float width, float height, std::string placeholder) 
           : textBox{x, y, width, height}, text(""), placeholder(placeholder), mouseOnText(false), framesCounter(0) {}
        void update();
        void draw();
        std::string get_text() { return this->text.empty()?this->placeholder:this->text; }
};


class TextBox {
public:
    TextBox(): TextBox({0,0,0,0}, "") {}
    TextBox(Rectangle bound, std::string);
    virtual ~TextBox() = default;
    void update();
    void draw();
    void update_data(move_in_chess, bool special_move=0);
private:
    Rectangle bound;
    std::string text;
    int text_box_padding;
    float text_height;
    int scroll_offset;
    int max_scroll;

    // Scrollbar settings
    Rectangle scroll_bar;
    Rectangle scroll_thumb;
    bool scroll_dragging;
    float scroll_thumb_offset;

    Font font;

    void update_scroll();
};


#endif