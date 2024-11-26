#include "raylib.h"
int main()
{
    #pragma region CUSTOM DATA TYPES

    struct AnimData
    {
        Rectangle rec;
        Vector2 pos;
        int frame;
        float updateTime;
        float runningTime;
    };

    #pragma endregion CUSTOM DATA TYPES

    #pragma region CONST FIELDS

    const int windowDimensions[2] = {300,600};
    const int windowHeight = windowDimensions[0];
    const int windowWidth = windowDimensions[1];
    const char windowName[] = "DapperDasher";
    const char scarfySpritePath[] = "textures/scarfy.png";
    const char nebulaSpritePath[] = "textures/12_nebula_spritesheet.png";
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

    // nebula variables
    Texture2D nebulaSprite = LoadTexture(nebulaSpritePath);

    // AnimData nebulaData
    // {
    //     {0,0,nebulaSprite.width/8.0, nebulaSprite.height/8.0},
    //     {windowWidth, windowHeight - nebulaData.rec.height},
    //     0,
    //     1.0/16.0,
    //     0.0
    // };

    AnimData nebulae[3]{};

    for(int i=0; i<3; i++)
    {
        nebulae[i].rec.x = 0;
        nebulae[i].rec.y = 0;
        nebulae[i].rec.width = nebulaSprite.width/8.0;
        nebulae[i].rec.height = nebulaSprite.height/8.0;
        nebulae[i].pos.x = windowWidth, 
        nebulae[i].pos.y = (windowHeight) - ((nebulaSprite.height/8.0) * (i+1));
    }

    // nebula x velocity in pixels/second.
    int nebulaXVelocity = -200;

    // scarfy variables
    Texture2D scarfySprite = LoadTexture(scarfySpritePath);

    AnimData scarfyData
    {
        {0,0,scarfySprite.width/6.0,scarfySprite.height},
        {windowWidth/2.0 - scarfyData.rec.width/2.0, windowHeight - scarfyData.rec.height},
        0,
        1.0/12.0,
        0.0
    };

    #pragma endregion GAME START

    #pragma region GAME LOOP

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        float deltaTime = GetFrameTime();

        #pragma region SCARFY FUNCTIONALITY

        if(scarfyData.pos.y >= (groundVal-scarfyData.rec.height))
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

        
        // update scarfy pos
        scarfyData.pos.y += velocityY * deltaTime;
        
        // update animation frame
        scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
        //nebulaData.rec.y = nebulaData.rec.height;

        scarfyData.runningTime += deltaTime;
        
        if(scarfyData.runningTime >= scarfyData.updateTime)
        {
            if(isGrounded)
            {
                scarfyData.frame++;
            }
            
            scarfyData.runningTime = 0.0;
        }

        if(scarfyData.frame > 5)
        {
            scarfyData.frame = 0;
        }

        DrawTextureRec(scarfySprite, scarfyData.rec, scarfyData.pos, WHITE);
        
        #pragma endregion SCARFY FUNCTIONALITY

        #pragma region NEBULA FUNCTIONALITY

        //update nebulae pos
        for(int i=0; i<3; i++)
        {
            nebulae[i].pos.x += nebulaXVelocity * deltaTime;
            nebulae[i].rec.x = nebulae[i].frame * nebulae[i].rec.width;
            nebulae[i].runningTime += deltaTime;

            if(nebulae[i].runningTime >= nebulae[i].updateTime)
            {
                nebulae[i].frame++;
                nebulae[i].runningTime = 0.0;
            }

            if(nebulae[i].frame == 7)
            {
                nebulae[i].frame = 0;
            }

            DrawTextureRec(nebulaSprite, nebulae[i].rec, nebulae[i].pos, RED);
        }

        #pragma endregion NEBULAE FUNCTIONALITY

        EndDrawing();
    }
    
    #pragma endregion GAME LOOP

    #pragma region GAME END 

    UnloadTexture(scarfySprite);
    UnloadTexture(nebulaSprite);
    CloseWindow();

    #pragma endregion GAME END

}