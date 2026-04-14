#include "ui.h"

#include <iostream>
#include <stdio.h>
#include <conio.h>

using std::cout;
using std::cin;



namespace {
    const int INF = (int)1e9;


    // 

    const char* mainMenuList[MAIN_MENU_COUNT] = {
        "1. Bắt đầu trò chơi",
        "0. Thoát"
    };

    const char* gameModeList[GAME_MODE_MENU_COUNT] = {
        "1. Người vs Người",
        "2. Người vs AI",
        "3. AI vs AI",
        "0. Quay lại"
    };

    const char* exitGameList[EXIT_CONFIRM_COUNT] = {
        "1. Có",
        "0. Không"
    };


    const char* _get_actor_text(const GameState& game, int actorIndex) {
        const PlayerConfig& p0 = game.matchConfig.players[0];
        const PlayerConfig& p1 = game.matchConfig.players[1];

        if (p0.type == PLAYER_TYPE_HUMAN && p1.type == PLAYER_TYPE_AI) {
            return actorIndex == 0 ? "Người chơi" : "AI";
        }

        if (p0.type == PLAYER_TYPE_AI && p1.type == PLAYER_TYPE_AI) {
            return actorIndex == 0 ? "AI 1" : "AI 2";
        }

        return actorIndex == 0 ? "Người chơi 1" : "Người chơi 2";
    }





    // * hàm in ra menu
    
    void _main_menu () {
        cout << ANSI_BOLD_CYAN << 
                "╔══════════════════════════════╗\n";
        cout << "║           NIM GAME           ║\n";
        cout << "╚══════════════════════════════╝\n";


        // in ra danh sách lựa chọn
        for (int i=0; i<MAIN_MENU_COUNT-1; ++i) {
            cout << ANSI_BOLD_GREEN << mainMenuList[i] << '\n';
        }
        cout << ANSI_BOLD_RED   << mainMenuList[MAIN_MENU_COUNT-1] << ANSI_RESET << '\n';

    }

    void _game_mode_menu () {
        cout << ANSI_BOLD_CYAN << 
                "╔══════════════════════════════╗\n";
        cout << "║          CHỌN CHẾ ĐỘ         ║\n";
        cout << "╚══════════════════════════════╝\n";

        // in ra danh sách lựa chọn
        for (int i=0; i<GAME_MODE_MENU_COUNT-1; ++i) {
            cout << ANSI_BOLD_GREEN << gameModeList[i] << '\n';
        }
        cout << ANSI_BOLD_RED   << gameModeList[GAME_MODE_MENU_COUNT-1] << ANSI_RESET << '\n';

    }

    void _exit_menu () {
       cout << ANSI_BOLD_CYAN << 
                "╔══════════════════════════════╗\n";
        cout << "║   BẠN CÓ MUỐN THOÁT KHÔNG?   ║\n";
        cout << "╚══════════════════════════════╝\n";

        // in ra danh sách lựa chọn
        cout << ANSI_BOLD_GREEN << exitGameList[0] << '\n';
        cout << ANSI_BOLD_RED   << exitGameList[1] << ANSI_RESET << '\n';

    }

    // hàm in ra menu theo con trỏ hàm truyền vào, kiểm tra input 
    int _show_menu (void (*menu)(), int menuCount) {
        int choice;
        const char* errorMessage = nullptr;

        while (true) {
            clear_screen();
            menu();
            
            cout << '\n';
            show_error_message(errorMessage);

            cout << ANSI_BOLD_YELLOW << "Chọn: " << ANSI_RESET;
            
            char line[10];
            cin.getline(line, 10);

            // nếu nhập không hợp lệ
            if (cin.fail()) {
                cin.clear(); // fix cin khi nhập không hợp lệ
                clear_line();
                errorMessage = "Đầu vào không hợp lệ, nhập lại";
                continue;
            }
            
            char tmp;
            if (sscanf(line, "%d %c", &choice, &tmp) != 1) {
                errorMessage = "Đầu vào không hợp lệ, nhập lại";
                continue;
            }

            if (choice < 0 || choice >= menuCount) {
                errorMessage = "Lựa chọn không hợp lệ";
                continue;
            }

            break;
        }

        return choice;
    }




    // * hàm in ra UI lúc chơi
    
    void _show_game_state (const GameState& game) {
        for (int pileIndex = 0; pileIndex < game.piles.size; ++pileIndex) {
            int stoneCount = get(game.piles, pileIndex);
            
            cout << ANSI_BOLD_GREEN << "Đống " << pileIndex + 1 << " (" << stoneCount <<"): ";

            for (int i = 0; i < stoneCount; ++i) {
                cout << ANSI_WHITE << "O" << ANSI_RESET;
            }

            cout << '\n';
        }
    }

}

// ------------------------------

// * 
void clear_line () {
//    char c;
//    while (cin.get(c) && c != '\n') {}
    cin.ignore(INF, '\n');
}

// void wait_enter() {
//     cout << "\nNhấn phím Enter để tiếp tục...";
//     char line[10];
//     cin.getline(line, 10);

//     if (cin.fail()) {
//         cin.clear(); 
//         clear_line();
//     }
// }

void wait_enter() {
    cout << ANSI_BOLD_CYAN << "\nNhấn phím Enter để tiếp tục..." << ANSI_RESET;

    while (true) {
        int key = _getch();

        // Enter
        if (key == '\r') {
            break;
        }

        // phím đặc biệt như mũi tên, F1...
        if (key == 0 || key == 224) {
            _getch();
        }
    }
}


void wait_press () {
    cout << ANSI_BOLD_CYAN << "\nNhấn phím bất kỳ để tiếp tục...";

    int key = _getch();

    // phím đặc biệt như mũi tên, F1...
    if (key == 0 || key == 224) {
        key = _getch();
    }
}

// xóa màn hình
void clear_screen () {
    system("cls");
}

 void show_error_message (const char* message) {
    if (message == nullptr) return;
    cout << ANSI_BOLD_RED << "[ERROR]: " << ANSI_RED << message << ANSI_RESET << '\n';
}

// void show_hint_message (const char* message) {
//     if (message == nullptr) return;
//     cout << ANSI_YELLOW << message << ANSI_RESET << '\n';
// }

// * MENU ---------

// in ra menu chính
MainMenu show_main_menu () {
    return (MainMenu)_show_menu(_main_menu, MAIN_MENU_COUNT);
}

// in ra menu chế độ chơi
GameModeMenu show_game_mode_menu () {
    return (GameModeMenu)_show_menu(_game_mode_menu, GAME_MODE_MENU_COUNT);
}

ExitConfirmChoice show_exit_menu () {
    return (ExitConfirmChoice)_show_menu(_exit_menu, EXIT_CONFIRM_COUNT);
}




// * INPUT -----------------------------

void show_input_pile_count (const GameSettings& settings, const char* errorMessage) {
    show_error_message(errorMessage);

    cout << ANSI_BOLD_YELLOW
         << "Nhập số lượng đống sỏi (từ " << settings.minPileCount
         << " tới " << settings.maxPileCount << "): " 
         << ANSI_RESET;
}

InputStatus input_pile_count (int& pileCount) {
    char line[10];
    cin.getline(line, 10);

    if (cin.fail()) {
        cin.clear();
        clear_line();

        return INPUT_TOO_LONG;
    }

    char temp;
    if (sscanf(line, "%d %c", &pileCount, &temp) == 1) {
        if (pileCount == 0) {
            return INPUT_EXIT;
        }
        return INPUT_OK;
    }

  
    return INPUT_INVALID_FORMAT;
}

void show_input_player_move (const GameState& game, const char* errorMessage) {
    cout << ANSI_BOLD_BLUE << "Đến lượt "
         << ANSI_BOLD_MAGENTA << _get_actor_text(game, game.currentTurn) << "\n\n" 
         << ANSI_RESET;

    show_error_message(errorMessage);

    cout << ANSI_BOLD_YELLOW << "Nhập đống sỏi và số lượng muốn lấy (ví dụ: 2 5): " << ANSI_RESET;
}

InputStatus input_player_move (Move& move) {
    char line[10];
    cin.getline(line, 10);

    if (cin.fail()) {
        cin.clear();
        clear_line();

        return INPUT_TOO_LONG;
    }

    int exitValue;
    char temp;

    if (sscanf(line, "%d %c", &exitValue, &temp) == 1) {
        if (exitValue == 0) {
            return INPUT_EXIT;
        }
    }

    if (sscanf(line, "%d %d %c", &move.pileIndex, &move.stoneCount, &temp) == 2) {
        --move.pileIndex;
        
        return INPUT_OK;
    }

    return INPUT_INVALID_FORMAT;
}




// * SHOW ---------------------------------

void show_last_game_state (const GameState& lastGame) {
    cout << ANSI_BOLD_BLUE << "Trạng thái game trước đó:\n" << ANSI_RESET;

    _show_game_state(lastGame);

    cout << '\n';
}

void show_current_game_state (const GameState& game) {
    cout << ANSI_BOLD_BLUE << "Trạng thái game hiện tại:\n" << ANSI_RESET;

    _show_game_state(game);

    cout << '\n';
}

void show_last_move (const GameState& lastGame, const Move& lastMove) {
    cout << ANSI_BOLD_MAGENTA << _get_actor_text(lastGame, lastGame.currentTurn)
         << ANSI_BOLD_BLUE << " đã chọn đống "
         << ANSI_BOLD_YELLOW << lastMove.pileIndex + 1
         << ANSI_BOLD_BLUE << " và bốc "
         << ANSI_BOLD_YELLOW << lastMove.stoneCount 
         << ANSI_BOLD_BLUE << " viên sỏi\n\n"
         << ANSI_RESET;
}
 
void show_winner (const GameState& game) {
    cout << ANSI_BOLD_BLUE << "Kết thúc trò chơi\n";

    int currentTurn = game.currentTurn;

    if (game.settings.gameRule == GAME_RULE_LAST_TAKE_LOSE) {
        currentTurn = 1 - currentTurn;
    }

    cout << ANSI_BOLD_MAGENTA << _get_actor_text(game, currentTurn) 
    << ANSI_BOLD_BLUE << " thắng\n\n" << ANSI_RESET;
}

// int main () {

//     SetConsoleOutputCP(CP_UTF8);
//     SetConsoleCP(CP_UTF8);

//     show_main_menu();

//     wait_enter();
// }


