#include "app/game_app.h"

#include <cstdio>

#include "overlays/overlays.h"
#include "screens/screens.h"
#include "ui/ui.h"

namespace {
    int _min_int(int a, int b) {
        return (a < b) ? a : b;
    }

    int _max_int(int a, int b) {
        return (a > b) ? a : b;
    }

    int _clamp_int(int value, int minValue, int maxValue) {
        return _max_int(minValue, _min_int(value, maxValue));
    }

    int _default_match_pile_count(const GameSettings& settings) {
        const int minPileCount = _max_int(1, settings.minPileCount);
        const int maxPileCount = _max_int(minPileCount, settings.maxPileCount);
        return _clamp_int(3, minPileCount, maxPileCount);
    }

    int _default_take_limit(const GameSettings& settings) {
        if (settings.maxTakePerMove > 0) {
            return settings.maxTakePerMove;
        }

        return _max_int(1, _min_int(3, _max_int(1, settings.maxInitialStoneCount)));
    }

    void _update_current_screen(GameAppState& app, float dt) {
        switch (app.currentScreen) {
            case APP_SCREEN_MAIN_MENU:   screen_main_menu_update(app, dt); break;
            case APP_SCREEN_MATCH_SETUP: screen_match_setup_update(app, dt); break;
            case APP_SCREEN_HELP:        screen_help_update(app, dt); break;
            case APP_SCREEN_CREDITS:     screen_credits_update(app, dt); break;
            case APP_SCREEN_SETTINGS:    screen_settings_update(app, dt); break;
            case APP_SCREEN_PLAY:        screen_play_update(app, dt); break;
            default: break;
        }
    }

    
    void _draw_current_screen(const GameAppState& app) {
        switch (app.currentScreen) {
            case APP_SCREEN_MAIN_MENU:   screen_main_menu_draw(); break;
            case APP_SCREEN_MATCH_SETUP: screen_match_setup_draw(app); break;
            case APP_SCREEN_HELP:        screen_help_draw(); break;
            case APP_SCREEN_CREDITS:     screen_credits_draw(); break;
            case APP_SCREEN_SETTINGS:    screen_settings_draw(); break;
            case APP_SCREEN_PLAY:        screen_play_draw(); break;
            default: break;
        }
    }

    void _update_current_overlay(GameAppState& app, float dt) {
        switch (app.currentOverlay) {
            case APP_OVERLAY_CONFIRM_EXIT: overlay_confirm_exit_update(app, dt); break;
            case APP_OVERLAY_PAUSE:        overlay_pause_update(app, dt); break;
            case APP_OVERLAY_RESULT:       overlay_result_update(app, dt); break;
            default: break;
        }
    }
    void _draw_current_overlay(const GameAppState& app) {
        switch (app.currentOverlay) {
            case APP_OVERLAY_CONFIRM_EXIT: overlay_confirm_exit_draw(app); break;
            case APP_OVERLAY_PAUSE:        overlay_pause_draw(); break;
            case APP_OVERLAY_RESULT:       overlay_result_draw(app); break;
            default: break;
        }
    }

}

void game_app_reset_match_setup(GameAppState& app) {
    app.setup.step = MATCH_SETUP_STEP_MODE;
    app.setup.gameMode = GAME_MODE_PVAI;
    app.setup.difficulty = AI_DIFFICULTY_MEDIUM;
    std::snprintf(app.setup.playerNames[0], sizeof(app.setup.playerNames[0]), u8"Người chơi 1");
    std::snprintf(app.setup.playerNames[1], sizeof(app.setup.playerNames[1]), u8"Người chơi 2");
    app.setup.activeNameInput = -1;
    app.setup.gameRule = app.gameSettings.gameRule;
    app.setup.useTakeLimit = app.gameSettings.maxTakePerMove > 0;
    app.setup.takeLimit = _default_take_limit(app.gameSettings);
    app.setup.pileCount = _default_match_pile_count(app.gameSettings);
}

void game_app_show_result(GameAppState& app, const char* message) {
    if (message != nullptr) {
        std::snprintf(app.resultMessage, sizeof(app.resultMessage), "%s", message);
    } else {
        app.resultMessage[0] = '\0';
    }

    app.currentOverlay = APP_OVERLAY_RESULT;
}

void game_app_open_confirm_exit(GameAppState& app, ExitTarget target) {
    app.exitTarget = target;
    app.currentOverlay = APP_OVERLAY_CONFIRM_EXIT;
}

void game_app_init(GameAppState& app) {
    app.currentScreen = APP_SCREEN_MAIN_MENU;
    app.currentOverlay = APP_OVERLAY_NONE;
    app.exitTarget = EXIT_TARGET_NONE;
    app.shouldQuit = false;
    app.gameSettings = load_game_settings();
    app.resultMessage[0] = '\0';
    game_app_reset_match_setup(app);
}

void game_app_update(GameAppState& app, float dt) {
    if (app.currentOverlay == APP_OVERLAY_NONE) {
        _update_current_screen(app, dt);
        return;
    }

    _update_current_overlay(app, dt);
}

void game_app_draw(const GameAppState& app) {
    const bool hasOverlay = (app.currentOverlay != APP_OVERLAY_NONE);

    ui_button_set_interaction_enabled(!hasOverlay);
    _draw_current_screen(app);

    ui_button_set_interaction_enabled(true); // mở lại cho ovẻlay

   _draw_current_overlay(app);
}
