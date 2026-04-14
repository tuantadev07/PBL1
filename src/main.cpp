#include "ui.h"
#include "ai.h"
#include "game.h"

#include <windows.h>
#include <iostream>

using namespace std;



MatchConfig create_match_config (GameMode gameMode) {
    MatchConfig matchConfig;
    matchConfig.gameMode = gameMode;

    if (gameMode == GAME_MODE_PVP) {
        matchConfig.players[0].type = PLAYER_TYPE_HUMAN;
        matchConfig.players[1].type = PLAYER_TYPE_HUMAN;
    } 
    else if (gameMode == GAME_MODE_PVAI) {
        matchConfig.players[0].type = PLAYER_TYPE_HUMAN;
        matchConfig.players[1].type = PLAYER_TYPE_AI;
        matchConfig.players[1].difficulty = AI_DIFFICULTY_VERY_EASY;
    }
    else {
        matchConfig.players[0].type = PLAYER_TYPE_AI;
        matchConfig.players[1].type = PLAYER_TYPE_AI;
        matchConfig.players[0].difficulty = AI_DIFFICULTY_VERY_EASY;
        matchConfig.players[1].difficulty = AI_DIFFICULTY_VERY_EASY;
    }

    return matchConfig;
}


ExitConfirmChoice confirm_exit () {
    clear_screen();
    return show_exit_menu();
}

const char* get_input_error_message(InputStatus status) {
    if (status == INPUT_TOO_LONG) {
        return "Quá dài, vui lòng nhập lại";
    }

    if (status == INPUT_INVALID_FORMAT) {
        return "Sai định dạng, vui lòng nhập lại";
    }

    return nullptr;
}


bool get_pile_count (const GameSettings& settings, int& pileCount) {
    const char* errorMessage = nullptr;

    // nhập số đống sỏi
    while (true) {
        clear_screen();
        show_input_pile_count(settings, errorMessage);

        InputStatus status = input_pile_count(pileCount);

        if (status == INPUT_EXIT) {
            if (confirm_exit() == EXIT_CONFIRM_YES) {
                return false;
            }
            continue;
        }

        const char* inputErrorMessage = get_input_error_message(status);
        if (inputErrorMessage != nullptr) {
            errorMessage = inputErrorMessage;
            continue;
        }

        if (pileCount < settings.minPileCount || pileCount > settings.maxPileCount) {
            errorMessage = "Số đống sỏi không nằm trong giới hạn, nhập lại";
            continue;
        }
        // nếu số đống sỏi hợp lệ
        return true;
    }
}

bool get_player_move (const GameState& game, Move& move) {
    const char* errorMessage = nullptr;

    // Lấy nước đi của người chơi và máy
    while (true) {
        clear_screen();
        show_current_game_state(game);
        show_input_player_move(game, errorMessage);

        InputStatus status = input_player_move(move);

        if (status == INPUT_EXIT) {
            if (confirm_exit() == EXIT_CONFIRM_YES) {
                return false;
            }
            continue;
        }

        const char* inputErrorMessage = get_input_error_message(status);
        if (inputErrorMessage != nullptr) {
            errorMessage = inputErrorMessage;
            continue;
        }

        if (!is_valid_move(game, move)) {
            errorMessage = "Nước đi không hợp lệ, nhập lại";
            continue;
        }
        // nếu nước đi hợp lệ
        return true;
    }
}


void start_game (const MatchConfig& matchConfig, const GameSettings& settings) {

    int pileCount;
    if (!get_pile_count(settings, pileCount)) {
        return;
    }

    // khởi tạo
    GameState game;
    init_game(game, pileCount, matchConfig, settings);

    // thông tin về lượt đi cuối cùng
    bool hasLastMove = false;
    GameState lastGame;
    Move lastMove;
    

    // game loop
    while (true) {

        // in ra trạng thái trước và nước đi
        if (hasLastMove) {
            clear_screen();

            show_last_game_state(lastGame);
            show_last_move(lastGame, lastMove);
            show_current_game_state(game);

            // nếu game đã kết thúc
            if (is_game_over(game)) {
                show_winner(game);
                wait_enter();
                break;
            }

            wait_press();
        }

        // phần lấy nước đi

        Move move;

        if (game.matchConfig.players[game.currentTurn].type == PLAYER_TYPE_HUMAN) {
            if (!get_player_move(game, move)) {
                break;
            }
        } else {
            move = choose_ai_move(game);
        }
        
        
        // xử lý sau khi lấy nước đi
        if (hasLastMove) {
            free_game(lastGame);
        }
        lastGame = copy_game_state(game);
        lastMove = move;
        hasLastMove = true;

        apply_move(game, move);
        
        // nếu game chưa kết thúc thì tiếp tục
        if (!is_game_over(game)) {
            next_turn(game);
        }        
    }

    //giải phóng dung lượng
    free_game(game);
    if (hasLastMove) {
        free_game(lastGame);
    }
}




// * 

void run_mode_menu (const GameSettings& settings) {
    while (true) {
        clear_screen();

        GameModeMenu modeChoice = show_game_mode_menu();

        if (modeChoice == GAME_MODE_MENU_PVP) {
            start_game(create_match_config(GAME_MODE_PVP), settings);
            continue;
        }

        if (modeChoice == GAME_MODE_MENU_PVAI) {
            start_game(create_match_config(GAME_MODE_PVAI), settings);
            continue;
        }

        if (modeChoice == GAME_MODE_MENU_AIVAI) {
            // start_game(create_match_config(GAME_MODE_AIVAI), settings);
            cout << "\nNạp lần đầu để mở khóa chế độ này\n";
            wait_press();
            continue;
        }

        if (modeChoice == GAME_MODE_MENU_EXIT) {
            return;
        }
    }
}

void run_game () {
    GameSettings settings = load_game_settings();

    // game loop
    while (true) {
        clear_screen();

        // phần menu chính
        MainMenu choice = show_main_menu();

        // bắt đầu game
        if (choice == MAIN_MENU_START) {
            run_mode_menu(settings);
            continue;
        }

        // thoát game
        if (choice == MAIN_MENU_EXIT) {
            if (confirm_exit() == EXIT_CONFIRM_YES) 
                return;
            else 
                continue;
        }
    }
}

int main() {
    // in ra tiếng Việt
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    run_game();
    
    return 0;
}



