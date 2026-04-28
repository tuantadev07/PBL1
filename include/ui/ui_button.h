#pragma once

#include "raylib.h"

struct UIButtonStateTextures;

enum UIButtonState {
    UI_BTN_NORMAL = 0,
    UI_BTN_HOVER,
    UI_BTN_PRESSED
};

struct UIButtonStyle {
    Color normal;
    Color hover;
    Color pressed;
    Color border;
};

void ui_button_set_interaction_enabled(bool enabled);
void ui_button_reset_active();

UIButtonState ui_button_get_state(int buttonId, Rectangle rect);
bool ui_button_is_clicked(int buttonId, Rectangle rect);

void ui_button_draw(Rectangle rect, const UIButtonStyle& style, UIButtonState state);
void ui_button_draw_state_textures(Rectangle rect, const UIButtonStateTextures& textures, UIButtonState state, bool isSelected = false);
void ui_button_draw_text(Rectangle rect, const char* text, Font font, float fontSize, UIButtonState state, Color normal, Color hover, Color pressed);
