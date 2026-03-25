#include "ai.h"

#include "random.h"

Move choose_random_move(const GameState& game) {
    Move move;
    
    while (true) {
        move.pileIndex = random_int(0, game.piles.size-1);

        int maxStone = get(game.piles, move.pileIndex);
        if (maxStone <= 0) {
            continue;
        }

        move.stoneCount = random_int(1, maxStone);
        break;
    }

    return move;
}
