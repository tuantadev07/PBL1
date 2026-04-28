#include "screens/screen_play.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "ai/ai.h"
#include "app/game_app.h"
#include "assets/game_assets.h"
#include "game/game.h"
#include "raylib.h"
#include "render/theme.h"
#include "screens/screen_common.h"
#include "ui/ui_button.h"
#include "ui/ui_text.h"

namespace {
    int _min_int(int a, int b) {
        return (a < b) ? a : b;
    }

    int _max_int(int a, int b) {
        return (a > b) ? a : b;
    }

    float _min_float(float a, float b) {
        return (a < b) ? a : b;
    }

    float _max_float(float a, float b) {
        return (a > b) ? a : b;
    }

    float _clamp_float(float value, float minValue, float maxValue) {
        return _max_float(minValue, _min_float(value, maxValue));
    }

    constexpr int BTN_CONFIRM_ID = 9001;

    constexpr int kMaxAnimatedClouds = 32;
    constexpr float kAiBaseThinkDelay = 1.1f;
    constexpr float kAiStepThinkDelay = 0.25f;
    constexpr float kFlightDuration = 0.82f;

    const UIButtonStyle BTN_STYLE = {
        Color{78, 126, 205, 255},
        Color{98, 148, 229, 255},
        Color{64, 106, 178, 255},
        Color{18, 26, 44, 255}
    };

    struct CloudFlightState {
        bool active = false;
        Move move{};
        int actorIndex = 0;
        double startTime = 0.0;
        Vector2 startPositions[kMaxAnimatedClouds]{};
        Vector2 size{};
    };

    bool g_hasSession = false;
    bool g_gameInitialized = false;

    MatchConfig g_matchConfig{};
    GameSettings g_roundSettings{};
    GameState g_game{};

    int g_seriesScore[2] = {0, 0};
    int g_roundPileCount = 3;

    Move g_selectedMove{-1, 0};
    Move g_aiMove{-1, 0};
    bool g_aiThinking = false;
    double g_aiThinkUntil = 0.0;
    CloudFlightState g_cloudFlight{};

    bool _uses_take_limit() {
        return g_roundSettings.maxTakePerMove > 0;
    }

    const char* _mode_label(GameMode mode) {
        switch (mode) {
            case GAME_MODE_PVP:   return u8"Đối kháng";
            case GAME_MODE_PVAI:  return u8"Người chơi và AI";
            default: return u8"UNKNƠ";
        }
    }

    const char* _rule_label(GameRule rule) {
        return (rule == GAME_RULE_LAST_TAKE_WIN)
            ? u8"Bốc cuối thắng"
            : u8"Bốc cuối thua";
    }

    const char* _actor_name_internal(int actorIndex) {
        if (actorIndex < 0 || actorIndex > 1) {
            return "";
        }

        const char* name = g_matchConfig.players[actorIndex].name;
        if (name[0] != '\0') {
            return name;
        }

        if (g_matchConfig.players[actorIndex].type == PLAYER_TYPE_AI) {
            return u8"AI";
        }

        return (actorIndex == 0) ? u8"Người chơi 1" : u8"Người chơi 2";
    }

    Color _actor_color(int actorIndex, bool active) {
        if (actorIndex == 0) {
            return active ? Color{255, 212, 94, 255} : Color{232, 238, 250, 255};
        }

        return active ? Color{255, 212, 94, 255} : Color{232, 238, 250, 255};
    }

    Rectangle _confirm_button_rect() {
        return Rectangle{490.0f, 622.0f, 300.0f, 64.0f};
    }

    Rectangle _board_rect() {
        return Rectangle{104.0f, 176.0f, (float)GetScreenWidth() - 208.0f, 376.0f};
    }

    float _row_height() {
        return _board_rect().height / (float)_max_int(1, g_game.piles.size);
    }

    float _max_visible_clouds() {
        int maxClouds = 1;
        for (int pileIndex = 0; pileIndex < g_game.piles.size; ++pileIndex) {
            maxClouds = _max_int(maxClouds, get(g_game.piles, pileIndex));
        }
        return (float)maxClouds;
    }

    Vector2 _cloud_size() {
        const Rectangle board = _board_rect();
        const float labelWidth = 136.0f;
        const float leftPadding = 22.0f;
        const float maxClouds = _max_visible_clouds();
        const float gap = 10.0f;
        const float widthByBoard = (board.width - labelWidth - leftPadding - gap * (maxClouds - 1.0f)) / _max_float(1.0f, maxClouds);
        const float widthByRow = (_row_height() * 0.72f) / 0.58f;
        const float maxWidth = (g_game.piles.size <= 5) ? 82.0f : 68.0f;
        const float minWidth = (g_game.piles.size <= 5) ? 42.0f : 28.0f;
        const float width = _min_float(widthByBoard, widthByRow);
        const float clampedWidth = _clamp_float(width, minWidth, maxWidth);
        return Vector2{clampedWidth, clampedWidth * 0.58f};
    }

    Rectangle _cloud_rect(int pileIndex, int cloudIndex) {
        const Rectangle board = _board_rect();
        const Vector2 cloudSize = _cloud_size();
        const float labelWidth = 136.0f;
        const float leftPadding = 22.0f;
        const float gap = 10.0f;
        const float rowTop = board.y + _row_height() * (float)pileIndex;
        const float startX = board.x + labelWidth + leftPadding;
        const float y = rowTop + (_row_height() - cloudSize.y) * 0.5f;

        return Rectangle{
            startX + cloudIndex * (cloudSize.x + gap),
            y,
            cloudSize.x,
            cloudSize.y
        };
    }

    int _cloud_button_id(int pileIndex, int cloudIndex) {
        return 10000 + pileIndex * 100 + cloudIndex;
    }

    bool _is_ai_turn() {
        if (!g_hasSession || !g_gameInitialized) {
            return false;
        }

        return g_game.matchConfig.players[g_game.currentTurn].type == PLAYER_TYPE_AI;
    }

    bool _is_human_turn() {
        return g_hasSession && g_gameInitialized && !_is_ai_turn();
    }

    float _ai_think_delay() {
        const AIDifficulty difficulty = g_game.matchConfig.players[g_game.currentTurn].difficulty;
        return kAiBaseThinkDelay + kAiStepThinkDelay * (float)difficulty;
    }

    int _winner_index() {
        int winner = g_game.currentTurn;
        if (g_roundSettings.gameRule == GAME_RULE_LAST_TAKE_LOSE) {
            winner = 1 - winner;
        }
        return winner;
    }

    void _clear_selection() {
        g_selectedMove = Move{-1, 0};
    }

    void _begin_turn() {
        _clear_selection();
        g_aiThinking = false;
        g_aiMove = Move{-1, 0};

        if (_is_ai_turn()) {
            g_aiThinking = true;
            g_aiMove = choose_ai_move(g_game);
            g_aiThinkUntil = GetTime() + _ai_think_delay();
        }
    }

    void _initialize_round(bool resetScore) {
        if (g_gameInitialized) {
            free_game(g_game);
            g_gameInitialized = false;
        }

        init_game(g_game, g_roundPileCount, g_matchConfig, g_roundSettings);
        g_gameInitialized = true;
        g_cloudFlight = {};
        g_aiThinking = false;
        g_aiThinkUntil = 0.0;
        g_aiMove = Move{-1, 0};
        _clear_selection();

        if (resetScore) {
            g_seriesScore[0] = 0;
            g_seriesScore[1] = 0;
        }

        g_game.currentTurn = GetRandomValue(0, 1);
        _begin_turn();
    }

    void _draw_cloud_shape(Rectangle rect, Color fillColor, Color outlineColor, Color accentColor) {
        const Rectangle shadowRect = {
            rect.x + 3.0f,
            rect.y + 4.0f,
            rect.width,
            rect.height
        };

        DrawEllipse(
            (int)(shadowRect.x + shadowRect.width * 0.46f),
            (int)(shadowRect.y + shadowRect.height * 0.70f),
            shadowRect.width * 0.34f,
            shadowRect.height * 0.20f,
            Color{11, 18, 35, 75}
        );

        DrawRectangleRounded(
            Rectangle{rect.x + rect.width * 0.16f, rect.y + rect.height * 0.42f, rect.width * 0.68f, rect.height * 0.34f},
            0.55f,
            8,
            fillColor
        );
        DrawCircleV(Vector2{rect.x + rect.width * 0.30f, rect.y + rect.height * 0.50f}, rect.height * 0.22f, fillColor);
        DrawCircleV(Vector2{rect.x + rect.width * 0.48f, rect.y + rect.height * 0.32f}, rect.height * 0.30f, fillColor);
        DrawCircleV(Vector2{rect.x + rect.width * 0.68f, rect.y + rect.height * 0.48f}, rect.height * 0.24f, fillColor);

        DrawCircleLinesV(Vector2{rect.x + rect.width * 0.30f, rect.y + rect.height * 0.50f}, rect.height * 0.22f, outlineColor);
        DrawCircleLinesV(Vector2{rect.x + rect.width * 0.48f, rect.y + rect.height * 0.32f}, rect.height * 0.30f, outlineColor);
        DrawCircleLinesV(Vector2{rect.x + rect.width * 0.68f, rect.y + rect.height * 0.48f}, rect.height * 0.24f, outlineColor);
        DrawRectangleRoundedLinesEx(
            Rectangle{rect.x + rect.width * 0.16f, rect.y + rect.height * 0.42f, rect.width * 0.68f, rect.height * 0.34f},
            0.55f,
            8,
            2.0f,
            outlineColor
        );

        DrawCircleV(Vector2{rect.x + rect.width * 0.40f, rect.y + rect.height * 0.30f}, rect.height * 0.09f, accentColor);
    }

    void _draw_cloud_button(Rectangle rect, int takeCount, UIButtonState state, bool isSelected, bool isSelectable) {
        if (g_assets.buttons.cloud.normal.id != 0) {
            ui_button_draw_state_textures(rect, g_assets.buttons.cloud, isSelectable ? state : UI_BTN_NORMAL, isSelected);
            if (!isSelectable) {
                DrawRectangleRounded(rect, 0.30f, 8, Fade(BLACK, 0.22f));
            }
        } else {
            Color fillColor = Color{245, 248, 255, 255};
            Color outlineColor = Color{70, 96, 160, 255};
            Color accentColor = Color{196, 225, 255, 255};

            if (!isSelectable) {
                fillColor = Color{184, 193, 208, 255};
                outlineColor = Color{100, 108, 124, 255};
                accentColor = Color{164, 172, 188, 255};
            } else if (isSelected) {
                fillColor = Color{255, 246, 213, 255};
                outlineColor = Color{255, 188, 72, 255};
                accentColor = Color{255, 223, 138, 255};
            } else if (state == UI_BTN_HOVER) {
                fillColor = Color{255, 255, 255, 255};
                outlineColor = Color{88, 132, 211, 255};
                accentColor = Color{220, 239, 255, 255};
            } else if (state == UI_BTN_PRESSED) {
                fillColor = Color{228, 236, 251, 255};
                outlineColor = Color{58, 92, 160, 255};
                accentColor = Color{181, 212, 245, 255};
            }

            _draw_cloud_shape(rect, fillColor, outlineColor, accentColor);
        }

        ui_draw_text_in_rect_with_outline(
            g_assets.fonts.common,
            TextFormat("%d", takeCount),
            rect,
            rect.height * 0.54f,
            1.0f,
            0.50f,
            0.50f,
            1.0f,
            isSelectable ? Color{36, 56, 98, 255} : Color{84, 88, 102, 255},
            WHITE
        );
    }

    void _start_flight(const Move& move, int actorIndex) {
        g_cloudFlight = {};
        g_cloudFlight.active = true;
        g_cloudFlight.move = move;
        g_cloudFlight.actorIndex = actorIndex;
        g_cloudFlight.startTime = GetTime();
        g_cloudFlight.size = _cloud_size();

        const int animatedCount = _min_int(move.stoneCount, kMaxAnimatedClouds);
        for (int i = 0; i < animatedCount; ++i) {
            const Rectangle rect = _cloud_rect(move.pileIndex, i);
            g_cloudFlight.startPositions[i] = Vector2{rect.x, rect.y};
        }

        _clear_selection();
        g_aiThinking = false;
    }

    void _finish_flight(GameAppState& app) {
        const Move move = g_cloudFlight.move;
        g_cloudFlight.active = false;

        if (!apply_move(g_game, move)) {
            _begin_turn();
            return;
        }

        if (is_game_over(g_game)) {
            const int winnerIndex = _winner_index();
            ++g_seriesScore[winnerIndex];

            char resultText[96];
            std::snprintf(resultText, sizeof(resultText), u8"%s Thắng ván này!", _actor_name_internal(winnerIndex));
            game_app_show_result(app, resultText);
            PlaySound(g_assets.sounds.end);
            return;
        }

        next_turn(g_game);
        _begin_turn();
    }

    void _draw_score_card(Rectangle rect, int actorIndex, bool active) {
        const Color borderColor = _actor_color(actorIndex, active);
        const Color fillColor = active ? Color{20, 28, 56, 225} : Color{18, 22, 42, 205};

        DrawRectangleRounded(rect, 0.16f, 10, fillColor);
        DrawRectangleRoundedLinesEx(rect, 0.16f, 10, 2.5f, borderColor);

        ui_draw_text_in_rect_with_outline(
            g_assets.fonts.common,
            _actor_name_internal(actorIndex),
            Rectangle{rect.x + 12.0f, rect.y + 8.0f, rect.width - 24.0f, 34.0f},
            26.0f,
            1.0f,
            0.5f,
            0.5f,
            1.0f,
            borderColor,
            BLACK
        );

        ui_draw_text_in_rect(
            g_assets.fonts.common,
            TextFormat(u8"Thắng: %d", g_seriesScore[actorIndex]),
            Rectangle{rect.x + 12.0f, rect.y + 44.0f, rect.width - 24.0f, 24.0f},
            20.0f,
            1.0f,
            0.5f,
            0.5f,
            theme_text_color()
        );
    }

    void _draw_header() {
        const float panelWidth = 280.0f;
        const float panelHeight = 84.0f;
        const float gap = 36.0f;
        const float totalWidth = panelWidth * 2.0f + gap;
        const float startX = (GetScreenWidth() - totalWidth) * 0.5f;
        const float topY = 26.0f;

        _draw_score_card(Rectangle{startX, topY, panelWidth, panelHeight}, 0, g_game.currentTurn == 0);
        _draw_score_card(Rectangle{startX + panelWidth + gap, topY, panelWidth, panelHeight}, 1, g_game.currentTurn == 1);

        // ui_draw_text_in_rect_with_outline(
        //     g_assets.fonts.common,
        //     _mode_label(g_matchConfig.gameMode),
        //     Rectangle{0.0f, 115.0f, (float)GetScreenWidth(), 22.0f},
        //     20.0f,
        //     2.0f,
        //     0.5f,
        //     0.5f,
        //     2.0f,
        //     theme_accent_color(),
        //     BLACK
        // );

        char summary[96];
        if (_uses_take_limit()) {
            std::snprintf(summary, sizeof(summary), u8"%s    I    Tối đa %d/lượtt", _rule_label(g_roundSettings.gameRule), g_roundSettings.maxTakePerMove);
        } else {
            std::snprintf(summary, sizeof(summary), u8"%s    I    Không giới hạn", _rule_label(g_roundSettings.gameRule));
        }

        ui_draw_text_in_rect_with_outline(
            g_assets.fonts.common,
            summary,
            Rectangle{0.0f, 115.0f, (float)GetScreenWidth(), 20.0f},
            18.0f,
            2.0f,
            0.5f,
            0.5f,
            2.0f,
            theme_accent_color(),      
            BLACK
        );
    }

    void _draw_pile_labels_and_clouds() {
        const Rectangle board = _board_rect();
        DrawRectangleRounded(board, 0.08f, 8, Color{16, 22, 38, 155});
        DrawRectangleRoundedLinesEx(board, 0.08f, 8, 2.0f, Color{182, 219, 255, 115});

        for (int pileIndex = 0; pileIndex < g_game.piles.size; ++pileIndex) {
            int visibleCloudCount = get(g_game.piles, pileIndex);
            if (g_cloudFlight.active && g_cloudFlight.move.pileIndex == pileIndex) {
                visibleCloudCount -= g_cloudFlight.move.stoneCount;
            }

            const float rowTop = board.y + _row_height() * (float)pileIndex;
            const Rectangle labelRect = {board.x + 16.0f, rowTop + 8.0f, 118.0f, _row_height() - 16.0f};
            ui_draw_text_in_rect(
                g_assets.fonts.common,
                TextFormat(u8"Cụm %d", pileIndex + 1),
                labelRect,
                22.0f,
                1.0f,
                0.0f,
                0.5f,
                Color{234, 240, 255, 255}
            );

            for (int cloudIndex = 0; cloudIndex < visibleCloudCount; ++cloudIndex) {
                const Rectangle cloudRect = _cloud_rect(pileIndex, cloudIndex);
                const Move candidateMove = {pileIndex, cloudIndex + 1};
                const bool isSelectable =
                    _is_human_turn() &&
                    !g_cloudFlight.active &&
                    !g_aiThinking &&
                    is_valid_move(g_game, candidateMove);
                const UIButtonState state = isSelectable ? ui_button_get_state(_cloud_button_id(pileIndex, cloudIndex), cloudRect) : UI_BTN_NORMAL;
                const bool isSelected =
                    (g_selectedMove.pileIndex == pileIndex) &&
                    (cloudIndex < g_selectedMove.stoneCount);

                _draw_cloud_button(cloudRect, cloudIndex + 1, state, isSelected, isSelectable);
            }
        }
    }

    void _draw_flying_clouds() {
        if (!g_cloudFlight.active) {
            return;
        }

        const int count = _min_int(g_cloudFlight.move.stoneCount, kMaxAnimatedClouds);
        const float elapsed = (float)(GetTime() - g_cloudFlight.startTime);
        const float t = _clamp_float(elapsed / kFlightDuration, 0.0f, 1.0f);
        const float eased = 1.0f - std::pow(1.0f - t, 3.0f);
        const float endX = (g_cloudFlight.actorIndex == 0)
            ? (-g_cloudFlight.size.x - 80.0f)
            : ((float)GetScreenWidth() + 80.0f);

        for (int i = 0; i < count; ++i) {
            const Vector2 start = g_cloudFlight.startPositions[i];
            const float x = start.x + (endX - start.x) * eased;
            const float y = start.y - 56.0f * std::sin(eased * 3.1415926f) - 26.0f * eased;
            const Rectangle rect = {x, y, g_cloudFlight.size.x, g_cloudFlight.size.y};
            const unsigned char alpha = (unsigned char)(255.0f * (1.0f - eased));
            const Texture2D selectedTexture = g_assets.buttons.cloud.selected;

            if (selectedTexture.id != 0) {
                const Rectangle src = {0.0f, 0.0f, (float)selectedTexture.width, (float)selectedTexture.height};
                DrawTexturePro(
                    selectedTexture,
                    src,
                    rect,
                    Vector2{0.0f, 0.0f},
                    0.0f,
                    Color{255, 255, 255, alpha}
                );
            } else {
                _draw_cloud_shape(
                    rect,
                    Color{255, 248, 226, alpha},
                    Color{255, 203, 87, alpha},
                    Color{255, 229, 153, alpha}
                );
            }
        }
    }

    void _draw_selection_summary() {
        if (g_selectedMove.pileIndex < 0 || g_cloudFlight.active || g_aiThinking || !_is_human_turn()) {
            return;
        }

        ui_draw_text_in_rect_with_outline(
            g_assets.fonts.common,
            TextFormat(
                u8"Đã chọn",
                g_selectedMove.pileIndex + 1,
                g_selectedMove.stoneCount
            ),
            Rectangle{0.0f, 565.0f, (float)GetScreenWidth(), 24.0f},
            24.0f,
            2.0f,
            0.5f,
            0.5f,
            2.0f,
            theme_text_color(),
            BLACK
        );

        screen_draw_button(BTN_CONFIRM_ID, _confirm_button_rect(), u8"ĐỒNG Ý", 30.0f, BTN_STYLE);
    }

    void _draw_turn_banner() {
        if (g_aiThinking) {
            ui_draw_text_in_rect_with_outline(
                g_assets.fonts.common,
                u8"AI đang suy nghĩ...",
                Rectangle{0.0f, 145.0f, (float)GetScreenWidth(), 24.0f},
                22.0f,
                2.0f,
                0.5f,
                0.5f,
                2.0f,
                theme_accent_color(),
                BLACK
            );
            return;
        }

        // ui_draw_text_in_rect_with_outline(
        //     g_assets.fonts.common,
        //     TextFormat(u8"Lượt của %s", _actor_name_internal(g_game.currentTurn)),
        //     Rectangle{0.0f, 156.0f, (float)GetScreenWidth(), 24.0f},
        //     22.0f,
        //     1.0f,
        //     0.5f,
        //     0.5f,
        //     2.0f,
        //     _actor_color(g_game.currentTurn, true),
        //     BLACK
        // );
    }

    void _draw_footer_hint() {
        const char* hintText = _is_human_turn()
            ? u8"Chọn một đám mây để bốc ngay từ bên trái đến vị trí đó, rồi nhấn Enter hoặc nút đồng ý"
            : u8"Nhấn ESC để mở tạm dừng";

        ui_draw_text_in_rect_with_outline(
            g_assets.fonts.common,
            hintText,
            Rectangle{0.0f, 700.0f, (float)GetScreenWidth(), 20.0f},
            18.0f,
            2.0f,
            0.5f,
            0.5f,
            2.0f,
            theme_accent_color(),
            BLACK
        );
    }

    void _handle_human_cloud_selection() {
        for (int pileIndex = 0; pileIndex < g_game.piles.size; ++pileIndex) {
            const int stonesInPile = get(g_game.piles, pileIndex);
            for (int cloudIndex = 0; cloudIndex < stonesInPile; ++cloudIndex) {
                const Move candidateMove = {pileIndex, cloudIndex + 1};
                if (!is_valid_move(g_game, candidateMove)) {
                    continue;
                }

                const Rectangle cloudRect = _cloud_rect(pileIndex, cloudIndex);
                if (ui_button_is_clicked(_cloud_button_id(pileIndex, cloudIndex), cloudRect)) {
                    g_selectedMove = candidateMove;
                    return;
                }
            }
        }
    }

    void _try_confirm_human_move() {
        if (g_selectedMove.pileIndex < 0 || !is_valid_move(g_game, g_selectedMove)) {
            return;
        }

        if (IsKeyPressed(KEY_ENTER) || ui_button_is_clicked(BTN_CONFIRM_ID, _confirm_button_rect())) {
            _start_flight(g_selectedMove, g_game.currentTurn);
        }
    }
}

void screen_play_start_new_match(const MatchConfig& matchConfig, const GameSettings& settings, int pileCount) {
    if (g_gameInitialized) {
        free_game(g_game);
        g_gameInitialized = false;
    }

    g_matchConfig = matchConfig;
    g_roundSettings = settings;
    g_hasSession = true;
    g_cloudFlight = {};
    g_aiThinking = false;
    g_aiThinkUntil = 0.0;
    g_aiMove = Move{-1, 0};
    g_roundPileCount = _max_int(1, pileCount);
    _clear_selection();
    _initialize_round(true);
}

void screen_play_continue_match() {
    if (!g_hasSession) {
        return;
    }

    _initialize_round(false);
}

void screen_play_shutdown() {
    if (g_gameInitialized) {
        free_game(g_game);
        g_gameInitialized = false;
    }

    g_hasSession = false;
    g_matchConfig = {};
    g_roundSettings = {};
    g_seriesScore[0] = 0;
    g_seriesScore[1] = 0;
    g_roundPileCount = 3;
    g_aiThinking = false;
    g_aiThinkUntil = 0.0;
    g_aiMove = Move{-1, 0};
    g_cloudFlight = {};
    _clear_selection();
}

const char* screen_play_get_actor_name(int actorIndex) {
    return _actor_name_internal(actorIndex);
}

int screen_play_get_score(int actorIndex) {
    if (actorIndex < 0 || actorIndex > 1) {
        return 0;
    }

    return g_seriesScore[actorIndex];
}

void screen_play_update(GameAppState& app, float dt) {
    (void)dt;

    if (!g_hasSession) {
        app.currentScreen = APP_SCREEN_MAIN_MENU;
        return;
    }

    if (!g_gameInitialized) {
        app.currentScreen = APP_SCREEN_MAIN_MENU;
        return;
    }

    if (IsKeyPressed(KEY_ESCAPE) && !g_cloudFlight.active) {
        app.currentOverlay = APP_OVERLAY_PAUSE;
        return;
    }

    if (g_cloudFlight.active) {
        if ((GetTime() - g_cloudFlight.startTime) >= kFlightDuration) {
            _finish_flight(app);
        }
        return;
    }

    if (g_aiThinking) {
        if (GetTime() >= g_aiThinkUntil && is_valid_move(g_game, g_aiMove)) {
            _start_flight(g_aiMove, g_game.currentTurn);
        }
        return;
    }

    if (!_is_human_turn()) {
        _begin_turn();
        return;
    }

    _handle_human_cloud_selection();
    _try_confirm_human_move();
}

void screen_play_draw() {
    ClearBackground(theme_bg_color());
    screen_draw_background(g_assets.images.bg_play, Color{28, 46, 38, 255});

    if (!g_gameInitialized) {
        return;
    }

    _draw_header();
    _draw_turn_banner();
    _draw_pile_labels_and_clouds();
    _draw_flying_clouds();
    _draw_selection_summary();
    _draw_footer_hint();

    ui_draw_text_in_rect_with_outline(
        g_assets.fonts.common,
        TextFormat(u8"Tổng còn lại: %d", g_game.totalStones),
        Rectangle{0.0f, 592.0f, (float)GetScreenWidth(), 22.0f},
        20.0f,
        2.0f,
        0.5f,
        0.5f,
        2.0f,
        theme_accent_color(),    
        BLACK
    );
}
