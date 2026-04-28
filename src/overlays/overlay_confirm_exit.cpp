#include "overlays/overlay_confirm_exit.h"

#include "raylib.h"
#include "app/game_app.h"
#include "assets/game_assets.h"
#include "render/theme.h"
#include "overlays/overlay_common.h"
#include "screens/screen_play.h"
#include "ui/ui.h"

namespace {
    constexpr int BTN_YES_ID = 1;
    constexpr int BTN_NO_ID = 2;

    Rectangle BTN_YES = {430, 420, 180, 70};
    Rectangle BTN_NO  = {670, 420, 180, 70};

    const UIButtonStyle BTN_STYLE = {
        Color{75, 110, 175, 255},
        Color{95, 130, 195, 255},
        Color{60, 90, 150, 255},
        Color{20, 24, 36, 255}
    };

    const char* _message_for_target(ExitTarget target) {
        switch (target) {
            case EXIT_TARGET_APP:
                return u8"Bạn có muốn thoát game không?";
            case EXIT_TARGET_PLAY_TO_MAIN_MENU:
                return u8"Bạn có muốn về menu chính không?\nTiến trình trận đấu hiện tại sẽ bị mất.";
            default:
                return u8"Bạn có chắc chắn không?";
        }
    }

    void _close_overlay(GameAppState& app) {
        app.currentOverlay = APP_OVERLAY_NONE;
        app.exitTarget = EXIT_TARGET_NONE;
    }

    void _cancel_exit(GameAppState& app) {
        if (app.exitTarget == EXIT_TARGET_PLAY_TO_MAIN_MENU) {
            app.currentOverlay = APP_OVERLAY_PAUSE;
            app.exitTarget = EXIT_TARGET_NONE;
            return;
        }

        _close_overlay(app);
    }

    void _confirm_exit(GameAppState& app) {
        switch (app.exitTarget) {
            case EXIT_TARGET_APP:
                app.shouldQuit = true;
                break;
            case EXIT_TARGET_PLAY_TO_MAIN_MENU:
                screen_play_shutdown();
                game_app_reset_match_setup(app);
                app.currentScreen = APP_SCREEN_MAIN_MENU;
                break;
            default:
                break;
        }

        app.currentOverlay = APP_OVERLAY_NONE;
        app.exitTarget = EXIT_TARGET_NONE;
    }
}

void overlay_confirm_exit_update(GameAppState& app, float dt) {
    (void)dt;

    if (ui_button_is_clicked(BTN_YES_ID, BTN_YES) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_Y)) {
        _confirm_exit(app);
        return;
    }

    if (ui_button_is_clicked(BTN_NO_ID, BTN_NO) || IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_N)) {
        _cancel_exit(app);
        return;
    }
}

void overlay_confirm_exit_draw(const GameAppState& app) {
    overlay_draw_backdrop(0.60f);

    Rectangle panel = {300, 200, 680, 360};
    overlay_draw_panel(panel, Color{22, 26, 40, 245}, theme_accent_color());

    const char* title = u8"XÁC NHẬN";
    const char* message = _message_for_target(app.exitTarget);

    ui_draw_text_in_rect(
        g_assets.fonts.common,
        title,
        panel,
        44.0f,
        1.0f,
        0.5f,
        0.1f,
        theme_accent_color()
    );

    ui_draw_text_in_rect(
        g_assets.fonts.common,
        message,
        panel,
        28.0f,
        1.0f,
        0.5f,
        0.3f,
        theme_text_color()
    );

    overlay_draw_button(
        BTN_YES_ID,
        BTN_YES,
        u8"CÓ",
        g_assets.fonts.common,
        28.0f,
        BTN_STYLE,
        DARKGRAY,
        BLACK,
        WHITE
    );

    overlay_draw_button(
        BTN_NO_ID,
        BTN_NO,
        u8"KHÔNG",
        g_assets.fonts.common,
        28.0f,
        BTN_STYLE,
        DARKGRAY,
        BLACK,
        WHITE
    );
}
