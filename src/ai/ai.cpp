#include "ai.h"

#include "random.h"

Move choose_random_move(const GameState& game) {
    Move move;
    
    while (true) {
        move.pileIndex = random_int(0, game.piles.size-1);

        int StoneCountInPile = get(game.piles, move.pileIndex);
        if (StoneCountInPile <= 0) {
            continue;
        }

        int maxTakeLimit = StoneCountInPile;
        int maxTakePerMove = game.settings.maxTakePerMove;
        
        if (maxTakePerMove > 0 && maxTakeLimit > maxTakePerMove) {
            maxTakeLimit = maxTakePerMove;
        }

        move.stoneCount = random_int(1, maxTakeLimit);

        break;
    }

    return move;
}

Move choose_ai_move(const GameState& game) {
    AIDifficulty difficulty = game.matchConfig.players[game.currentTurn].difficulty;

    if (difficulty == AI_DIFFICULTY_VERY_EASY) {
        return choose_random_move(game);
    }

    return choose_random_move(game);
}