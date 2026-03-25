#pragma once

#include "vector.h"


struct Move {
    int pileIndex = 0;
    int stoneCount = 0;
};

struct GameState {
    vector_int piles;
    int gameMode = 0;
    int gameRule = 0;
    int currentTurn = 0;
    int totalStones = 0;
};

// khởi tạo game
void init_game (GameState& game, int pileCount, int gameMode);
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
