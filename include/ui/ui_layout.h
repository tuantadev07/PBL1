#pragma once

#include "raylib.h"

Vector2 ui_text_position_in_rect(
    Rectangle rect,
    Font font,
    const char* text,
    float fontSize,
    float spacing = 1.0f,
    float alignX = 0.5f,
    float alignY = 0.5f
);