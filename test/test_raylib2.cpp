// #include "raylib.h"

// int main() {
//     InitWindow(800, 600, "Nim Game");
//     SetTargetFPS(60);

//     int sticks = 21;
//     bool playerTurn = true;

//     while (!WindowShouldClose()) {

//         // Input player
//         if (playerTurn) {
//             if (IsKeyPressed(KEY_ONE)) { sticks -= 1; playerTurn = false; }
//             if (IsKeyPressed(KEY_TWO)) { sticks -= 2; playerTurn = false; }
//             if (IsKeyPressed(KEY_THREE)) { sticks -= 3; playerTurn = false; }
//         } else {
//             // AI random
//             int take = GetRandomValue(1, 3);
//             sticks -= take;
//             playerTurn = true;
//         }

//         BeginDrawing();
//         ClearBackground(RAYWHITE);

//         DrawText(TextFormat("Sticks: %d", sticks), 300, 200, 30, BLACK);
//         DrawText(playerTurn ? "Your turn (1-3)" : "AI turn", 300, 250, 20, DARKGRAY);

//         if (sticks <= 0) {
//             DrawText(playerTurn ? "AI wins!" : "You win!", 300, 300, 30, RED);
//         }

//         EndDrawing();
//     }

//     CloseWindow();
// }