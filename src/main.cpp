#include "raylib.h"
#include "app/game_app.h"
#include "config/config.h"
#include "assets/game_assets.h"

int main() {
    InitWindow(AppConfig::WINDOW_WIDTH, AppConfig::WINDOW_HEIGHT, AppConfig::WINDOW_TITLE);
    SetExitKey(KEY_NULL);
    SetTargetFPS(AppConfig::TARGET_FPS);

    InitAudioDevice();
    Music bgm = LoadMusicStream("assets/music/beanfeast.mp3");
    PlayMusicStream(bgm);
    SetMusicVolume(bgm, 0.1f);

    game_assets_load();

    GameAppState app;
    game_app_init(app);

    while (!WindowShouldClose() && !app.shouldQuit) {
        UpdateMusicStream(bgm);

        float dt = GetFrameTime();
        game_app_update(app, dt);

        BeginDrawing();
            game_app_draw(app);

        EndDrawing();
    }

    UnloadMusicStream(bgm);
    UnloadSound(g_assets.sounds.click);
    UnloadSound(g_assets.sounds.end);
    game_assets_unload();

    CloseWindow();
    return 0;
}
