#pragma once

#include "common/array.h"
#include "game/game_settings.h"


struct Move {
    int pileIndex = 0;
    int stoneCount = 0;
};

struct GameState {
    Array piles;
    MatchConfig matchConfig;
    GameSettings settings;
    int currentTurn = 0;
    int totalStones = 0;
};


// khởi tạo game
void init_game (GameState& game, int pileCount, const MatchConfig& matchConfig, const GameSettings& settings);
// giải phóng dung lượng game.piles
void free_game (GameState& game);

// đổi lượt chơi
void next_turn (GameState& game);

// trả về 1 nếu game kết thúc
bool is_game_over(const GameState& game);

// trả về 1 nếu nước đi hợp lệ
bool is_valid_move(const GameState& game, const Move& move);

// trả về 1 nếu nhận nước đi thành công
bool apply_move (GameState& game, Move move);

GameState copy_game_state(const GameState& game);
