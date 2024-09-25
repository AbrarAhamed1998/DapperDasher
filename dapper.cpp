#include "raylib.h"
int main()
{
    #pragma region CONST FIELDS

    const int windowHeight = 300;
    const int windowWidth = 600;
    const char windowName[] = "DapperDasher";
    const char scarfySpritePath[] = "textures/scarfy.png";
    const char nebulaSpritePath[] = "textures/12_nebula_spritesheet.png";
    const int gravity = 1000;
    const float animationUpdateTime = 1.0/12.0;

    #pragma endregion CONST FIELDS

    #pragma region PRIVATE FIELDS

    // int rectWidth = 30;
    // int rectHeight = 50;
    int groundVal = windowHeight;
    //int posY = windowHeight/2;
    int velocityY = 0;
    int jumpForce = 600;
    
    bool isGrounded;

    int scarfyFrameX = 0;

    float runningTime = 0.0;

    int nebulaFrameX = 0;
    int nebulaFrameY = 0;

    #pragma endregion PRIVATE FIELDS

    #pragma region GAME START

    InitWindow(windowWidth, windowHeight, windowName);
    SetTargetFPS(60);

    // nebula variables
    Texture2D nebulaSprite = LoadTexture(nebulaSpritePath);
    Rectangle nebulaRect;
    nebulaRect.width = nebulaSprite.width/8.0;
    nebulaRect.height = nebulaSprite.height/8.0;
    nebulaRect.x = 0;
    nebulaRect.y = 0;

    Vector2 nebulaPos;
    nebulaPos.x = 0;
    nebulaPos.y = 0;

    // scarfy variables
    Texture2D scarfySprite = LoadTexture(scarfySpritePath);
    Rectangle scarfyRect;
    scarfyRect.width = scarfySprite.width/6.0;
    scarfyRect.height = scarfySprite.height;
    scarfyRect.x = 0;
    scarfyRect.y = 0;

    Vector2 scarfyPos;
    scarfyPos.x = windowWidth/2.0 - scarfyRect.width/2.0;
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
        
        // update animation frame
        scarfyRect.x = scarfyFrameX * scarfyRect.width;
        nebulaRect.x = nebulaFrameX * nebulaRect.width;
        nebulaRect.y = nebulaFrameY * nebulaRect.height;

        runningTime += deltaTime;
        
        if(runningTime >= animationUpdateTime)
        {
            scarfyFrameX++;
            nebulaFrameX++;
            
            runningTime = 0.0;
        }



        if(scarfyFrameX > 5)
        {
            scarfyFrameX = 0;
        }

        DrawTextureRec(scarfySprite, scarfyRect, scarfyPos, WHITE);
        
        if(nebulaFrameX == 4 && nebulaFrameY == 7)
        {
            nebulaFrameX = 0;
            nebulaFrameY = 0;
        }

        if(nebulaFrameX > 7)
        {
            nebulaFrameX = 0;
            nebulaFrameY++;
            if(nebulaFrameY > 7)
            {
                nebulaFrameY = 0;
            }
        }


        DrawTextureRec(nebulaSprite, nebulaRect, nebulaPos, WHITE);

        EndDrawing();
    }
    
    #pragma endregion GAME LOOP

    #pragma region GAME END 

    UnloadTexture(scarfySprite);
    CloseWindow();

    #pragma endregion GAME END

}