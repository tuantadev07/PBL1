#include "game_settings.h"

#include <fstream>
#include <string.h>

const char* filePath = "src/settings.txt";

GameSettings load_game_settings() {
    GameSettings settings;

    std::ifstream file(filePath);

    if (!file.is_open()) {
        return settings;
    }

    char key[1000];
    int value;

    while (file >> key >> value) {
        if (strcmp(key, "minPileCount") == 0) {
            settings.minPileCount = value;
        } 
        else if (strcmp(key, "maxPileCount") == 0) {
            settings.maxPileCount = value;
        } 
        else if (strcmp(key, "minInitialStoneCount") == 0) {
            settings.minInitialStoneCount = value;
        }
        else if (strcmp(key, "maxInitialStoneCount") == 0) {
            settings.maxInitialStoneCount = value;
        }
        else if (strcmp(key, "maxTakePerMove") == 0) {
            settings.maxTakePerMove = value;
        } else if (strcmp(key, "gameRule") == 0) {  
            settings.gameRule = (GameRule)value; 
        }
    }

    return settings;
}