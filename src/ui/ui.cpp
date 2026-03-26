#include "ui.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using std::cout;
using std::cin;

namespace {
    const int INF = (int)1e9;


    // 

    const char* mainMenuList[MAIN_MENU_COUNT] = {
        "1. Bắt đầu trò chơi",
        "0. Thoát"
    };

    const char* gameModeList[GAME_MODE_COUNT] = {
        "1. Người vs Người",
        "2. Người vs AI",
        "3. AI vs AI",
        "0. Quay lại"
    };

    const char* exitGameList[EXIT_GAME_COUNT] = {
        "1. Có",
        "0. Không"
    };


   
    const char* actorTextList[GAME_MODE_COUNT][2] = {
        {"", ""},

        // PVP
        {
            "Người chơi 1",
            "Người chơi 2"
        },

        // PVAI
        {
            "Người chơi",
            "AI"
        },

        // AIVAI
        {
            "AI 1",
            "AI 2"
        }
    };




    // * hàm in ra menu
    
    void _main_menu () {
        cout << "====================\n";
        cout << "        STONE GAME\n";
        cout << "====================\n";

        // in ra danh sách lựa chọn
        for (int i=0; i<MAIN_MENU_COUNT; ++i) {
            cout << mainMenuList[i] << '\n';
        }

        cout << "\nChọn: ";
    }

    void _game_mode_menu () {
        cout << "====================\n";
        cout << "        CHỌN CHẾ ĐỘ\n";
        cout << "====================\n";

        // in ra danh sách lựa chọn
        for (int i=0; i<GAME_MODE_COUNT; ++i) {
            cout << gameModeList[i] << '\n';
        }

        cout << "\nChọn: ";
    }

    void _exit_game_menu () {
        cout << "==========================\n";
        cout << "        BẠN CÓ MUỐN THOÁT KHÔNG?\n";
        cout << "==========================\n";

        // in ra danh sách lựa chọn
        for (int i=0; i<EXIT_GAME_COUNT; ++i) {
            cout << exitGameList[i] << '\n';
        }

        cout << "\nChọn: ";
    }

    // hàm in ra menu theo con trỏ hàm truyền vào, kiểm tra input 
    int _show_menu (void (*menu)(), int menuCount) {
        int choice;

        while (true) {
            menu();

            char line[100];
            cin.getline(line, 100);

            
            // nếu nhập không hợp lệ
            if (cin.fail()) {
                cin.clear(); // fix cin khi nhập không hợp lệ
                clear_line();

                cout << "\nKhông hợp lệ, nhập lại\n";

                wait_enter();
                clear_screen();
                continue;
            }
            
            char tmp;
            if (sscanf(line, "%d %c", &choice, &tmp) == 1) {
                if (0 <= choice && choice < menuCount) {
                    break;
                }
            }

            // nếu nhập không hợp lệ
            cout << "\nLựa chọn không hợp lệ\n";

            wait_enter();
            clear_screen();
        }

        return choice;
    }




    // * hàm in ra UI lúc chơi
    
    void _show_game_state (const GameState& game) {
        for (int pileIndex = 0; pileIndex < game.piles.size; ++pileIndex) {
            int stoneCount = get(game.piles, pileIndex);
            
            cout << "Đống " << pileIndex + 1 << " (" << stoneCount <<"): ";

            for (int i = 0; i < stoneCount; ++i) {
                cout << "O";
            }

            cout << '\n';
        }
    }

}

// ------------------------------


void clear_line () {
//    char c;
//    while (cin.get(c) && c != '\n') {}
    cin.ignore(INF, '\n');
}

void wait_enter() {
    cout << "\nNhấn Enter để tiếp tục...";
    char line[100];
    cin.getline(line, 100);

    if (cin.fail()) {
        cin.clear(); 
        clear_line();
    }
}

// xóa màn hình
void clear_screen () {
    system("cls");
}




// in ra menu chính
int show_main_menu () {
    return _show_menu(_main_menu, MAIN_MENU_COUNT);
}

// in ra menu chế độ chơi
int show_game_mode_menu () {
    return _show_menu(_game_mode_menu, GAME_MODE_COUNT);
}

int show_exit_game_menu () {
    return _show_menu(_exit_game_menu, EXIT_GAME_COUNT);
}




// * INPUT -----------------------------

void show_input_pile_count (const GameSettings& settings) {
    cout << "Nhập số lượng đống sỏi (từ " << settings.minPileCount
        << " tới " << settings.maxPileCount << "): ";
}

InputStatus input_pile_count (int& pileCount) {
    char line[100];
    cin.getline(line, 100);

    if (cin.fail()) {
        cin.clear();
        clear_line();

        return INPUT_TOO_LONG;
    }

    char tmp;
    if (sscanf(line, "%d %c", &pileCount, &tmp) == 1) {
        return INPUT_OK;
    }

    return INPUT_INVALID_FORMAT;
}

void show_input_player_move (const GameState& game) {
    cout << "Đến lượt " << actorTextList[game.gameMode][game.currentTurn] << '\n';
    cout << "Nhập đống sỏi và số lượng muốn lấy (ví dụ: 2 5): ";
}

InputStatus input_player_move (Move& move) {
    char line[100];
    cin.getline(line, 100);

    if (cin.fail()) {
        cin.clear();
        clear_line();

        return INPUT_TOO_LONG;
    }

    char tmp;
    if (sscanf(line, "%d %d %c", &move.pileIndex, &move.stoneCount, &tmp) == 2) {
        --move.pileIndex;
        
        return INPUT_OK;
    }

    return INPUT_INVALID_FORMAT;
}




// * SHOW ---------------------------------

void show_last_game_state (const GameState& oldGame) {
    cout << "Trạng thái game trước đó là\n";

    _show_game_state(oldGame);

    cout << '\n';
}

void show_current_game_state (const GameState& game) {
    cout << "Trạng thái game hiện tại\n";

    _show_game_state(game);

    cout << '\n';
}

void show_last_move (const GameState& game, int lastActor, const Move& lastMove) {
    cout << actorTextList[game.gameMode][lastActor]
        << " đã chọn đống " << lastMove.pileIndex + 1
        << " và bốc " << lastMove.stoneCount << " viên sỏi\n\n";
}
 
void show_winner (const GameState& game) {
    cout << "Kết thúc trò chơi\n";

    int gameMode = game.gameMode;
    int currentTurn = game.currentTurn;

    if (game.settings.gameRule == GAME_RULE_LAST_TAKE_LOSE) {
        currentTurn = 1 - currentTurn;
    }

    cout << actorTextList[gameMode][currentTurn] << " thắng\n\n";
}
// int main () {

//     SetConsoleOutputCP(CP_UTF8);
//     SetConsoleCP(CP_UTF8);

//     show_main_menu();

//     wait_enter();
// }


