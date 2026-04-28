#include "overlays/overlay_pause.h"

#include "raylib.h"
#include "app/game_app.h"
#include "assets/game_assets.h"
#include "render/theme.h"
#include "overlays/overlay_common.h"
#include "screens/screen_play.h"
#include "ui/ui.h"

namespace {

    constexpr int BTN_RESUME_ID = 1;
    constexpr int BTN_MENU_ID = 2;


    Rectangle BTN_RESUME = {490, 300, 300, 70};
    Rectangle BTN_MENU   = {490, 400, 300, 70};

    const UIButtonStyle BTN_STYLE = {
        Color{75, 110, 175, 255},
        Color{95, 130, 195, 255},
        Color{60, 90, 150, 255},
        Color{20, 24, 36, 255}
    };
}

void overlay_pause_update(GameAppState& app, float dt) {
    (void)dt;

    if (ui_button_is_clicked(BTN_RESUME_ID, BTN_RESUME) || IsKeyPressed(KEY_R) || IsKeyPressed(KEY_ESCAPE)) {
        app.currentOverlay = APP_OVERLAY_NONE;
        return;
    }

    if (ui_button_is_clicked(BTN_MENU_ID, BTN_MENU) || IsKeyPressed(KEY_M)) {
        game_app_open_confirm_exit(app, EXIT_TARGET_PLAY_TO_MAIN_MENU);
        
        return;
    }
}

void overlay_pause_draw() {
    overlay_draw_backdrop(0.55f);

    Rectangle panel = {360, 180, 560, 360};
    overlay_draw_panel(panel, Color{22, 26, 40, 245}, theme_accent_color());

    const char* textTitle = u8"TẠM DỪNG";
    ui_draw_text_in_rect(
        g_assets.fonts.common,
        textTitle,
        panel,
        44.0f,
        1.0f,
        0.5f,
        0.1f,
        theme_accent_color()
    );

    overlay_draw_button(
        BTN_RESUME_ID,
        BTN_RESUME,
        u8"TIẾP TỤC",
        g_assets.fonts.common,
        26.0f,
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
        26.0f,
        BTN_STYLE,
        DARKGRAY,
        BLACK,
        WHITE
    );

    /*

    

    overlay_draw_button(
    ui_button_draw(BTN_RESUME, BTN_STYLE, resumeState);
    ui_button_draw_text(
        BTN_RESUME,
        u8"TIẾP TỤC",
        g_assets.fonts.common,
        26.0f,
        resumeState,
        DARKGRAY,
        BLACK,
        WHITE
    );

    UIButtonState menuState = ui_button_get_state(BTN_MENU_ID, BTN_MENU);
    ui_button_draw(BTN_MENU, BTN_STYLE, menuState);
    ui_button_draw_text(
        BTN_MENU,
        u8"VỀ MENU CHÍNH",
        g_assets.fonts.common,
        26.0f,
        menuState,
        DARKGRAY,
        BLACK,
        WHITE
    );
    */
}
