#include "game/game.h"
#include "common/random.h"


void init_game (GameState& game, int pileCount, const MatchConfig& matchConfig, const GameSettings& settings) {
    init_array(game.piles, pileCount);

    game.matchConfig = matchConfig;
    game.settings = settings;
    game.currentTurn = 0;
    game.totalStones = 0;

    for (int pileIndex = 0; pileIndex < game.piles.size; ++pileIndex) {
        set(game.piles, pileIndex, 
            random_int(
                settings.minInitialStoneCount, 
                settings.maxInitialStoneCount
            )
        );
        game.totalStones += get(game.piles, pileIndex);
    }
}

void free_game (GameState& game) {
    free_array(game.piles);
    game.totalStones = 0;
}

void next_turn (GameState& game) {
    game.currentTurn = 1 - game.currentTurn;
}

bool is_game_over(const GameState& game) {
    return game.totalStones == 0;
}

bool is_valid_move(const GameState& game, const Move& move) {
    if (move.pileIndex < 0 || move.pileIndex >= game.piles.size) {
        return false;
    }
    
    if (move.stoneCount <= 0) {
        return false;
    }

    int maxTakePerMove = game.settings.maxTakePerMove;
    if (maxTakePerMove > 0 && move.stoneCount > maxTakePerMove) {
        return false;
    }

    if (move.stoneCount > get(game.piles, move.pileIndex)) {
        return false;
    }

    return true;
}

bool apply_move (GameState& game, Move move) {
    if (is_valid_move(game, move)) {
        set(game.piles, move.pileIndex, get(game.piles, move.pileIndex) - move.stoneCount);
        game.totalStones -= move.stoneCount;

        return true;
    }

    return false;
}

GameState copy_game_state(const GameState& game) {
    GameState gameCopy;

    gameCopy.piles = copy(game.piles);
    gameCopy.matchConfig = game.matchConfig;
    gameCopy.currentTurn = game.currentTurn;
    gameCopy.totalStones = game.totalStones;
    gameCopy.settings = game.settings;

    return gameCopy;
}
