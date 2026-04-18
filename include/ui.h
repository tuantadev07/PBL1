#pragma once

#include "game.h"

#define ANSI_RESET          "\033[0m"

#define ANSI_BOLD           "\033[1m"
#define ANSI_UNDERLINE      "\033[4m"

// Chữ thường (Regular Foreground)
#define ANSI_BLACK          "\033[0;30m"
#define ANSI_RED            "\033[0;31m"
#define ANSI_GREEN          "\033[0;32m"
#define ANSI_YELLOW         "\033[0;33m"
#define ANSI_BLUE           "\033[0;34m"
#define ANSI_MAGENTA        "\033[0;35m"
#define ANSI_CYAN           "\033[0;36m"
#define ANSI_WHITE          "\033[0;37m"

// Chữ in đậm (Bold Foreground)
#define ANSI_BOLD_BLACK     "\033[1;30m"
#define ANSI_BOLD_RED       "\033[1;31m"
#define ANSI_BOLD_GREEN     "\033[1;32m"
#define ANSI_BOLD_YELLOW    "\033[1;33m"
#define ANSI_BOLD_BLUE      "\033[1;34m"
#define ANSI_BOLD_MAGENTA   "\033[1;35m"
#define ANSI_BOLD_CYAN      "\033[1;36m"
#define ANSI_BOLD_WHITE     "\033[1;37m"

// Chữ gạch chân (Underline Foreground)
#define ANSI_UL_BLACK       "\033[4;30m"
#define ANSI_UL_RED         "\033[4;31m"
#define ANSI_UL_GREEN       "\033[4;32m"
#define ANSI_UL_YELLOW      "\033[4;33m"
#define ANSI_UL_BLUE        "\033[4;34m"
#define ANSI_UL_MAGENTA     "\033[4;35m"
#define ANSI_UL_CYAN        "\033[4;36m"
#define ANSI_UL_WHITE       "\033[4;37m"

// Màu nền (Background)
#define ANSI_BG_BLACK       "\033[40m"
#define ANSI_BG_RED         "\033[41m"
#define ANSI_BG_GREEN       "\033[42m"
#define ANSI_BG_YELLOW      "\033[43m"
#define ANSI_BG_BLUE        "\033[44m"
#define ANSI_BG_MAGENTA     "\033[45m"
#define ANSI_BG_CYAN        "\033[46m"
#define ANSI_BG_WHITE       "\033[47m"



enum MainMenu {
    MAIN_MENU_EXIT = 0,
    MAIN_MENU_START = 1,
    MAIN_MENU_COUNT
};

enum ExitConfirmChoice {
    EXIT_CONFIRM_NO = 0,
    EXIT_CONFIRM_YES = 1,
    EXIT_CONFIRM_COUNT
};

enum GameModeMenu {
    GAME_MODE_MENU_EXIT = 0,
    GAME_MODE_MENU_PVP = 1,
    GAME_MODE_MENU_PVAI = 2,
    GAME_MODE_MENU_AIVAI = 3,
    GAME_MODE_MENU_COUNT
};

enum GameDifficultyMenu {
    GAME_DIFFICULTY_MENU_EXIT = 0,
    GAME_DIFFICULTY_MENU_VERY_EASY = 1,
    GAME_DIFFICULTY_MENU_EASY = 2,
    GAME_DIFFICULTY_MENU_MEDIUM = 3,
    GAME_DIFFICULTY_MENU_HARD = 4,
    GAME_DIFFICULTY_MENU_VERY_HARD = 5,
    GAME_DIFFICULTY_MENU_COUNT
};

enum InputStatus {
    INPUT_OK = 0,
    INPUT_EXIT,
    INPUT_INVALID_FORMAT,
    INPUT_TOO_LONG
};




// dọn ký tụ rác và thừa còn lại
void clear_line ();

// chờ người dùng ấn entẻ
void wait_enter ();

void wait_press ();

// xóa màn hình
void clear_screen ();

const char* get_actor_text (const GameState& game, int actorIndex);





// in ra menu chính và trả về lựa chọn
MainMenu show_main_menu ();

// in ra menu chế độ chơi và trả về lựa chọn
GameModeMenu show_game_mode_menu ();

GameDifficultyMenu show_game_difficulty_menu ();

// in ra menu lúc xác nhận thoát game
ExitConfirmChoice show_exit_menu ();



// in ra giao diện nhập số đống sỏi
void show_input_pile_count (const GameSettings& settings, const char* errorMessage = nullptr);
// lấy số đống sỏi khi bắt đầu game
InputStatus input_pile_count (int& pileCount);

// in ra giao diện nhập nước đi cho người chơi
void show_input_player_move (const GameState& game, const char* errorMessage = nullptr);
// lấy nước đi và trã về mã lỗi
InputStatus input_player_move (Move& move);


int show_roll_animation();

void show_error_message (const char* message);
void show_hint_message (const char* message);

//hiển thị trạng thái game
void show_current_game_state (const GameState& game);

void show_last_game_state (const GameState& lastGame);

void show_last_move (const GameState& lastGame, const Move& lastMove);


// in ra người thắng
void show_winner (const GameState& game);
