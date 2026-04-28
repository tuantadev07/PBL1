#include "screens/screen_help.h"

#include "app/game_app.h"
#include "assets/game_assets.h"
#include "raylib.h"
#include "render/theme.h"
#include "screens/screen_common.h"
#include "ui/ui.h"


namespace {
    constexpr int BTN_BACK_ID = 1;

    Rectangle BTN_BACK = {540, 600, 200, 75};

    const UIButtonStyle BTN_STYLE = ui_style_button_common();

} // 


void screen_help_update(GameAppState& app, float dt) {
    (void)dt;

    if (ui_button_is_clicked(BTN_BACK_ID, BTN_BACK) || IsKeyPressed(KEY_ESCAPE)) {
        app.currentScreen = APP_SCREEN_MAIN_MENU;
    }
}

void screen_help_draw() {
    screen_draw_background(theme_bg_color());

    const Rectangle screenRect = {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()};

    ui_draw_text_in_rect_with_outline(
        g_assets.fonts.common,
        u8"HƯỚNG DẪN",
        screenRect,
        52.0f,
        1.0f,
        0.5f,
        0.25f,
        2.0f,
        theme_text_color(),
        BLACK
    );

    ui_draw_text_in_rect(
        g_assets.fonts.common,
        u8"Help screen\nCách chơi: ",
        screenRect,
        28.0f,
        1.0f,
        0.5f,
        0.45f,
        theme_accent_color()
    );

    ui_draw_text_in_rect(
        g_assets.fonts.common,
        u8"ESC để quay lại",
        screenRect,
        24.0f,
        1.0f,
        0.5f,
        0.58f,
        theme_text_color()
    );

    screen_draw_button(BTN_BACK_ID, BTN_BACK, u8"QUAY LẠI", 36, BTN_STYLE);
}
