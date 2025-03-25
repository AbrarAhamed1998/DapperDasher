#include "raylib.h"
#pragma region CUSTOM DATA TYPES

    class AnimData
    {
        Rectangle rec;
        Rectangle currentPaddedRec;
        Vector2 pos;
        int frame;
        float updateTime;
        float runningTime;
        int paddingValue;

        public :
        
        void SetTexturePaddingValue(int val)
        {
            paddingValue = val;
        }

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

            currentPaddedRec.width = rec.width - (2 * paddingValue);
            currentPaddedRec.height = rec.width - (2 * paddingValue);
        }

        void SetPos(float x, float y)
        {
            pos.x = x;
            pos.y = y;

            currentPaddedRec.x = pos.x + paddingValue;
            currentPaddedRec.y = pos.y + paddingValue;
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
            return currentPaddedRec;
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
const char gameOverText[] = "Game Over!";
const char gameWonText[] = "You Win!";

const int gravity = 1000;
const int bgScrollSpeed = 20;
const int mgScrollSpeed = 40;
const int fgScrollSpeed = 80;

const int countOfNebulae = 3;

const int nebulaTexturePadding = 50z;
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
    bool collision{};

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
    
    AnimData nebulae[countOfNebulae]{};

    for(int i=0; i<countOfNebulae; i++)
    {
        nebulae[i].SetTexturePaddingValue(nebulaTexturePadding);
        nebulae[i].SetRectangle(0,0,nebulaSprite.width/8.0,nebulaSprite.height/8.0);

        nebulae[i].SetPos(
            windowWidth + ((nebulaSprite.height/8.0) * (i+1) * 2),
            (windowHeight) - ((nebulaSprite.height/8.0) * GetRandomValue(1,3))
        );
    }

    float finishLine {nebulae[countOfNebulae-1].GetPos().x};

    // nebula x velocity in pixels/second.
    int nebulaXVelocity = -200;

    // scarfy variables
    Texture2D scarfySprite = LoadTexture(scarfySpritePath);

    AnimData scarfyData {};
    scarfyData.SetTexturePaddingValue(scarfyTexturePadding);
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
        
        HandleScrollingTextures(bgX, bgScrollSpeed, deltaTime, bgSprite);
        HandleScrollingTextures(mgX, mgScrollSpeed, deltaTime, mgSprite);
        HandleScrollingTextures(fgX, fgScrollSpeed, deltaTime, fgSprite);

        #pragma endregion BG/MG/FG FUNCTIONALITY

        #pragma region WIN OR LOSE CONDITION

        for(AnimData nebula : nebulae)
        {
            Rectangle nebRect {
                nebula.GetPos().x + nebulaTexturePadding,
                nebula.GetPos().y + nebulaTexturePadding,
                nebula.GetPaddedRectangle().width,
                nebula.GetPaddedRectangle().height
            };

            Rectangle scarfyRect {
                scarfyData.GetPos().x,
                scarfyData.GetPos().y,
                scarfyData.GetPaddedRectangle().width,
                scarfyData.GetPaddedRectangle().height
            };

            if(CheckCollisionRecs(nebRect, scarfyRect))
            {
                collision = true;
            }

            if(collision)
                break;
        }

        if(collision)
        {
            DrawText(gameOverText, windowWidth/4, windowHeight/2, 40, RED);
        }


        if(scarfyData.GetPos().x >= finishLine && !collision)
        {
            DrawText(gameWonText, windowWidth/4, windowHeight/2, 40, GREEN);
        }

        #pragma endregion WIN OR LOSE CONDITION

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

        

        if(!collision)
        {
            DrawTextureRec(scarfySprite, scarfyData.GetRectangle(), scarfyData.GetPos(), WHITE);
        }

        
        #pragma endregion SCARFY FUNCTIONALITY

        #pragma region NEBULA FUNCTIONALITY

        //update nebulae pos
        for(int i=0; i<3; i++)
        {
            UpdateNebulaData(nebulae[i], nebulaXVelocity, deltaTime);
            if(!collision)
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


