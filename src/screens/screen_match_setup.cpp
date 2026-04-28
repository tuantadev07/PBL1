#include "screens/screen_match_setup.h"

#include <cstdio>
#include "app/game_app.h"
#include "assets/game_assets.h"
#include "raylib.h"
#include "render/theme.h"
#include "screens/screens.h"
#include "ui/ui.h"

namespace {
    int _min_int(int a, int b) {
        return (a < b) ? a : b;
    }

    int _max_int(int a, int b) {
        return (a > b) ? a : b;
    }

    constexpr int BTN_MODE_1P_ID = 101;
    constexpr int BTN_MODE_2P_ID = 102;
    constexpr int BTN_MODE_BACK_ID = 103;

    constexpr int BTN_DIFF_VEASY_ID = 201;
    constexpr int BTN_DIFF_EASY_ID  = 202;
    constexpr int BTN_DIFF_MED_ID   = 203;
    constexpr int BTN_DIFF_HARD_ID  = 204;
    constexpr int BTN_DIFF_VHARD_ID = 205;
    constexpr int BTN_DIFF_NEXT_ID  = 206;
    constexpr int BTN_DIFF_BACK_ID  = 207;

    constexpr int BTN_NAMES_NEXT_ID = 301;
    constexpr int BTN_NAMES_BACK_ID = 302;

    constexpr int BTN_ROUND_RULE_WIN_ID = 401;
    constexpr int BTN_ROUND_RULE_LOSE_ID = 402;
    constexpr int BTN_ROUND_SWITCH_ID = 403;
    constexpr int BTN_ROUND_LIMIT_DEC_ID = 404;
    constexpr int BTN_ROUND_LIMIT_INC_ID = 405;
    constexpr int BTN_ROUND_PILE_DEC_ID = 406;
    constexpr int BTN_ROUND_PILE_INC_ID = 407;
    constexpr int BTN_ROUND_BACK_ID = 408;
    constexpr int BTN_ROUND_START_ID = 409;

    Rectangle BTN_MODE_1P = {160, 300, 420, 80};
    Rectangle BTN_MODE_2P = {700, 300, 420, 80};
    Rectangle BTN_MODE_BACK = {490, 600, 300, 70};

    Rectangle BTN_DIFF_VEASY = {110, 300, 300, 70};
    Rectangle BTN_DIFF_EASY  = {490, 300, 300, 70};
    Rectangle BTN_DIFF_MED   = {870, 300, 300, 70};
    Rectangle BTN_DIFF_HARD  = {300, 400, 300, 70};
    Rectangle BTN_DIFF_VHARD = {680, 400, 300, 70};
    Rectangle BTN_DIFF_NEXT  = {690, 600, 260, 70};
    Rectangle BTN_DIFF_BACK  = {330, 600, 260, 70};

    Rectangle INPUT_P1 = {390, 240, 500, 64};
    Rectangle INPUT_P2 = {390, 340, 500, 64};
    Rectangle BTN_NAMES_NEXT = {690, 600, 260, 70};
    Rectangle BTN_NAMES_BACK = {330, 600, 260, 70};
    Rectangle ROUND_PANEL = {170, 140, 940, 560};
    Rectangle ROUND_TITLE_RECT = {170, 162, 940, 40};
    Rectangle ROUND_RULE_LABEL = {218, 204, 844, 24};
    Rectangle BTN_ROUND_RULE_WIN = {218, 232, 408, 66};
    Rectangle BTN_ROUND_RULE_LOSE = {654, 232, 408, 66};
    Rectangle ROUND_SWITCH_LABEL = {218, 354, 300, 24};
    Rectangle BTN_ROUND_SWITCH = {858, 346, 180, 54};
    Rectangle ROUND_SWITCH_STATE_LABEL = {858, 406, 180, 20};
    Rectangle ROUND_TAKE_LABEL = {218, 454, 280, 24};
    Rectangle BTN_ROUND_LIMIT_DEC = {792, 436, 54, 56};
    Rectangle ROUND_TAKE_VALUE = {862, 432, 120, 64};
    Rectangle BTN_ROUND_LIMIT_INC = {998, 436, 54, 56};
    Rectangle ROUND_PILE_LABEL = {218, 554, 280, 24};
    Rectangle BTN_ROUND_PILE_DEC = {792, 536, 54, 56};
    Rectangle ROUND_PILE_VALUE = {862, 532, 120, 64};
    Rectangle BTN_ROUND_PILE_INC = {998, 536, 54, 56};
    Rectangle ROUND_PILE_RANGE_LABEL = {218, 592, 844, 20};
    Rectangle BTN_ROUND_BACK = {218, 614, 220, 62};
    Rectangle BTN_ROUND_START = {842, 614, 220, 62};

    const UIButtonStyle BTN_STYLE = ui_style_button_common();
    const UIButtonStyle BTN_SELECTED_STYLE = ui_style_button_common();

    const UIButtonStyle BTN_DISABLED_STYLE = {
        Color{82, 92, 118, 220},
        Color{82, 92, 118, 220},
        Color{82, 92, 118, 220},
        Color{36, 42, 58, 255}
    };

    const UITextInputStyle INPUT_STYLE = {
        Color{24, 28, 44, 255},
        Color{36, 42, 62, 255},
        Color{120, 130, 160, 255},
        theme_accent_color(),
        RAYWHITE,
        Color{170, 175, 190, 255}
    };

    int _clamp_int(int value, int minValue, int maxValue) {
        return _max_int(minValue, _min_int(value, maxValue));
    }

    int _min_pile_count(const GameSettings& settings) {
        return _max_int(1, settings.minPileCount);
    }

    int _max_pile_count(const GameSettings& settings) {
        return _max_int(_min_pile_count(settings), settings.maxPileCount);
    }

    int _default_pile_count(const GameSettings& settings) {
        return _clamp_int(3, _min_pile_count(settings), _max_pile_count(settings));
    }

    int _max_take_limit(const GameSettings& settings) {
        return _max_int(1, settings.maxInitialStoneCount);
    }

    int _default_take_limit(const GameSettings& settings) {
        if (settings.maxTakePerMove > 0) {
            return settings.maxTakePerMove;
        }

        return _max_int(1, _min_int(3, _max_take_limit(settings)));
    }

    const char* _difficulty_label(AIDifficulty difficulty) {
        switch (difficulty) {
            case AI_DIFFICULTY_VERY_EASY: return u8"RẤT DỄ";
            case AI_DIFFICULTY_EASY:      return u8"DỄ";
            case AI_DIFFICULTY_MEDIUM:    return u8"TRUNG BÌNH";
            case AI_DIFFICULTY_HARD:      return u8"KHÓ";
            case AI_DIFFICULTY_VERY_HARD: return u8"RẤT KHÓ";
            default: return "UNKNOWN";
        }
    }

    void _copy_or_default(char* out, int capacity, const char* source, const char* fallback) {
        if (source != nullptr && source[0] != '\0') {
            std::snprintf(out, capacity, "%s", source);
        } else {
            std::snprintf(out, capacity, "%s", fallback);
        }
    }

    MatchConfig _build_match_config(const MatchSetupState& setup) {
        MatchConfig match{};
        match.gameMode = setup.gameMode;

        match.players[0].type = PLAYER_TYPE_HUMAN;
        _copy_or_default(match.players[0].name, sizeof(match.players[0].name), setup.playerNames[0], u8"Người chơi 1");

        if (setup.gameMode == GAME_MODE_PVP) {
            match.players[1].type = PLAYER_TYPE_HUMAN;
            _copy_or_default(match.players[1].name, sizeof(match.players[1].name), setup.playerNames[1], u8"Người chơi 2");
        } else {
            match.players[1].type = PLAYER_TYPE_AI;
            match.players[1].difficulty = setup.difficulty;
            std::snprintf(match.players[1].name, sizeof(match.players[1].name), "AI");
        }

        return match;
    }

    GameSettings _build_round_settings(const GameAppState& app) {
        GameSettings settings = app.gameSettings;
        settings.gameRule = app.setup.gameRule;
        settings.maxTakePerMove = app.setup.useTakeLimit ? app.setup.takeLimit : 0;
        return settings;
    }

    const UIButtonStateTextures* _stepper_textures(bool isIncrease) {
        const UIButtonStateTextures& textures = isIncrease
            ? g_assets.buttons.play_setup_increase
            : g_assets.buttons.play_setup_decrease;

        if (textures.normal.id != 0) {
            return &textures;
        }

        if (g_assets.buttons.common.normal.id != 0) {
            return &g_assets.buttons.common;
        }

        return nullptr;
    }

    void _draw_stepper_symbol(Rectangle rect, bool isIncrease, Color color) {
        const Rectangle horizontalBar = {
            rect.x + rect.width * 0.24f,
            rect.y + rect.height * 0.46f,
            rect.width * 0.52f,
            rect.height * 0.10f
        };

        DrawRectangleRounded(horizontalBar, 0.4f, 6, color);

        if (isIncrease) {
            const Rectangle verticalBar = {
                rect.x + rect.width * 0.45f,
                rect.y + rect.height * 0.24f,
                rect.width * 0.10f,
                rect.height * 0.52f
            };
            DrawRectangleRounded(verticalBar, 0.4f, 6, color);
        }
    }

    void _draw_round_stepper_button(int buttonId, Rectangle rect, bool enabled, bool isIncrease) {
        const UIButtonState state = enabled ? ui_button_get_state(buttonId, rect) : UI_BTN_NORMAL;
        const UIButtonStateTextures* textures = _stepper_textures(isIncrease);

        if (textures != nullptr) {
            ui_button_draw_state_textures(rect, *textures, state, false);
            if (!enabled) {
                DrawRectangleRounded(rect, 0.18f, 8, Fade(BLACK, 0.28f));
            }
            return;
        }

        ui_button_draw(rect, enabled ? BTN_STYLE : BTN_DISABLED_STYLE, state);

        Color iconColor = Color{236, 240, 248, 255};
        if (enabled && state == UI_BTN_HOVER) iconColor = WHITE;
        if (enabled && state == UI_BTN_PRESSED) iconColor = Color{250, 214, 126, 255};
        if (!enabled) iconColor = Color{170, 178, 194, 255};

        _draw_stepper_symbol(rect, isIncrease, iconColor);
    }

    void _draw_round_switch(Rectangle rect, bool enabled, UIButtonState state) {
        if (g_assets.switches.play_limit.off.id != 0 && g_assets.switches.play_limit.on.id != 0) {
            const Texture2D tex = enabled ? g_assets.switches.play_limit.on : g_assets.switches.play_limit.off;
            const Rectangle src = {0.0f, 0.0f, (float)tex.width, (float)tex.height};
            DrawTexturePro(tex, src, rect, Vector2{0.0f, 0.0f}, 0.0f, WHITE);

            if (state == UI_BTN_HOVER) {
                DrawRectangleRounded(rect, 0.42f, 10, Fade(WHITE, 0.12f));
            } else if (state == UI_BTN_PRESSED) {
                DrawRectangleRounded(rect, 0.42f, 10, Fade(BLACK, 0.10f));
            }
            return;
        }

        const Color trackColor = enabled
            ? (state == UI_BTN_PRESSED ? Color{74, 173, 114, 255} : Color{92, 201, 128, 255})
            : Color{114, 122, 146, 255};
        const Color thumbColor = enabled ? RAYWHITE : Color{226, 232, 240, 255};

        DrawRectangleRounded(rect, 0.48f, 10, trackColor);
        DrawRectangleRoundedLinesEx(rect, 0.48f, 10, 2.0f, Color{24, 32, 50, 255});

        const float thumbSize = rect.height - 10.0f;
        const float thumbX = enabled
            ? (rect.x + rect.width - thumbSize - 5.0f)
            : (rect.x + 5.0f);

        DrawCircleV(
            Vector2{thumbX + thumbSize * 0.5f, rect.y + rect.height * 0.5f},
            thumbSize * 0.5f,
            thumbColor
        );
    }

    void _draw_round_value_box(Rectangle rect, int value, bool enabled) {
        const Color fillColor = enabled ? Color{24, 28, 44, 255} : Color{36, 40, 52, 240};
        const Color borderColor = enabled ? theme_accent_color() : Color{90, 98, 118, 255};
        const Color textColor = enabled ? RAYWHITE : Color{176, 184, 202, 255};

        DrawRectangleRec(rect, fillColor);
        DrawRectangleLinesEx(rect, 2.0f, borderColor);
        ui_draw_text_in_rect(
            g_assets.fonts.common,
            TextFormat("%d", value),
            rect,
            30.0f,
            1.0f,
            0.5f,
            0.5f,
            textColor
        );
    }

    void _draw_round_setup(const GameAppState& app) {
        const bool canDecreaseTake = app.setup.useTakeLimit && app.setup.takeLimit > 1;
        const bool canIncreaseTake = app.setup.useTakeLimit && app.setup.takeLimit < _max_take_limit(app.gameSettings);
        const bool canDecreasePiles = app.setup.pileCount > _min_pile_count(app.gameSettings);
        const bool canIncreasePiles = app.setup.pileCount < _max_pile_count(app.gameSettings);

        DrawRectangleRounded(ROUND_PANEL, 0.10f, 10, Color{14, 22, 44, 235});
        DrawRectangleRoundedLinesEx(ROUND_PANEL, 0.10f, 10, 3.0f, theme_accent_color());

        ui_draw_text_in_rect_with_outline(
            g_assets.fonts.common,
            u8"CẤU HÌNH VÁN ĐẤU",
            ROUND_TITLE_RECT,
            38.0f,
            1.0f,
            0.5f,
            0.5f,
            2.0f,
            theme_text_color(),
            BLACK
        );

        ui_draw_text_in_rect(
            g_assets.fonts.common,
            u8"Chọn luật kết thúc",
            ROUND_RULE_LABEL,
            22.0f,
            1.0f,
            0.0f,
            0.5f,
            Color{233, 239, 250, 255}
        );

        screen_draw_button(
            BTN_ROUND_RULE_WIN_ID,
            BTN_ROUND_RULE_WIN,
            u8"BỐC CUỐI THẮNG",
            28.0f,
            BTN_SELECTED_STYLE,
            app.setup.gameRule == GAME_RULE_LAST_TAKE_WIN
        );
        screen_draw_button(
            BTN_ROUND_RULE_LOSE_ID,
            BTN_ROUND_RULE_LOSE,
            u8"BỐC CUỐI THUA",
            28.0f,
            BTN_SELECTED_STYLE,
            app.setup.gameRule == GAME_RULE_LAST_TAKE_LOSE
        );

        ui_draw_text_in_rect(
            g_assets.fonts.common,
            u8"Có giới hạn không?",
            ROUND_SWITCH_LABEL,
            22.0f,
            1.0f,
            0.0f,
            0.5f,
            Color{233, 239, 250, 255}
        );

        _draw_round_switch(BTN_ROUND_SWITCH, app.setup.useTakeLimit, ui_button_get_state(BTN_ROUND_SWITCH_ID, BTN_ROUND_SWITCH));

        ui_draw_text_in_rect(
            g_assets.fonts.common,
            app.setup.useTakeLimit ? u8"ĐANG BẬT" : u8"ĐANG TẮT",
            ROUND_SWITCH_STATE_LABEL,
            18.0f,
            1.0f,
            0.5f,
            0.5f,
            app.setup.useTakeLimit ? Color{177, 244, 188, 255} : Color{206, 214, 228, 255}
        );

        ui_draw_text_in_rect(
            g_assets.fonts.common,
            u8"Giới hạn mỗi lượt",
            ROUND_TAKE_LABEL,
            22.0f,
            1.0f,
            0.0f,
            0.5f,
            Color{233, 239, 250, 255}
        );

        _draw_round_stepper_button(BTN_ROUND_LIMIT_DEC_ID, BTN_ROUND_LIMIT_DEC, canDecreaseTake, false);
        _draw_round_value_box(ROUND_TAKE_VALUE, app.setup.takeLimit, app.setup.useTakeLimit);
        _draw_round_stepper_button(BTN_ROUND_LIMIT_INC_ID, BTN_ROUND_LIMIT_INC, canIncreaseTake, true);

        if (!app.setup.useTakeLimit) {
            DrawRectangleRounded(ROUND_TAKE_VALUE, 0.18f, 8, Fade(BLACK, 0.28f));
        }

        ui_draw_text_in_rect(
            g_assets.fonts.common,
            u8"Số lượng đống",
            ROUND_PILE_LABEL,
            22.0f,
            1.0f,
            0.0f,
            0.5f,
            Color{233, 239, 250, 255}
        );

        _draw_round_stepper_button(BTN_ROUND_PILE_DEC_ID, BTN_ROUND_PILE_DEC, canDecreasePiles, false);
        _draw_round_value_box(ROUND_PILE_VALUE, app.setup.pileCount, true);
        _draw_round_stepper_button(BTN_ROUND_PILE_INC_ID, BTN_ROUND_PILE_INC, canIncreasePiles, true);

        char pileRangeText[64];
        std::snprintf(
            pileRangeText,
            sizeof(pileRangeText),
            u8"Từ %d đến %d đống",
            _min_pile_count(app.gameSettings),
            _max_pile_count(app.gameSettings)
        );
        ui_draw_text_in_rect(
            g_assets.fonts.common,
            pileRangeText,
            ROUND_PILE_RANGE_LABEL,
            18.0f,
            1.0f,
            0.0f,
            0.5f,
            Color{206, 220, 240, 255}
        );

        screen_draw_button(BTN_ROUND_BACK_ID, BTN_ROUND_BACK, u8"QUAY LẠI", 28.0f, BTN_STYLE);
        screen_draw_button(BTN_ROUND_START_ID, BTN_ROUND_START, u8"BẮT ĐẦU", 28.0f, BTN_STYLE);
    }
}

void screen_match_setup_update(GameAppState& app, float dt) {
    (void)dt;

    switch (app.setup.step) {
        case MATCH_SETUP_STEP_MODE: {
            if (ui_button_is_clicked(BTN_MODE_1P_ID, BTN_MODE_1P)) {
                app.setup.gameMode = GAME_MODE_PVAI;
                app.setup.step = MATCH_SETUP_STEP_DIFFICULTY;
                app.setup.activeNameInput = -1;
            }

            if (ui_button_is_clicked(BTN_MODE_2P_ID, BTN_MODE_2P)) {
                app.setup.gameMode = GAME_MODE_PVP;
                app.setup.step = MATCH_SETUP_STEP_NAMES;
                app.setup.activeNameInput = 0;
            }

            if (IsKeyPressed(KEY_ESCAPE) || ui_button_is_clicked(BTN_MODE_BACK_ID, BTN_MODE_BACK)) {
                game_app_reset_match_setup(app);
                app.currentScreen = APP_SCREEN_MAIN_MENU;
                app.currentOverlay = APP_OVERLAY_NONE;
            }
            break;
        }

        case MATCH_SETUP_STEP_DIFFICULTY: {
            if (ui_button_is_clicked(BTN_DIFF_VEASY_ID, BTN_DIFF_VEASY)) app.setup.difficulty = AI_DIFFICULTY_VERY_EASY;
            if (ui_button_is_clicked(BTN_DIFF_EASY_ID, BTN_DIFF_EASY))   app.setup.difficulty = AI_DIFFICULTY_EASY;
            if (ui_button_is_clicked(BTN_DIFF_MED_ID, BTN_DIFF_MED))     app.setup.difficulty = AI_DIFFICULTY_MEDIUM;
            if (ui_button_is_clicked(BTN_DIFF_HARD_ID, BTN_DIFF_HARD))   app.setup.difficulty = AI_DIFFICULTY_HARD;
            if (ui_button_is_clicked(BTN_DIFF_VHARD_ID, BTN_DIFF_VHARD)) app.setup.difficulty = AI_DIFFICULTY_VERY_HARD;

            if (IsKeyPressed(KEY_ESCAPE) || ui_button_is_clicked(BTN_DIFF_BACK_ID, BTN_DIFF_BACK)) {
                app.setup.step = MATCH_SETUP_STEP_MODE;
                app.setup.activeNameInput = -1;
            }

            if (ui_button_is_clicked(BTN_DIFF_NEXT_ID, BTN_DIFF_NEXT)) {
                app.setup.step = MATCH_SETUP_STEP_NAMES;
                app.setup.activeNameInput = 0;
            }
            break;
        }

        case MATCH_SETUP_STEP_NAMES: {
            if (ui_text_input_is_clicked(INPUT_P1)) {
                app.setup.activeNameInput = 0;
            } else if (app.setup.gameMode == GAME_MODE_PVP && ui_text_input_is_clicked(INPUT_P2)) {
                app.setup.activeNameInput = 1;
            } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                app.setup.activeNameInput = -1;
            }

            ui_text_input_update(
                app.setup.playerNames[0],
                (int)sizeof(app.setup.playerNames[0]),
                app.setup.activeNameInput == 0
            );

            ui_text_input_update(
                app.setup.playerNames[1],
                (int)sizeof(app.setup.playerNames[1]),
                app.setup.gameMode == GAME_MODE_PVP && app.setup.activeNameInput == 1
            );

            if (IsKeyPressed(KEY_ESCAPE) || ui_button_is_clicked(BTN_NAMES_BACK_ID, BTN_NAMES_BACK)) {
                app.setup.activeNameInput = -1;
                if (app.setup.gameMode == GAME_MODE_PVP) {
                    app.setup.step = MATCH_SETUP_STEP_MODE;
                } else {
                    app.setup.step = MATCH_SETUP_STEP_DIFFICULTY;
                }
            }

            if (ui_button_is_clicked(BTN_NAMES_NEXT_ID, BTN_NAMES_NEXT)) {
                app.setup.activeNameInput = -1;
                app.setup.step = MATCH_SETUP_STEP_ROUND_SETUP;
            }
            break;
        }

        case MATCH_SETUP_STEP_ROUND_SETUP: {
            if (ui_button_is_clicked(BTN_ROUND_RULE_WIN_ID, BTN_ROUND_RULE_WIN)) {
                app.setup.gameRule = GAME_RULE_LAST_TAKE_WIN;
            }

            if (ui_button_is_clicked(BTN_ROUND_RULE_LOSE_ID, BTN_ROUND_RULE_LOSE)) {
                app.setup.gameRule = GAME_RULE_LAST_TAKE_LOSE;
            }

            if (ui_button_is_clicked(BTN_ROUND_SWITCH_ID, BTN_ROUND_SWITCH)) {
                app.setup.useTakeLimit = !app.setup.useTakeLimit;
            }

            if (app.setup.useTakeLimit &&
                app.setup.takeLimit > 1 &&
                ui_button_is_clicked(BTN_ROUND_LIMIT_DEC_ID, BTN_ROUND_LIMIT_DEC)) {
                --app.setup.takeLimit;
            }

            if (app.setup.useTakeLimit &&
                app.setup.takeLimit < _max_take_limit(app.gameSettings) &&
                ui_button_is_clicked(BTN_ROUND_LIMIT_INC_ID, BTN_ROUND_LIMIT_INC)) {
                ++app.setup.takeLimit;
            }

            if (app.setup.pileCount > _min_pile_count(app.gameSettings) &&
                ui_button_is_clicked(BTN_ROUND_PILE_DEC_ID, BTN_ROUND_PILE_DEC)) {
                --app.setup.pileCount;
            }

            if (app.setup.pileCount < _max_pile_count(app.gameSettings) &&
                ui_button_is_clicked(BTN_ROUND_PILE_INC_ID, BTN_ROUND_PILE_INC)) {
                ++app.setup.pileCount;
            }

            if (IsKeyPressed(KEY_ESCAPE) || ui_button_is_clicked(BTN_ROUND_BACK_ID, BTN_ROUND_BACK)) {
                app.setup.step = MATCH_SETUP_STEP_NAMES;
            }

            if (IsKeyPressed(KEY_ENTER) || ui_button_is_clicked(BTN_ROUND_START_ID, BTN_ROUND_START)) {
                const MatchConfig matchConfig = _build_match_config(app.setup);
                const GameSettings roundSettings = _build_round_settings(app);
                screen_play_start_new_match(matchConfig, roundSettings, app.setup.pileCount);
                app.currentScreen = APP_SCREEN_PLAY;
                app.currentOverlay = APP_OVERLAY_NONE;
            }
            break;
        }

        default: break;
    }
}

void screen_match_setup_draw(const GameAppState& app) {
    ClearBackground(theme_bg_color());
    screen_draw_background(g_assets.images.bg_match_setup, Color{35, 30, 55, 255});
    const Rectangle screenRect = {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()};

    ui_draw_text_in_rect_with_outline(
        g_assets.fonts.common,
        u8"THIẾT LẬP TRẬN",
        screenRect,
        84.0f,
        2.0f,
        0.5f,
        0.1f,
        3.0f,
        theme_text_color(),
        BLACK
    );

    if (app.setup.step == MATCH_SETUP_STEP_MODE) {
        ui_draw_text_in_rect_with_outline(
            g_assets.fonts.common,
            u8"CHỌN CHẾ ĐỘ CHƠI",
            screenRect,
            64.0f,
            2.0f,
            0.5f,
            0.25f,
            2.0f,
            theme_accent_color(),
            BLACK
        );

        screen_draw_button(BTN_MODE_1P_ID, BTN_MODE_1P, u8"1 NGƯỜI CHƠI", 32.0f, BTN_STYLE);
        screen_draw_button(BTN_MODE_2P_ID, BTN_MODE_2P, u8"2 NGƯỜI CHƠI", 32.0f, BTN_STYLE);
        screen_draw_button(BTN_MODE_BACK_ID, BTN_MODE_BACK, u8"QUAY LẠI", 32.0f, BTN_STYLE);
        return;
    }

    if (app.setup.step == MATCH_SETUP_STEP_DIFFICULTY) {
        ui_draw_text_in_rect_with_outline(
            g_assets.fonts.common,
            u8"CHỌN ĐỘ KHÓ AI",
            screenRect,
            64.0f,
            2.0f,
            0.5f,
            0.25f,
            2.0f,
            theme_accent_color(),
            BLACK
        );

        screen_draw_button(BTN_DIFF_VEASY_ID, BTN_DIFF_VEASY, _difficulty_label(AI_DIFFICULTY_VERY_EASY), 32.0f, BTN_SELECTED_STYLE,
            app.setup.difficulty == AI_DIFFICULTY_VERY_EASY);
        screen_draw_button(BTN_DIFF_EASY_ID, BTN_DIFF_EASY, _difficulty_label(AI_DIFFICULTY_EASY), 32.0f, BTN_SELECTED_STYLE,
            app.setup.difficulty == AI_DIFFICULTY_EASY);
        screen_draw_button(BTN_DIFF_MED_ID, BTN_DIFF_MED, _difficulty_label(AI_DIFFICULTY_MEDIUM), 32.0f, BTN_SELECTED_STYLE,
            app.setup.difficulty == AI_DIFFICULTY_MEDIUM);
        screen_draw_button(BTN_DIFF_HARD_ID, BTN_DIFF_HARD, _difficulty_label(AI_DIFFICULTY_HARD), 32.0f, BTN_SELECTED_STYLE,
            app.setup.difficulty == AI_DIFFICULTY_HARD);
        screen_draw_button(BTN_DIFF_VHARD_ID, BTN_DIFF_VHARD, _difficulty_label(AI_DIFFICULTY_VERY_HARD), 32.0f, BTN_SELECTED_STYLE,
            app.setup.difficulty == AI_DIFFICULTY_VERY_HARD);

        screen_draw_button(BTN_DIFF_BACK_ID, BTN_DIFF_BACK, u8"QUAY LẠI", 32.0f, BTN_STYLE);
        screen_draw_button(BTN_DIFF_NEXT_ID, BTN_DIFF_NEXT, u8"TIẾP TỤC", 32.0f, BTN_STYLE);
        return;
    }

    if (app.setup.step == MATCH_SETUP_STEP_NAMES) {
        ui_draw_text_in_rect_with_outline(
            g_assets.fonts.common,
            u8"NHẬP TÊN NGƯỜI CHƠI",
            screenRect,
            64.0f,
            2.0f,
            0.5f,
            0.25f,
            2.0f,
            theme_accent_color(),
            BLACK
        );

        ui_draw_text_in_rect_with_outline(
            g_assets.fonts.common,
            u8"Người chơi 1",
            Rectangle{220.0f, 250.0f, 150.0f, 44.0f},
            32.0f,
            1.5f,
            0.0f,
            0.5f,
            2.0f,
            theme_text_color(),
            BLACK
        );

        ui_text_input_draw(
            INPUT_P1,
            app.setup.playerNames[0],
            u8"Nhập tên Người chơi 1",
            g_assets.fonts.common,
            32.0f,
            app.setup.activeNameInput == 0,
            INPUT_STYLE
        );

        if (app.setup.gameMode == GAME_MODE_PVP) {
            ui_draw_text_in_rect_with_outline(
                g_assets.fonts.common,
                u8"Người chơi 2",
                Rectangle{220.0f, 350.0f, 150.0f, 44.0f},
                32.0f,
                1.5f,
                0.0f,
                0.5f,
                2.0f,
                theme_text_color(),
                BLACK
            );

            ui_text_input_draw(
                INPUT_P2,
                app.setup.playerNames[1],
                u8"Nhập tên Người chơi 2",
                g_assets.fonts.common,
                32.0f,
                app.setup.activeNameInput == 1,
                INPUT_STYLE
            );
        } else {
            ui_draw_text_in_rect_with_outline(
                g_assets.fonts.common,
                u8"AI sẽ dùng tên mặc định",
                Rectangle{420.0f, 350.0f, 420.0f, 44.0f},
                32.0f,
                1.5f,
                0.0f,
                0.5f,
                2.0f,
                theme_text_color(),
                BLACK
            );
        }

        screen_draw_button(BTN_NAMES_BACK_ID, BTN_NAMES_BACK, u8"QUAY LẠI", 32.0f, BTN_STYLE);
        screen_draw_button(BTN_NAMES_NEXT_ID, BTN_NAMES_NEXT, u8"TIẾP TỤC", 32.0f, BTN_STYLE);
        return;
    }

    if (app.setup.step == MATCH_SETUP_STEP_ROUND_SETUP) {
        _draw_round_setup(app);
    }
}
