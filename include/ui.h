#pragma once

#include "game.h"

enum MainMenu {
    MAIN_MENU_EXIT = 0,
    MAIN_MENU_START = 1,
    MAIN_MENU_COUNT
};

enum GameMode {
    GAME_MODE_EXIT = 0,
    GAME_MODE_PVP = 1,
    GAME_MODE_PVAI = 2,
    GAME_MODE_AIVAI = 3,
    GAME_MODE_COUNT
};

enum GameRule {
    GAME_RULE_LAST_TAKE_LOSE = 0,
    GAME_RULE_LAST_TAKE_WIN,
    GAME_RULE_COUNT
};

enum ExitGame {
    EXIT_GAME_NO = 0,
    EXIT_GAME_YES = 1,
    EXIT_GAME_COUNT
};

enum InputMoveStatus {
    INPUT_MOVE_OK = 0,
    INPUT_MOVE_INVALID_FORMAT,
    INPUT_MOVE_TOO_LONG
};


// dọn ký tụ rác và thừa còn lại
void clear_line ();

// chờ người dùng ấn entẻ
void wait_enter ();

// xóa màn hình
void clear_screen ();



// in ra menu chính và trả về lựa chọn
int show_main_menu ();

// in ra menu chế độ chơi và trả về lựa chọn
int show_game_mode_menu ();

// in ra menu lúc xác nhận thoát game
int show_exit_game_menu ();



// nhập số đống sỏi
int input_pile_count ();

// in ra giao diện nhập nước đi cho người chơi
void show_input_player_move (const GameState& game);

// lấy nước đi và trã về mã lỗi
InputMoveStatus input_player_move (Move& move);


//hiển thị trạng thái game
void show_current_game_state (const GameState& game);

void show_last_game_state (const GameState& oldGame);

void show_last_move (const GameState& game, int lastActor, const Move& lastMove);


// in ra nước đi của AI
void show_ai_move (const GameState& game, const Move& move);

// in ra người thắng
void show_winner (const GameState& game);