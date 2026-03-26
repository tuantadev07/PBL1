#include "ui.h"
#include "ai.h"
#include "game.h"

#include <windows.h>
#include <iostream>

using namespace std;



const char* get_input_error_message(InputStatus status) {
    if (status == INPUT_TOO_LONG) {
        return "Quá dài, vui lòng nhập lại";
    }

    if (status == INPUT_INVALID_FORMAT) {
        return "Sai định dạng, vui lòng nhập lại";
    }

    return nullptr;
}



void start_game (GameMode gameMode, const GameSettings& settings) {

    int pileCount;
    const char* errorMessage = nullptr;

    // nhập số đống sỏi
    while (true) {
        clear_screen();

        if (errorMessage != nullptr) {
            cout << errorMessage << '\n';
            wait_enter();
            clear_screen();
        }

        show_input_pile_count(settings);

        InputStatus status = input_pile_count(pileCount);

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
        break;
    }
    

    // khởi tạo
    GameState game;
    init_game(game, pileCount, gameMode, settings);

    // thông tin về lượt đi cuối cùng
    bool hasLastMove = false;
    GameState oldGame;
    Move lastMove;
    int lastActor = -1;

    // game loop
    while (true) {

        // in ra trạng thái trước và nước đi
        if (hasLastMove) {
            clear_screen();

            show_last_game_state(oldGame);

            show_last_move(oldGame, lastActor, lastMove);

            show_current_game_state(game);

            if (is_game_over(game)) {
                show_winner(game);

                free_game(oldGame);

                wait_enter();
                break;
            }

            wait_enter();
        }


        // phần lấy nước đi

        Move move;
        int actor = game.currentTurn;

        if (game.gameMode == GAME_MODE_PVP || game.currentTurn == 0) {
            const char* errorMessage = nullptr;

            // Lấy nước đi của người chơi và máy
            while (true) {
                clear_screen();
                show_current_game_state(game);

                if (errorMessage != nullptr) {
                    cout << errorMessage << "\n\n";
                    
                    wait_enter();
                    clear_screen();
                    show_current_game_state(game);
                }

                show_input_player_move(game);

                InputStatus status = input_player_move(move);

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
                break;
            }
        } else {
            move = choose_random_move(game);
        }
        

        // xử lý sau khi lấy nước đi

        if (hasLastMove) {
            free_game(oldGame);
        }

        oldGame = copy_game_state(game);

        apply_move(game, move);

        lastActor = actor;
        lastMove = move;
        hasLastMove = true;


        // nếu game chưa kết thúc thì tiếp tục
        if (!is_game_over(game)) {
            next_turn(game);
        }        
    }

    //giải phóng dung lượng
    free_game(game);

}




// * 

void run_game () {
    GameSettings settings = load_game_settings();

    // game loop
    while (true) {
        clear_screen();

        // phần menu chính
        MainMenu choice = show_main_menu();

        // bắt đầu game
        if (choice == MAIN_MENU_START) {
            while (true) {
                clear_screen();

                GameModeMenu modeChoice = show_game_mode_menu();

                if (modeChoice == GAME_MODE_MENU_PVP) {
                    clear_screen();
                    start_game(GAME_MODE_PVP, settings);
                    continue;
                }

                if (modeChoice == GAME_MODE_MENU_PVAI) {
                    clear_screen();
                    start_game(GAME_MODE_PVAI, settings);
                    continue;
                }

                if (modeChoice == GAME_MODE_MENU_AIVAI) {
                    cout << "\nNạp lần đầu để mở khóa chức năng này\n";
                    wait_enter();
                    continue;
                }

                if (modeChoice == GAME_MODE_MENU_EXIT) {
                    break;
                }
            }
            continue;
        }

        // thoát game
        if (choice == MAIN_MENU_EXIT) {
            clear_screen();

            ExitGameMenu exitChoice = show_exit_game_menu();
            if (exitChoice == EXIT_GAME_YES) {
                return;
            } else {
                continue;
            }
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



