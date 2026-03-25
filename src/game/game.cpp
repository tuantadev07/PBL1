#include "game.h"

#include "random.h"


void init_game (GameState& game, int pileCount, int gameMode) {
    init_vector(game.piles, pileCount);

    game.gameMode = gameMode;
    game.currentTurn = 0;
    game.totalStones = 0;

    for (int pileIndex = 0; pileIndex < game.piles.size; ++pileIndex) {
        set(game.piles, pileIndex, random_int(1, 10));
        game.totalStones += get(game.piles, pileIndex);
    }
}

void free_game (GameState& game) {
    free(game.piles);
    game.gameMode = 0;
    game.gameRule = 0;
    game.currentTurn = 0;
    game.totalStones = 0;
}

void next_turn (GameState& game) {
    game.currentTurn = 1 - game.currentTurn;
}

bool is_game_over(const GameState& game) {
    return game.totalStones == 0  ;
}

bool is_valid_move(const GameState& game, const Move& move) {
    if (move.pileIndex < 0 || move.pileIndex >= game.piles.size) {
        return false;
    }
    
    if (move.stoneCount <= 0) {
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
    gameCopy.gameMode = game.gameMode;
    gameCopy.currentTurn = game.currentTurn;
    gameCopy.totalStones = game.totalStones;
    gameCopy.gameRule = game.gameRule;

    return gameCopy;
}
