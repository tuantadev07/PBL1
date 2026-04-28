#pragma once

enum GameMode {
    GAME_MODE_PVP = 0,
    GAME_MODE_PVAI,
    GAME_MODE_AIVAI,
};

enum GameRule {
    GAME_RULE_LAST_TAKE_LOSE = 0,
    GAME_RULE_LAST_TAKE_WIN,
};

enum PlayerType {
    PLAYER_TYPE_HUMAN = 0,
    PLAYER_TYPE_AI,
};

enum AIDifficulty {
    AI_DIFFICULTY_VERY_EASY = 0,
    AI_DIFFICULTY_EASY,
    AI_DIFFICULTY_MEDIUM,
    AI_DIFFICULTY_HARD,
    AI_DIFFICULTY_VERY_HARD,
};

struct PlayerConfig {
    PlayerType type = PLAYER_TYPE_HUMAN;
    AIDifficulty difficulty = AI_DIFFICULTY_MEDIUM;
    char name[32] = "";
};

struct MatchConfig {
    GameMode gameMode = GAME_MODE_PVAI;
    PlayerConfig players[2];
};

struct GameSettings {
    int minPileCount = 1;
    int maxPileCount = 10;

    int minInitialStoneCount = 5;
    int maxInitialStoneCount = 10;

    int maxTakePerMove = 0; // 0 = không giới hạn

    GameRule gameRule = GAME_RULE_LAST_TAKE_LOSE;
};

GameSettings load_game_settings();

