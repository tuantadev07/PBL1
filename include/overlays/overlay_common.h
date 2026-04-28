#pragma once

#include "raylib.h"
#include "ui/ui_button.h"

void overlay_draw_backdrop(float alpha);
void overlay_draw_panel(
    Rectangle panel,
    Color fillColor,
    Color borderColor,
    float roundness = 0.08f,
    int segments = 8,
    float borderThickness = 3.0f
);
void overlay_draw_button(
    int buttonId,
    Rectangle rect,
    const char* text,
    Font font,
    float fontSize,
    const UIButtonStyle& fallbackStyle,
    Color textNormal,
    Color textHover,
    Color textPressed,
    bool isSelected = false
);
