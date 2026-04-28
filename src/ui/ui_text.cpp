#include "ui/ui_text.h"

#include "ui/ui_layout.h"

void ui_draw_text_in_rect(
    Font font,
    const char* text,
    Rectangle rect,
    float fontSize,
    float spacing,
    float alignX,
    float alignY,
    Color textColor
) {
    if (text == nullptr || text[0] == '\0') {
        return;
    }

    const Vector2 pos = ui_text_position_in_rect(
        rect,
        font,
        text,
        fontSize,
        spacing,
        alignX,
        alignY
    );

    DrawTextEx(font, text, pos, fontSize, spacing, textColor);
}

void ui_draw_text_with_outline(
    Font font,
    const char* text,
    Vector2 pos,
    float fontSize,
    float spacing,
    float outlineThickness,
    Color textColor,
    Color outlineColor
) {
    if (text == nullptr || text[0] == '\0') {
        return;
    }

    DrawTextEx(font, text, Vector2{pos.x - outlineThickness, pos.y - outlineThickness}, fontSize, spacing, outlineColor);
    DrawTextEx(font, text, Vector2{pos.x,                    pos.y - outlineThickness}, fontSize, spacing, outlineColor);
    DrawTextEx(font, text, Vector2{pos.x + outlineThickness, pos.y - outlineThickness}, fontSize, spacing, outlineColor);
    DrawTextEx(font, text, Vector2{pos.x - outlineThickness, pos.y},                    fontSize, spacing, outlineColor);
    DrawTextEx(font, text, Vector2{pos.x + outlineThickness, pos.y},                    fontSize, spacing, outlineColor);
    DrawTextEx(font, text, Vector2{pos.x - outlineThickness, pos.y + outlineThickness}, fontSize, spacing, outlineColor);
    DrawTextEx(font, text, Vector2{pos.x,                    pos.y + outlineThickness}, fontSize, spacing, outlineColor);
    DrawTextEx(font, text, Vector2{pos.x + outlineThickness, pos.y + outlineThickness}, fontSize, spacing, outlineColor);

    DrawTextEx(font, text, pos, fontSize, spacing, textColor);
}

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
) {
    if (text == nullptr || text[0] == '\0') {
        return;
    }

    const Vector2 pos = ui_text_position_in_rect(
        rect,
        font,
        text,
        fontSize,
        spacing,
        alignX,
        alignY
    );

    ui_draw_text_with_outline(
        font,
        text,
        pos,
        fontSize,
        spacing,
        outlineThickness,
        textColor,
        outlineColor
    );
}
