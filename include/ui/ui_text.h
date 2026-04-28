#pragma once

#include "raylib.h"

void ui_draw_text_in_rect(
    Font font,
    const char* text,
    Rectangle rect,
    float fontSize,
    float spacing,
    float alignX,
    float alignY,
    Color textColor
);

void ui_draw_text_with_outline(
    Font font,
    const char* text,
    Vector2 pos,
    float fontSize,
    float spacing,
    float outlineThickness,
    Color textColor,
    Color outlineColor
);

void ui_draw_text_in_rect_with_outline(
    Font font,
    const char* text,
    Rectangle rect,
    float fontSize,
    float spacing,
    float alignX,
    float alignY,
    float outlineThickness,
    Color textColor,
    Color outlineColor
);
