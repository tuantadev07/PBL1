#include "screens/screen_main_menu.h"

#include "app/game_app.h"
#include "assets/game_assets.h"
#include "raylib.h"
#include "render/theme.h"
#include "screens/screen_common.h"
#include "ui/ui.h"

namespace {
    constexpr int BTN_PLAY_ID = 1;
    constexpr int BTN_SETTINGS_ID = 2;
    constexpr int BTN_HELP_ID = 3;
    constexpr int BTN_CREDITS_ID = 4;
    constexpr int BTN_QUIT_ID = 5;

    Rectangle BTN_PLAY     = {540, 250, 200, 75};
    Rectangle BTN_SETTINGS = {540, 338, 200, 75};
    Rectangle BTN_HELP     = {540, 426, 200, 75};
    Rectangle BTN_CREDITS  = {540, 514, 200, 75};
    Rectangle BTN_QUIT     = {540, 602, 200, 75};

    const UIButtonStyle BTN_STYLE = ui_style_button_common();

    void _draw_title(Rectangle rect, Font font, const char* text, float fontSize, Color color) {
        ui_draw_text_in_rect_with_outline(
            font,
            text,
            rect,
            fontSize,
            2.0f,
            0.5f,
            0.1f,
            3.0f,
            color,
            BLACK
        );
    }

} //

void screen_main_menu_update(GameAppState& app, float dt) {
    (void)dt;

    if (ui_button_is_clicked(BTN_PLAY_ID, BTN_PLAY)) {
        game_app_reset_match_setup(app);
        app.currentOverlay = APP_OVERLAY_NONE;
        app.currentScreen = APP_SCREEN_MATCH_SETUP;
        return;
    }

    if (ui_button_is_clicked(BTN_SETTINGS_ID, BTN_SETTINGS)) {
        app.currentOverlay = APP_OVERLAY_NONE;
        app.currentScreen = APP_SCREEN_SETTINGS;
        return;
    }

    if (ui_button_is_clicked(BTN_HELP_ID, BTN_HELP)) {
        app.currentOverlay = APP_OVERLAY_NONE;
        app.currentScreen = APP_SCREEN_HELP;
        return;
    }

    if (ui_button_is_clicked(BTN_CREDITS_ID, BTN_CREDITS)) {
        app.currentOverlay = APP_OVERLAY_NONE;
        app.currentScreen = APP_SCREEN_CREDITS;
        return;
    }

    if (IsKeyPressed(KEY_ESCAPE) || ui_button_is_clicked(BTN_QUIT_ID, BTN_QUIT)) {
        game_app_open_confirm_exit(app, EXIT_TARGET_APP);
    }
}

void screen_main_menu_draw() {
    ClearBackground(theme_bg_color());

    screen_draw_background(g_assets.images.bg_main_menu, BLACK);

    _draw_title(
        Rectangle{0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()},
        g_assets.fonts.title,
        "NIM GAME",
        128.0f,
        GOLD
    );

    screen_draw_button(BTN_PLAY_ID, BTN_PLAY, u8"CHƠI", 36.0f, BTN_STYLE);
    screen_draw_button(BTN_SETTINGS_ID, BTN_SETTINGS, u8"CÀI ĐẶT", 36.0f, BTN_STYLE);
    screen_draw_button(BTN_HELP_ID, BTN_HELP, u8"HƯỚNG DẪN", 36.0f, BTN_STYLE);
    screen_draw_button(BTN_CREDITS_ID, BTN_CREDITS, u8"THÔNG TIN", 36.0f, BTN_STYLE);
    screen_draw_button(BTN_QUIT_ID, BTN_QUIT, u8"THOÁT", 36.0f, BTN_STYLE);
}
