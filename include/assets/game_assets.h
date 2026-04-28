#pragma once
#include "raylib.h"

struct musicAssets {
    Music bgm;
};

struct soundAssets {
    Sound click;
    Sound end;
};



struct UIButtonStateTextures {
    Texture2D normal{};
    Texture2D hover{};
    Texture2D pressed{};
    Texture2D selected{};
};

struct UISwitchTextures {
    Texture2D off{};
    Texture2D on{};
};

struct ImageAssets {
    Texture2D bg_main_menu{};
    Texture2D bg_match_setup{};
    Texture2D bg_play{};
};

struct ButtonAssets {
    UIButtonStateTextures common{};
    UIButtonStateTextures cloud{};
    UIButtonStateTextures play_setup_decrease{};
    UIButtonStateTextures play_setup_increase{};
};

struct SwitchAssets {
    UISwitchTextures play_limit{};
};

struct FontAssets {
    Font common{};
    Font title{};
    bool ownsCommon = false;
    bool ownsTitle = false;
};

struct GameAssets {
    ImageAssets images{};
    ButtonAssets buttons{};
    SwitchAssets switches{};
    FontAssets fonts{};

    musicAssets musics{};
    soundAssets sounds{};

    bool loaded = false;
};

inline GameAssets g_assets = {};

bool game_assets_load();
void game_assets_unload();
