#pragma once

#include "game.h"

enum MainMenu {
    MAIN_MENU_EXIT = 0,
    MAIN_MENU_START = 1,
    MAIN_MENU_COUNT
};

enum ExitGame {
    EXIT_GAME_NO = 0,
    EXIT_GAME_YES = 1,
    EXIT_GAME_COUNT
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

// xóa màn hình
void clear_screen ();



// in ra menu chính và trả về lựa chọn
int show_main_menu ();

// in ra menu chế độ chơi và trả về lựa chọn
int show_game_mode_menu ();

// in ra menu lúc xác nhận thoát game
int show_exit_game_menu ();



// in ra giao diện nhập số đống sỏi
void show_input_pile_count (const GameSettings& settings);

// lấy số đống sỏi khi bắt đầu game
InputStatus input_pile_count (int& pileCount);

// in ra giao diện nhập nước đi cho người chơi
void show_input_player_move (const GameState& game);

// lấy nước đi và trã về mã lỗi
InputStatus input_player_move (Move& move);


//hiển thị trạng thái game
void show_current_game_state (const GameState& game);

void show_last_game_state (const GameState& oldGame);

void show_last_move (const GameState& game, int lastActor, const Move& lastMove);


// in ra người thắng
void show_winner (const GameState& game);