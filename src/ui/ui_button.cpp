#include "ui/ui_button.h"

#include "assets/game_assets.h"
#include "ui/ui_layout.h"

namespace {
    int g_activeButtonId = -1;
    bool g_interactionEnabled = true;
}

void ui_button_set_interaction_enabled(bool enabled) {
    g_interactionEnabled = enabled;
}

void ui_button_reset_active() {
    g_activeButtonId = -1;
}

UIButtonState ui_button_get_state(int buttonId, Rectangle rect) {
    if (!g_interactionEnabled) {
        return UI_BTN_NORMAL;
    }

    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, rect);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (hover) {
            g_activeButtonId = buttonId;
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && g_activeButtonId == buttonId) {
        g_activeButtonId = -1;
    }

    if (g_activeButtonId == buttonId && IsMouseButtonDown(MOUSE_BUTTON_LEFT) && hover) {
        return UI_BTN_PRESSED;
    }

    if (hover) {
        return UI_BTN_HOVER;
    }

    return UI_BTN_NORMAL;
}

bool ui_button_is_clicked(int buttonId, Rectangle rect) {
    if (!g_interactionEnabled) {
        return false;
    }

    const Vector2 mouse = GetMousePosition();
    const bool hover = CheckCollisionPointRec(mouse, rect);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (hover) {
            g_activeButtonId = buttonId;
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        const bool isOwner = (g_activeButtonId == buttonId);
        const bool clicked = isOwner && hover;

        if (isOwner) {
            g_activeButtonId = -1;
        }

        if (clicked) {
            PlaySound(g_assets.sounds.click);
            // playSoundClick();
        }

        return clicked;
    }

    return false;
}

void ui_button_draw(Rectangle rect, const UIButtonStyle& style, UIButtonState state) {
    Color fill = style.normal;
    if (state == UI_BTN_HOVER) fill = style.hover;
    if (state == UI_BTN_PRESSED) fill = style.pressed;

    DrawRectangleRec(rect, fill);
    DrawRectangleLinesEx(rect, 2.0f, style.border);
}

void ui_button_draw_state_textures(Rectangle rect, const UIButtonStateTextures& textures, UIButtonState state, bool isSelected) {
    Texture2D tex = textures.normal;
    if (isSelected && textures.selected.id != 0) tex = textures.selected;
    if (state == UI_BTN_HOVER && textures.hover.id != 0) tex = textures.hover;
    if (state == UI_BTN_PRESSED && textures.pressed.id != 0) tex = textures.pressed;

    if (tex.id == 0) return;

    Rectangle src = {0.0f, 0.0f, (float)tex.width, (float)tex.height};
    DrawTexturePro(tex, src, rect, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
}

void ui_button_draw_text(
    Rectangle rect, const char* text, Font font, float fontSize,
    UIButtonState state, Color normal, Color hover, Color pressed
) {
    if (text == nullptr) return;

    Color c = normal;
    if (state == UI_BTN_HOVER) c = hover;
    if (state == UI_BTN_PRESSED) c = pressed;

    Vector2 pos = ui_text_position_in_rect(rect, font, text, fontSize, 1.0f, 0.5f, 0.5f);

    DrawTextEx(font, text, pos, fontSize, 1.0f, c);
}
