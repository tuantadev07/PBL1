#include "overlays/overlay_common.h"

#include "assets/game_assets.h"

void overlay_draw_backdrop(float alpha) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, alpha));
}

void overlay_draw_panel(
    Rectangle panel,
    Color fillColor,
    Color borderColor,
    float roundness,
    int segments,
    float borderThickness
) {
    DrawRectangleRounded(panel, roundness, segments, fillColor);
    DrawRectangleRoundedLinesEx(panel, roundness, segments, borderThickness, borderColor);
}

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
        font,
        fontSize,
        state,
        textNormal,
        textHover,
        textPressed
    );
}
