#include "ui.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>

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
        std::cout << "====================\n";
        std::cout << "        STONE GAME\n";
        std::cout << "====================\n";

        // in ra danh sách lựa chọn
        for (int i=0; i<MAIN_MENU_COUNT; ++i) {
            std::cout << mainMenuList[i] << '\n';
        }

        std::cout << "\nChọn: ";
    }

    void _game_mode_menu () {
        std::cout << "====================\n";
        std::cout << "        CHỌN CHẾ ĐỘ\n";
        std::cout << "====================\n";

        // in ra danh sách lựa chọn
        for (int i=0; i<GAME_MODE_COUNT; ++i) {
            std::cout << gameModeList[i] << '\n';
        }

        std::cout << "\nChọn: ";
    }

    void _exit_game_menu () {
        std::cout << "==========================\n";
        std::cout << "        BẠN CÓ MUỐN THOÁT KHÔNG?\n";
        std::cout << "==========================\n";

        // in ra danh sách lựa chọn
        for (int i=0; i<EXIT_GAME_COUNT; ++i) {
            std::cout << exitGameList[i] << '\n';
        }

        std::cout << "\nChọn: ";
    }

    // hàm in ra menu theo con trỏ hàm truyền vào, kiểm tra input 
    int _show_menu (void (*menu)(), int menuCount) {
        int choice;

        while (true) {
            menu();

            char line[100];
            std::cin.getline(line, 100);

            
            // nếu nhập không hợp lệ
            if (std::cin.fail()) {
                std::cin.clear(); // fix cin khi nhập không hợp lệ
                clear_line();

                std::cout << "\nKhông hợp lệ, nhập lại\n";

                wait_enter();
                clear_screen();
                continue;
            }
            
            char tmp;
            if (std::sscanf(line, "%d %c", &choice, &tmp) == 1) {
                if (0 <= choice && choice < menuCount) {
                    break;
                }
            }

            // nếu nhập không hợp lệ
            std::cout << "\nLựa chọn không hợp lệ\n";

            wait_enter();
            clear_screen();
        }

        return choice;
    }




    // * hàm in ra UI lúc chơi
    
    void _show_ui_input_pile_count () {
        std::cout << "Nhập số lượng đống sỏi: ";
    }

    void _show_game_state (const GameState& game) {
        for (int pileIndex = 0; pileIndex < game.piles.size; ++pileIndex) {
            int stoneCount = get(game.piles, pileIndex);
            
            std::cout << "Đống " << pileIndex + 1 << " (" << stoneCount <<"): ";

            for (int i = 0; i < stoneCount; ++i) {
                std::cout << "O";
            }

            std::cout << '\n';
        }
    }



}



void clear_line () {
//    char c;
//    while (std::cin.get(c) && c != '\n') {}
    std::cin.ignore(INF, '\n');
}

void wait_enter() {
    std::cout << "\nNhấn Enter để tiếp tục...";
    char line[100];
    std::cin.getline(line, 100);

    if (std::cin.fail()) {
        std::cin.clear(); 
        clear_line();
    }
}

// xóa màn hình
void clear_screen () {
    std::system("cls");
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

int input_pile_count () {  
    int pileCount;

    while (true) {
        _show_ui_input_pile_count();

        char line[100];
        std::cin.getline(line, 100);

        // nếu nhập không hợp lệ
        if (std::cin.fail()) {
            std::cin.clear(); // fix cin khi nhập không hợp lệ
            clear_line();

            std::cout << "\nKhông hợp lệ, nhập lại\n";

            wait_enter();
            clear_screen();
            continue;
        }
        
        char tmp;
        if (std::sscanf(line, "%d %c", &pileCount, &tmp) == 1) {
            if (0 < pileCount && pileCount < 36) {
                break;
            }    
        }

        // nếu nhập ký tự không hợp lệ
        std::cout << "Số lượng hoặc dữ liệu đầu vào không hợp lệ\n";
        std::cout << "Sổ đống sỏi phải là 1 số nguyên dương và bé hơn 36\n";

        wait_enter();
        clear_screen();
    } 

    return pileCount;
}

void show_input_player_move (const GameState& game) {
    std::cout << "Đến lượt " << actorTextList[game.gameMode][game.currentTurn] << '\n';
    std::cout << "Nhập đống sỏi và số lượng muốn lấy: ";
}

InputMoveStatus input_player_move (Move& move) {
    char line[100];
    std::cin.getline(line, 100);

    if (std::cin.fail()) {
        std::cin.clear();
        clear_line();

        return INPUT_MOVE_TOO_LONG;
    }

    char tmp;
    if (std::sscanf(line, "%d %d %c", &move.pileIndex, &move.stoneCount, &tmp) == 2) {
        --move.pileIndex;
        
        return INPUT_MOVE_OK;
    }

    return INPUT_MOVE_INVALID_FORMAT;
}




// * SHOW ---------------------------------

void show_last_game_state (const GameState& oldGame) {
    std::cout << "Trạng thái game trước đó là\n";

    _show_game_state(oldGame);

    std::cout << '\n';
}

void show_current_game_state (const GameState& game) {
    std::cout << "Trạng thái game hiện tại\n";

    _show_game_state(game);

    std::cout << '\n';
}

void show_last_move (const GameState& game, int lastActor, const Move& lastMove) {
    std::cout << actorTextList[game.gameMode][lastActor]
        << " đã chọn đống " << lastMove.pileIndex + 1
        << " và bốc " << lastMove.stoneCount << " viên sỏi\n\n";
}


void show_ai_move (const GameState& game, const Move& move) {
    // PVAI
     if (game.gameMode == GAME_MODE_PVAI) {
        std::cout << "Đến lượt AI\n";
        std::cout << "AI chọn đống sỏi thứ " << move.pileIndex + 1 << '\n';
        std::cout << "AI bốc " << move.stoneCount << " viên sỏi\n";
    } else { // AIVAI
        int currentAI = game.currentTurn + 1;
        int pilePosition = move.pileIndex + 1;
        int stoneCount = move.stoneCount;

        std::cout << "Đến lượt AI " << currentAI << '\n';
        std::cout << "AI " << currentAI << "chọn đống sỏi thứ " << pilePosition << '\n';
        std::cout << "AI bốc " << stoneCount << " viên sỏi\n";
    }
}
 
void show_winner (const GameState& game) {
    std::cout << "Kết thúc trò chơi\n";

    int gameMode = game.gameMode;
    int currentTurn = game.currentTurn;

    if (game.gameRule == GAME_RULE_LAST_TAKE_LOSE) {
        currentTurn = 1 - currentTurn;
    }

    std::cout << actorTextList[gameMode][currentTurn] << " thắng\n\n";
}
// int main () {

//     SetConsoleOutputCP(CP_UTF8);
//     SetConsoleCP(CP_UTF8);

//     show_main_menu();

//     wait_enter();
// }


