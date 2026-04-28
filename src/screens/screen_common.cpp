#include "screens/screen_common.h"

#include "assets/game_assets.h"

void screen_draw_background(Color fallbackColor) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), fallbackColor);
}

void screen_draw_background(Texture2D texture, Color fallbackColor) {
    if (texture.id != 0) {
        const Rectangle src = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
        const Rectangle dst = {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()};
        DrawTexturePro(texture, src, dst, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
        return;
    }

    screen_draw_background(fallbackColor);
}

void screen_draw_button(
    int buttonId,
    Rectangle rect,
    const char* text,
    float fontSize,
    const UIButtonStyle& fallbackStyle,
    bool isSelected
) {
    const UIButtonState state = ui_button_get_state(buttonId, rect);

    if (g_assets.buttons.common.normal.id != 0) {
        ui_button_draw_state_textures(rect, g_assets.buttons.common, state, isSelected);
    } else {
        ui_button_draw(rect, fallbackStyle, state);
    }

    ui_button_draw_text(
        rect,
        text,
        g_assets.fonts.common,
        fontSize,
        state,
        DARKGRAY,
        BLACK,
        WHITE
    );
}
