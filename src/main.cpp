#include "ui.h"
#include "ai.h"
#include "game.h"

#include <windows.h>
#include <iostream>

using namespace std;

void start_game (int gameMode) {

    int pileCount = input_pile_count();

    GameState game;
    init_game(game, pileCount, gameMode);

    // thông tin về lừa đi cuối cùng
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
                }

                show_input_player_move(game);

                InputMoveStatus status = input_player_move(move);

                if (status == INPUT_MOVE_TOO_LONG) {
                    errorMessage = "Quá dài, nhập lại";
                    continue;
                }

                if (status == INPUT_MOVE_INVALID_FORMAT) {
                    errorMessage = "Sai định dạng, nhập lại. Ví dụ: 2 5";
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
        

        // xử lý sau khi lấy nưỡc đi

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

void run_game () {
    // game loop
    while (true) {
        clear_screen();

        // phần menu chính
        int choice = show_main_menu();

        // bắt đầu game
        if (choice == MAIN_MENU_START) {
            while (true) {
                clear_screen();

                int mode = show_game_mode_menu();

                if (mode == GAME_MODE_PVP) {
                    cout << "\nChưa có tính năng này\n";
                    wait_enter();
                    continue;
                }

                if (mode == GAME_MODE_PVAI) {
                    clear_screen();
                    start_game(mode);
                    continue;
                }

                if (mode == GAME_MODE_AIVAI) {
                    cout << "\nChưa có tính năng này\n";
                    wait_enter();
                    continue;
                }

                if (mode == GAME_MODE_EXIT) {
                    break;
                }
            }
            continue;
        }

        // thoát game
        if (choice == MAIN_MENU_EXIT) {
            clear_screen();

            int exit = show_exit_game_menu();
            if (exit == EXIT_GAME_YES) {
                return;
            } else {
                continue;
            }
        }
    }
}

int main() {
    // in ra tiếng việt
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    run_game();
    
    return 0;
}
