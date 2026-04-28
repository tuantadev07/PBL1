#include "ui/ui_layout.h"


Vector2 ui_text_position_in_rect(
    Rectangle rect,
    Font font,
    const char* text,
    float fontSize,
    float spacing,
    float alignX,
    float alignY
) {
    if (text == nullptr) {
        text = "";
    }

    const Vector2 textSize = MeasureTextEx(font, text, fontSize, spacing);

    return Vector2{
        rect.x + (rect.width - textSize.x) * alignX,
        rect.y + (rect.height - textSize.y) * alignY
    };
}

