#pragma once

#include "game/game_settings.h"

struct GameAppState;

void screen_play_start_new_match(const MatchConfig& matchConfig, const GameSettings& settings, int pileCount);
void screen_play_continue_match();
void screen_play_shutdown();

const char* screen_play_get_actor_name(int actorIndex);
int screen_play_get_score(int actorIndex);

void screen_play_update(GameAppState& app, float dt);
void screen_play_draw();
