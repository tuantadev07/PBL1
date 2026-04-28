#pragma once

#include "raylib.h"

struct UITextInputStyle {
    Color fill;
    Color fillActive;
    Color border;
    Color borderActive;
    Color text;
    Color placeholder;
};

bool ui_text_input_is_clicked(Rectangle rect);
void ui_text_input_update(char* buffer, int capacity, bool active);
void ui_text_input_draw(
    Rectangle rect,
    const char* text,
    const char* placeholder,
    Font font,
    float fontSize,
    bool active,
    const UITextInputStyle& style
);
