#include "raylib.h"
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
#pragma region PRIVATE METHODS
bool IsScarfyGrounded(AnimData &scarfyData, int groundVal)
{
    if (scarfyData.pos.y >= (groundVal - scarfyData.rec.height))
    {
        return true;
    }
    else
    {
        return false;
    }
}

AnimData UpdateAnimationData(AnimData data, float deltaTime, int maxFrame)
{
    data.runningTime += deltaTime;

    if(data.runningTime >= data.updateTime)
    {
        data.frame++;
        data.runningTime = 0.0;
    } 

    if(data.frame > maxFrame)
    {
        data.frame = 0;
    }

    return data;
}


AnimData UpdateNebulae(AnimData nebulaeData, float nebulaXVelocity, float deltaTime)
{
    nebulaeData.pos.x += nebulaXVelocity * deltaTime;
    nebulaeData.rec.x = nebulaeData.frame * nebulaeData.rec.width;
    nebulaeData = UpdateAnimationData(nebulaeData, deltaTime, 7);
    return nebulaeData;
}

AnimData UpdateScarfyData(AnimData scarfyData, float velocityY, float deltaTime, bool isGrounded)
{
    // update scarfy pos
        scarfyData.pos.y += velocityY * deltaTime;
        
        // update animation frame
        scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
        //nebulaData.rec.y = nebulaData.rec.height;

        if(isGrounded)
        {
            // dont update animation frame if scarfy is not on the ground
            scarfyData = UpdateAnimationData(scarfyData, deltaTime, 5);
        }
        
        return scarfyData;
}

#pragma endregion PRIVATE METHODS
#pragma region CONST FIELDS

const int windowDimensions[2] = {300,600};
const int windowHeight = windowDimensions[0];
const int windowWidth = windowDimensions[1];
const char windowName[] = "DapperDasher";
const char scarfySpritePath[] = "textures/scarfy.png";
const char nebulaSpritePath[] = "textures/12_nebula_spritesheet.png";
const int gravity = 1000;

#pragma endregion CONST FIELDS
#pragma region MAIN METHOD

int main()
{
    #pragma region PRIVATE LOCAL FIELDS

    // int rectWidth = 30;
    // int rectHeight = 50;
    int groundVal = windowHeight;
    //int posY = windowHeight/2;
    int velocityY = 0;
    int jumpForce = 600;
    
    bool isGrounded;

    #pragma endregion PRIVATE LOCAL FIELDS

    

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
        nebulae[i].pos.x = windowWidth + ((nebulaSprite.height/8.0) * (i+1)), 
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

        isGrounded = IsScarfyGrounded(scarfyData, groundVal);

        if(!isGrounded)
        {
            velocityY += gravity * deltaTime;
        }
        else
        {
            velocityY = 0;
        }

        if(IsKeyPressed(KEY_SPACE) && isGrounded)
        {
            velocityY -= jumpForce; 
        }

        
        scarfyData = UpdateScarfyData(scarfyData, velocityY, deltaTime, isGrounded);

        DrawTextureRec(scarfySprite, scarfyData.rec, scarfyData.pos, WHITE);
        
        #pragma endregion SCARFY FUNCTIONALITY

        #pragma region NEBULA FUNCTIONALITY

        //update nebulae pos
        for(int i=0; i<3; i++)
        {
            nebulae[i] = UpdateNebulae(nebulae[i], nebulaXVelocity, deltaTime);
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

#pragma endregion MAIN METHOD


