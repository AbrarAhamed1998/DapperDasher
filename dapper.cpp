#include "raylib.h"
int main()
{
    const int windowHeight = 300;
    const int windowWidth = 600;
    
    InitWindow(windowWidth, windowHeight, "DapperDasher");

    const int gravity = 1;

    int rectWidth = 30;
    int rectHeight = 50;
    int groundVal = windowHeight;
    int posY = windowHeight/2;
    int velocityY = 0;
    
    bool isGrounded;

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        if(posY >= (groundVal-rectHeight))
        {
            isGrounded = true;
            velocityY = 0;
        }
        else
        {
            isGrounded = false;
        }

        if(!isGrounded)
        {
            velocityY += gravity;
        }

        if(IsKeyPressed(KEY_SPACE))
        {
            velocityY -= 10; 
        }

        posY += velocityY;
        
        
        DrawRectangle(windowWidth/2, posY, rectWidth, rectHeight, BLUE);

        EndDrawing();
    }
    CloseWindow();
}