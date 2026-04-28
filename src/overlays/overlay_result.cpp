#include "overlays/overlay_result.h"

#include <cstdio>

#include "app/game_app.h"
#include "assets/game_assets.h"
#include "overlays/overlay_common.h"
#include "raylib.h"
#include "render/theme.h"
#include "screens/screen_play.h"
#include "ui/ui_text.h"

namespace {
    constexpr int BTN_CONTINUE_ID = 1;
    constexpr int BTN_MENU_ID = 2;

    Rectangle BTN_CONTINUE = {336, 458, 248, 68};
    Rectangle BTN_MENU = {696, 458, 248, 68};

    const UIButtonStyle BTN_STYLE = {
        Color{75, 110, 175, 255},
        Color{95, 130, 195, 255},
        Color{60, 90, 150, 255},
        Color{20, 24, 36, 255}
    };
}

void overlay_result_update(GameAppState& app, float dt) {
    (void)dt;

    if (ui_button_is_clicked(BTN_CONTINUE_ID, BTN_CONTINUE) || IsKeyPressed(KEY_ENTER)) {
        screen_play_continue_match();
        app.currentOverlay = APP_OVERLAY_NONE;
        app.resultMessage[0] = '\0';
        return;
    }

    if (ui_button_is_clicked(BTN_MENU_ID, BTN_MENU) || IsKeyPressed(KEY_ESCAPE)) {
        screen_play_shutdown();
        game_app_reset_match_setup(app);
        app.currentOverlay = APP_OVERLAY_NONE;
        app.currentScreen = APP_SCREEN_MAIN_MENU;
    }
}

void overlay_result_draw(const GameAppState& app) {
    overlay_draw_backdrop(0.68f);

    Rectangle panel = {230, 148, 820, 438};
    overlay_draw_panel(panel, Color{18, 24, 40, 245}, theme_accent_color());

    ui_draw_text_in_rect_with_outline(
        g_assets.fonts.common,
        u8"KẾT QUẢ TRẬN ĐẤU",
        Rectangle{panel.x, panel.y + 22.0f, panel.width, 48.0f},
        38.0f,
        1.0f,
        0.5f,
        0.5f,
        2.0f,
        theme_accent_color(),
        BLACK
    );

    const char* message = (app.resultMessage[0] != '\0')
        ? app.resultMessage
        : u8"TRẬN ĐẤU KẾT THÚC";

    ui_draw_text_in_rect(
        g_assets.fonts.common,
        message,
        Rectangle{panel.x + 48.0f, panel.y + 102.0f, panel.width - 96.0f, 52.0f},
        30.0f,
        1.0f,
        0.5f,
        0.5f,
        theme_text_color()
    );

    char scoreLine[160];
    std::snprintf(
        scoreLine,
        sizeof(scoreLine),
        u8"%s  %d  -  %d  %s",
        screen_play_get_actor_name(0),
        screen_play_get_score(0),
        screen_play_get_score(1),
        screen_play_get_actor_name(1)
    );

    ui_draw_text_in_rect_with_outline(
        g_assets.fonts.common,
        scoreLine,
        Rectangle{panel.x + 48.0f, panel.y + 188.0f, panel.width - 96.0f, 42.0f},
        32.0f,
        1.0f,
        0.5f,
        0.5f,
        1.5f,
        Color{255, 222, 126, 255},
        BLACK
    );

    ui_draw_text_in_rect(
        g_assets.fonts.common,
        u8"Chơi tiếp sẽ giữ nguyên tỉ số, về menu chính sẽ mất tỉ số",
        Rectangle{panel.x + 46.0f, panel.y + 274.0f, panel.width - 92.0f, 44.0f},
        21.0f,
        1.0f,
        0.5f,
        0.5f,
        Color{216, 228, 246, 255}
    );

    overlay_draw_button(
        BTN_CONTINUE_ID,
        BTN_CONTINUE,
        u8"CHƠI TIẾP",
        g_assets.fonts.common,
        24.0f,
        BTN_STYLE,
        DARKGRAY,
        BLACK,
        WHITE
    );

    overlay_draw_button(
        BTN_MENU_ID,
        BTN_MENU,
        u8"VỀ MENU CHÍNH",
        g_assets.fonts.common,
        24.0f,
        BTN_STYLE,
        DARKGRAY,
        BLACK,
        WHITE
    );
}
