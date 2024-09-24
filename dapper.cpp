#include "raylib.h"
int main()
{
    #pragma region CONST FIELDS

    const int windowHeight = 300;
    const int windowWidth = 600;
    const char windowName[] = "DapperDasher";
    const char scarfySpritePath[] = "textures/scarfy.png";
    const int gravity = 1000;

    #pragma endregion CONST FIELDS

    #pragma region PRIVATE FIELDS

    // int rectWidth = 30;
    // int rectHeight = 50;
    int groundVal = windowHeight;
    //int posY = windowHeight/2;
    int velocityY = 0;
    int jumpForce = 600;
    
    bool isGrounded;

    #pragma endregion PRIVATE FIELDS

    #pragma region GAME START

    InitWindow(windowWidth, windowHeight, windowName);
    SetTargetFPS(60);
    Texture2D scarfySprite = LoadTexture(scarfySpritePath);
    Rectangle scarfyRect;
    scarfyRect.width = scarfySprite.width/6;
    scarfyRect.height = scarfySprite.height;
    scarfyRect.x = 0;
    scarfyRect.y = 0;

    Vector2 scarfyPos;
    scarfyPos.x = windowWidth/2 - scarfyRect.width/2;
    scarfyPos.y = windowHeight - scarfyRect.height;

    #pragma endregion GAME START

    #pragma region GAME LOOP

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        float deltaTime = GetFrameTime();

        if(scarfyPos.y >= (groundVal-scarfyRect.height))
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
            velocityY += gravity * deltaTime;
        }

        if(IsKeyPressed(KEY_SPACE) && isGrounded)
        {
            velocityY -= jumpForce; 
        }

        scarfyPos.y += velocityY * deltaTime;
        
        DrawTextureRec(scarfySprite, scarfyRect, scarfyPos, WHITE);
        
        //DrawRectangle(windowWidth/2, posY, rectWidth, rectHeight, BLUE);

        EndDrawing();
    }
    
    #pragma endregion GAME LOOP

    #pragma region GAME END 

    UnloadTexture(scarfySprite);
    CloseWindow();

    #pragma endregion GAME END

}