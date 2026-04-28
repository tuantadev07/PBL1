#pragma once

#include "raylib.h"
#include "ui/ui_button.h"

void screen_draw_background(Color fallbackColor);
void screen_draw_background(Texture2D texture, Color fallbackColor);
void screen_draw_button(
    int buttonId,
    Rectangle rect,
    const char* text,
    float fontSize,
    const UIButtonStyle& fallbackStyle,
    bool isSelected = false
);
