#include <iostream>
#include <raylib.h>
#include "config.h"

int main () {

    InitWindow(Game::screenWidth, Game::screenHeight, Game::nameGame);
    SetTargetFPS(Game::maxFPS);

    Rectangle rect1 = { 100, 100, 200, 150 };
    
    Color color1 = BLUE;
    double timer = 0.0;
    bool isHover = false;
    bool isEffect = false;



    while (!WindowShouldClose()) {

        Vector2 mousePos = GetMousePosition();


        // color1 = BLUE;

        if (CheckCollisionPointRec(mousePos, rect1)) {
            // color1 = RED;
            isHover = true;

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                timer = GetTime() + 0.5;
            }
        }

        isEffect = GetTime() < timer;

        if (isEffect) {
            color1 = GOLD;
        }
        else if (isHover) {
            color1 = RED;
        }
        else {
            color1 = BLUE;
        }
        

        BeginDrawing();
            ClearBackground(BLACK);


            DrawRectangleRec(rect1, color1);

            int x = rect1.x + rect1.width;
            int y = rect1.y + rect1.height + 10;

            if (isEffect) 
                DrawText("click", x, y, 20, WHITE);

            


        EndDrawing();
    }

    /*
    
    
    */


    CloseWindow();

    return 0;


}