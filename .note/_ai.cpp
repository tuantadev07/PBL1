#include "ai.h"
#include "random.h"


// * =================================================
// *                HÀM NỘI BỘ CÙA FILE
// * =================================================

namespace {
    int _get_take_limit(const GameState& game, int pileIndex) {
        int stonesInPile = get(game.piles, pileIndex);
        int maxTakePerMove = game.settings.maxTakePerMove;

        if (maxTakePerMove <= 0) return stonesInPile; // khong giới hạn
        return (stonesInPile < maxTakePerMove) ? stonesInPile : maxTakePerMove;
    }

    int _get_optimal_rate_percent(AIDifficulty difficulty) {
        if (difficulty == AI_DIFFICULTY_VERY_EASY) return 0; // random
        if (difficulty == AI_DIFFICULTY_EASY) return 25;
        if (difficulty == AI_DIFFICULTY_MEDIUM) return 50;
        if (difficulty == AI_DIFFICULTY_HARD) return 75;
        return 100; // AI_DIFFICULTY_VERY_HARD
    }

    bool _should_use_optimal(AIDifficulty difficulty) {
        int rate = _get_optimal_rate_percent(difficulty); 
        if (rate <= 0) return false;                      
        if (rate >= 100) return true;                     
        return random_int(1, 100) <= rate;               
    }

    bool _find_must_play_move(const GameState& game, Move& outMove) {
        int total = game.totalStones;

        for (int pileIndex = 0; pileIndex< game.piles.size; ++pileIndex) {
            int stones = get(game.piles, pileIndex);
            if (stones <= 0) continue;

            int takeLimit = _get_take_limit(game, pileIndex);

            if (game.settings.gameRule == GAME_RULE_LAST_TAKE_WIN) { // bốc cuối thắng
                // lụm hết để thắng luon
                int needed = total;
                if (needed >= 1 && needed <= stones && needed <= takeLimit) {
                    outMove = {pileIndex, needed};
                    return true;
                }
            } else { // Bốc cuối thua
                // chừa lại 1 viên cuối cho đói thủ
                int needed = total - 1;
                if (needed >= 1 && needed <= stones && needed <= takeLimit) {
                    outMove = {pileIndex, needed};
                    return true;
                }
            }
        }

        return false;
    }


    // * bốc cuối win + không giới hạn
    bool _find_optimal_move_normal_unlimited(const GameState& game, Move& outMove) {
       int nimSum = 0;
        for (int pileIndex = 0; pileIndex < game.piles.size; ++pileIndex) {
            nimSum ^= get(game.piles, pileIndex);
        }

        //  ở thế thua
        if (nimSum == 0) return false;

        for (int pileIndex = 0; pileIndex < game.piles.size; ++pileIndex) {
            int stonesInPile = get(game.piles, pileIndex);
            int target = stonesInPile ^ nimSum;

            // đưa nim sum về trạng thái = 0
            if (target < stonesInPile) {
                int take = stonesInPile - target;
                if (take <= _get_take_limit(game, pileIndex)) {
                    outMove.pileIndex = pileIndex;
                    outMove.stoneCount = take;
                    return true;
                }
            }
        }

        return false;
    }

    // bốc cuối win + giới hạn
    // bool _find_optimal_move_normal_limited(const GameState& game, Move& outMove) {
    //     int m = game.settings.maxTakePerMove;
    //     if (m <= 0) return _find_optimal_move_normal_unlimited(game, outMove);

    //     int nimSum = 0;
    //     for (int i = 0; i < game.piles.size; ++i) {
    //         nimSum ^= (get(game.piles, i) % (m + 1));
    //     }

    //     if (nimSum == 0) return false;

    //     for (int i = 0; i < game.piles.size; ++i) {
    //         int stonesInPile = get(game.piles, i);
    //         if (stonesInPile <= 0) continue;

    //         int g = stonesInPile % (m + 1);
    //         int targetG = g ^ nimSum;

    //         
    //         if (targetG >= g) continue;

    //         int take = g - targetG; // 1..m
    //         if (take <= 0) continue;
    //         if (take > _get_take_limit(game, i)) continue;

    //         outMove.pileIndex = i;
    //         outMove.stoneCount = take;
    //         return true;
    //     }

    //     return false;
    // }

    // * bốc cuối thua + không giới hạn
    bool _find_optimal_move_misere_unlimited(const GameState& game, Move& outMove) {
       int onesCount = 0;
        int bigCount = 0;
        int bigIndex = -1;

        for (int i = 0; i < game.piles.size; ++i) {
            int stonesInPile = get(game.piles, i);
            if (stonesInPile <= 0) continue;

            if (stonesInPile == 1) {
                ++onesCount;
            } else {
                ++bigCount;
                bigIndex = i;
            }
        }

        // tất cả đều là đống 1
        if (bigCount == 0) {
            // nếu chẵn thì có nước thắng, bốc 1 để để lại lẻ cho đối thủ
            if (onesCount % 2 == 0) {
                for (int i = 0; i < game.piles.size; ++i) {
                    if (get(game.piles, i) == 1) {
                        outMove.pileIndex = i;
                        outMove.stoneCount = 1;
                        return true;
                    }
                }
            }
            // nếu lẻ thì ở thế thua
            return false;
        }

        // chỉ còn 1 đống > 1
        if (bigCount == 1) {
            int stonesInPile = get(game.piles, bigIndex);

            if (onesCount % 2 == 0) {
                // để lại đống có số sỏi là 1 có số lượng lẻ cho đối thủ, biến đống lớn thành 1
                outMove.pileIndex = bigIndex;
                outMove.stoneCount = stonesInPile - 1;
                return true;
            } else {
                // nhai lun đống lớn
                outMove.pileIndex = bigIndex;
                outMove.stoneCount = stonesInPile;
                return true;
            }
        }

        // từ 2 đống > 1 trở lên, chơi kiểu nornam
        return _find_optimal_move_normal_unlimited(game, outMove);
    }

    // bool _find_optimal_move_misere_limited(const GameState& game, Move& outMove) {
    //   int m = game.settings.maxTakePerMove;
    //     if (m <= 0) return _find_optimal_move_misere_unlimited(game, outMove);

    //     Move safeFallback = {-1, 0};
    //     Move forcedLoss = {-1, 0};

    //      ưu tiên nước để lại đúng 1 viên cho đối thủ (thắng ngay)
    //      ưu tiên đưa về trạng thái toàn đống 1 với số lượng lẻ cho đối thủ
    //     for (int i = 0; i < game.piles.size; ++i) {
    //         int stonesInPile = get(game.piles, i);
    //         if (stonesInPile <= 0) continue;

    //         int takeLimit = _get_take_limit(game, i);
    //         for (int take = 1; take <= takeLimit; ++take) {
    //             int newTotal = game.totalStones - take;

    //             if (newTotal == 1) {
    //                 outMove.pileIndex = i;
    //                 outMove.stoneCount = take;
    //                 return true;
    //             }

    //             if (newTotal == 0) {
    //                 if (forcedLoss.pileIndex == -1 || take < forcedLoss.stoneCount) {
    //                     forcedLoss.pileIndex = i;
    //                     forcedLoss.stoneCount = take;
    //                 }
    //                 continue;
    //             }

    //             if (safeFallback.pileIndex == -1) {
    //                 safeFallback.pileIndex = i;
    //                 safeFallback.stoneCount = take;
    //             }

    //             int onesCount = 0;
    //             int bigCount = 0;
    //             for (int p = 0; p < game.piles.size; ++p) {
    //                 int after = get(game.piles, p);
    //                 if (p == i) after -= take;

    //                 if (after <= 0) continue;
    //                 if (after == 1) ++onesCount;
    //                 else ++bigCount;
    //             }

    //             if (bigCount == 0 && (onesCount % 2 == 1)) {
    //                 outMove.pileIndex = i;
    //                 outMove.stoneCount = take;
    //                 return true;
    //             }
    //         }
    //     }

    //     thử nước "normal limited optimal" như heuristic
    //     Move normalMove;
    //     if (_find_optimal_move_normal_limited(game, normalMove) &&
    //         is_valid_move(game, normalMove) &&
    //         (game.totalStones - normalMove.stoneCount > 0)) {
    //         outMove = normalMove;
    //         return true;
    //     }

    //     không có thì lấy 1 nước an toàn (không kết thúc game)
    //     if (safeFallback.pileIndex != -1) {
    //         outMove = safeFallback;
    //         return true;
    //     }

    //     bị ép thì chịu thua
    //     if (forcedLoss.pileIndex != -1) {
    //         outMove = forcedLoss;
    //         return true;
    //     }

    //     return false;
    // }

    bool _find_optimal_move(const GameState& game, Move& outMove) {
        bool isUnlimited = (game.settings.maxTakePerMove <= 0); // Có giới hạn hay không

        if (game.settings.gameRule == GAME_RULE_LAST_TAKE_WIN) { // Luật bốc cuối thắng
            if (isUnlimited) return _find_optimal_move_normal_unlimited(game, outMove);
            // return _find_optimal_move_normal_limited(game, outMove);
        }

        // luật bốc cuối thua
        if (isUnlimited) return _find_optimal_move_misere_unlimited(game, outMove);
        // return _find_optimal_move_misere_limited(game, outMove);
    }

    Move _choose_random_move(const GameState& game, int stoneCount = -1) {
        Move move = {0, 0};

        Array validPiles = create_array();

        for (int pileIndex = 0; pileIndex < game.piles.size; ++pileIndex) {
            if (get(game.piles, pileIndex) > 0) {
                if (stoneCount != -1) {
                    return (Move){pileIndex, stoneCount};
                }
                push_back(validPiles, pileIndex);
            }
        }

        if (validPiles.size == 0) {
            free_array(validPiles);
            return move; 
        }
        
        int pick = random_int(0, validPiles.size - 1);
        move.pileIndex = get(validPiles, pick);

        int stonesInPile = get(game.piles, move.pileIndex);
        int maxTakeLimit = stonesInPile;
        int maxTakePerMove = game.settings.maxTakePerMove;
        
        // theo setting, maxTakePerMove == 0 là không giới hạn số lượng lấy
        if (maxTakePerMove > 0 && maxTakeLimit > maxTakePerMove) {
            maxTakeLimit = maxTakePerMove;
        }

        move.stoneCount = random_int(1, maxTakeLimit);

        free_array(validPiles); 
        return move;
    }
}



// * =================================================
// *                   HÀM PUBLIC                   
// * =================================================


Move choose_ai_move(const GameState& game) {
    AIDifficulty difficulty = game.matchConfig.players[game.currentTurn].difficulty;

    if (difficulty != AI_DIFFICULTY_VERY_EASY) {
        Move mustMove;
        if (_find_must_play_move(game, mustMove)) {
            return mustMove;
        }
    }

    Move optimalMove;
    if (_should_use_optimal(difficulty)) {
        if (_find_optimal_move(game, optimalMove) && is_valid_move(game, optimalMove)) {
            return optimalMove;
        }
        else {
            return _choose_random_move(game, 1);
        }
    }

    return _choose_random_move(game);
}
