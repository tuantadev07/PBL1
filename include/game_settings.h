#pragma once

enum GameMode {
    GAME_MODE_PVP = 0,
    GAME_MODE_PVAI,
    GAME_MODE_AIVAI,
    GAME_MODE_COUNT
};

enum GameRule {
    GAME_RULE_LAST_TAKE_LOSE = 0,
    GAME_RULE_LAST_TAKE_WIN,
    GAME_RULE_COUNT
};

struct GameSettings {
    int minPileCount = 2;
    int maxPileCount = 10;

    int minInitialStoneCount = 5;
    int maxInitialStoneCount = 10;

    int maxTakePerMove = 0; // 0 = không giới hạn

    GameRule gameRule = GAME_RULE_LAST_TAKE_LOSE;
};

GameSettings load_game_settings();

