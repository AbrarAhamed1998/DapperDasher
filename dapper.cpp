#include "raylib.h"
#pragma region CUSTOM DATA TYPES

    class AnimData
    {
        Rectangle rec;
        Rectangle paddedRec;
        Vector2 pos;
        int frame;
        float updateTime;
        float runningTime;

        public :
        
        // Setters
        void SetUpdateTime(float val)
        {
            updateTime = val;
        }

        void SetRunningTime(float val)
        {
            runningTime = val;
        }

        void SetPositon(float x, float y)
        {
            pos.x = x;
            pos.y = y;
        }

        void SetFrame(int val)
        {
            frame = val;
        }

        void SetRectangle(float xpos, float ypos, float width, float height)
        {
            rec.x = xpos;
            rec.y = ypos;
            rec.width = width;
            rec.height = height;
        }

        void SetPos(float x, float y)
        {
            pos.x = x;
            pos.y = y;
        }

        void UpdateAnimationData(float deltaTime, int maxFrame)
        {
            runningTime += deltaTime;

            if(runningTime >= updateTime)
            {
                frame++;
                runningTime = 0.0;
            } 

            if(frame > maxFrame)
            {
                frame = 0;
            }
        }

        bool IsGrounded(int groundVal)
        {
            if (pos.y >= (groundVal - rec.height))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        void IncrementXPos(float incrementVal)
        {
            pos.x += incrementVal;
        }

        void IncrementYPos(float incrementVal)
        {
            pos.y += incrementVal;
        }

        void SetCurrentXFrame()
        {
            rec.x = frame * rec.width;
        }

        // Getters
        Rectangle GetRectangle()
        {
            return rec;
        }

        Rectangle GetPaddedRectangle()
        {
            return paddedRec;
        }

        Vector2 GetPos()
        {
            return pos;
        }

    };

#pragma endregion CUSTOM DATA TYPES

#pragma region PRIVATE METHODS


void UpdateNebulaData(AnimData &nebulaData, float nebulaXVelocity, float deltaTime)
{
    nebulaData.IncrementXPos(nebulaXVelocity * deltaTime);
    nebulaData.SetCurrentXFrame();
    nebulaData.UpdateAnimationData(deltaTime, 7);
}

void UpdateScarfyData(AnimData &scarfyData, float velocityY, float deltaTime, bool isGrounded)
{
    // update scarfy pos
        scarfyData.IncrementYPos(velocityY * deltaTime);
        
        // update animation frame
        scarfyData.SetCurrentXFrame();

        //nebulaData.rec.y = nebulaData.rec.height;

        if(isGrounded)
        {
            // dont update animation frame if scarfy is not on the ground
            scarfyData.UpdateAnimationData(deltaTime, 5);
        }
}

void HandleScrollingTextures(float &x, int scrollSpeed, float dT, Texture2D &sprite)
{
        x -= scrollSpeed * dT;


        if(x < -(sprite.width * 2.0))
        {
            x = 0;
        }

        Vector2 texture1Vector {x, 0.0};

        Vector2 texture2Vector {x+(sprite.width*2.0), 0.0};

        DrawTextureEx(sprite, texture1Vector, 0.0, 2.0, WHITE);
        DrawTextureEx(sprite, texture2Vector, 0.0, 2.0, WHITE);
}

#pragma endregion PRIVATE METHODS
#pragma region CONST FIELDS

const int windowDimensions[2] = {300,500};
const int windowHeight = windowDimensions[0];
const int windowWidth = windowDimensions[1];
const char windowName[] = "DapperDasher";
const char scarfySpritePath[] = "textures/scarfy.png";
const char nebulaSpritePath[] = "textures/12_nebula_spritesheet.png";
const char bgSpritePath[] = "textures/far-buildings.png";
const char mgSpritePath[] = "textures/back-buildings.png";
const char fgSpritePath[] = "textures/foreground.png";

const int gravity = 1000;
const int bgScrollSpeed = 20;
const int mgScrollSpeed = 40;
const int fgScrollSpeed = 80;

const int countOfNebulae = 3;

const int nebulaTexturePadding = 20;
const int scarfyTexturePadding = 0;

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

    float bgX{};
    float mgX{};
    float fgX{};


    #pragma endregion PRIVATE LOCAL FIELDS

    

    #pragma region GAME START

    InitWindow(windowWidth, windowHeight, windowName);
    SetTargetFPS(60);

    // Initialize Textures
    Texture2D nebulaSprite = LoadTexture(nebulaSpritePath);

    Texture2D bgSprite = LoadTexture(bgSpritePath);

    Texture2D mgSprite = LoadTexture(mgSpritePath);

    Texture2D fgSprite = LoadTexture(fgSpritePath);
    
    // AnimData nebulaData
    // {
    //     {0,0,nebulaSprite.width/8.0, nebulaSprite.height/8.0},
    //     {windowWidth, windowHeight - nebulaData.rec.height},
    //     0,
    //     1.0/16.0,
    //     0.0
    // };

    AnimData nebulae[countOfNebulae]{};

    for(int i=0; i<countOfNebulae; i++)
    {
        // nebulae[i].rec.x = 0;
        // nebulae[i].rec.y = 0;
        // nebulae[i].rec.width = nebulaSprite.width/8.0;
        // nebulae[i].rec.height = nebulaSprite.height/8.0;
        nebulae[i].SetRectangle(0,0,nebulaSprite.width/8.0,nebulaSprite.height/8.0);

        nebulae[i].SetPos(
            windowWidth + ((nebulaSprite.height/8.0) * (i+1) * 2),
            (windowHeight) - ((nebulaSprite.height/8.0) * GetRandomValue(1,3))
        );

        // nebulae[i].pos.x = windowWidth + ((nebulaSprite.height/8.0) * (i+1) * 2), 
        // nebulae[i].pos.y = (windowHeight) - ((nebulaSprite.height/8.0) * GetRandomValue(1,3)); 
    }

    float finishLine {nebulae[countOfNebulae-1].GetPos().x};

    // nebula x velocity in pixels/second.
    int nebulaXVelocity = -200;

    // scarfy variables
    Texture2D scarfySprite = LoadTexture(scarfySpritePath);

    AnimData scarfyData {};
    scarfyData.SetRectangle(0,0,scarfySprite.width/6.0,scarfySprite.height);
    scarfyData.SetPos(windowWidth/2.0 - scarfyData.GetRectangle().width/2.0, windowHeight - scarfyData.GetRectangle().height);
    scarfyData.SetFrame(0);
    scarfyData.SetUpdateTime(1.0/12.0);
    scarfyData.SetRunningTime(0.0);

    #pragma endregion GAME START

    #pragma region GAME LOOP

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        float deltaTime = GetFrameTime();

        #pragma region BG/MG/FG FUNCTIONALITY

        
        // bgX -= bgScrollSpeed * deltaTime;

        // if(bgX < -((bgSprite.width * 2.0) - displacement))
        // {
        //     bgX = displacement;
        // }

        // Vector2 bg1Vector {bgX, 0.0};

        // Vector2 bg2Vector {bgX+(bgSprite.width*2.0), 0.0};

        // DrawTextureEx(bgSprite, bg1Vector, 0.0, 2.0, WHITE);
        // DrawTextureEx(bgSprite, bg2Vector, 0.0, 2.0, WHITE);

        HandleScrollingTextures(bgX, bgScrollSpeed, deltaTime, bgSprite);
        HandleScrollingTextures(mgX, mgScrollSpeed, deltaTime, mgSprite);
        HandleScrollingTextures(fgX, fgScrollSpeed, deltaTime, fgSprite);

        #pragma endregion BG/MG/FG FUNCTIONALITY


        #pragma region SCARFY FUNCTIONALITY

        isGrounded = scarfyData.IsGrounded(groundVal);

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

        
        UpdateScarfyData(scarfyData, velocityY, deltaTime, isGrounded);

        DrawTextureRec(scarfySprite, scarfyData.GetRectangle(), scarfyData.GetPos(), WHITE);
        
        #pragma endregion SCARFY FUNCTIONALITY

        #pragma region NEBULA FUNCTIONALITY

        //update nebulae pos
        for(int i=0; i<3; i++)
        {
            UpdateNebulaData(nebulae[i], nebulaXVelocity, deltaTime);
            DrawTextureRec(nebulaSprite, nebulae[i].GetRectangle(), nebulae[i].GetPos(), RED);
        }

        finishLine += nebulaXVelocity * deltaTime;

        #pragma endregion NEBULAE FUNCTIONALITY

        EndDrawing();
    }
    
    #pragma endregion GAME LOOP

    #pragma region GAME END 

    UnloadTexture(scarfySprite);
    UnloadTexture(nebulaSprite);
    UnloadTexture(bgSprite);
    UnloadTexture(mgSprite);
    UnloadTexture(fgSprite);

    CloseWindow();

    #pragma endregion GAME END
}

#pragma endregion MAIN METHOD


