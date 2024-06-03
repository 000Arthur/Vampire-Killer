#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>

Game::Game()
{
    state = GameState::INITIAL_SCREEN;
    scene = nullptr;
    img_menu = nullptr;

    currentGameFrame = 0;
    framesCnt = 0;
    gameDuration = 2.0f;
    cntTime = 0;

    target = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
}
AppStatus Game::Initialise(float scale)
{
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow((int)w, (int)h, "Vampire Killer");

    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }
    
    timeBtwImages = 2.0f / img_InitialAnimation.size();
    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);

    return AppStatus::OK;
}
AppStatus Game::LoadResources()
{
    InitAudioDevice();
    music = LoadMusicStream("Vamp Killer ost/OGG/02 Vampire Killer.ogg");
    musicGameOver = LoadMusicStream("Vamp Killer ost/OGG/10 Game Over.ogg");
    musicInitialAnim = LoadMusicStream("Vamp Killer ost/OGG/01 Prologue.ogg");


    ResourceManager& data = ResourceManager::Instance();
    if (data.LoadTexture(Resource::IMG_MENU, "images/Vampire Killer MSX Title Screen [English] 2 CUT.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);

    if (data.LoadTexture(Resource::IMG_UI, "images/UI life full BLANK.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_UI = data.GetTexture(Resource::IMG_UI);
    
    if (data.LoadTexture(Resource::IMG_IS, "images/Initial Screen.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_InitialScreen = data.GetTexture(Resource::IMG_IS);

    if (data.LoadTexture(Resource::IMA_GAMEOVER, "images/Game Over Void UI.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_GameOver = data.GetTexture(Resource::IMA_GAMEOVER);

    for (int i = 1; i <= 84;++i)
    {
        std::string path = "images/Intro Animation/Intro" + std::to_string(i) + ".png";
        Texture2D gameImage = LoadTexture(path.c_str());
        img_InitialAnimation.push_back(gameImage);
    }


    return AppStatus::OK;
}
AppStatus Game::BeginPlay()
{
    scene = new Scene();
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
void Game::FinishPlay()
{
    scene->Release();
    delete scene;
    scene = nullptr;
}
AppStatus Game::Update()
{
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if(WindowShouldClose()) return AppStatus::QUIT;
    UpdateMusicStream(music);
    UpdateMusicStream(musicInitialAnim);
    switch (state)
    {
        case GameState::INITIAL_SCREEN:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE)) state = GameState::MAIN_MENU;
        break; 

        case GameState::TRANSITION:
            framesCnt += GetFrameTime();
            if (cntTime >= 3)
            {
                if (framesCnt >= timeBtwImages)
                {
                    framesCnt = 0;
                    currentGameFrame++;
                }
                if (currentGameFrame < img_InitialAnimation.size())
                {
                    DrawTexture(img_InitialAnimation[currentGameFrame], 0, 0, WHITE);
                }
                else {
                    StopMusicStream(musicInitialAnim);
                    PlayMusicStream(music);
                    state = GameState::PLAYING;
                }
                cntTime = 0;
            }
            else cntTime++;
            break;

        case GameState::MAIN_MENU:
            
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                if(BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
                state = GameState::TRANSITION;
                PlayMusicStream(musicInitialAnim);
            }
            break;

        case GameState::PLAYING:  
            if (IsKeyPressed(KEY_ESCAPE))
            {
                FinishPlay();
                state = GameState::MAIN_MENU;
                StopMusicStream(music);
            }
            else if (scene->IsPlayerDead())
            {
                FinishPlay();
                state = GameState::GAME_OVER;
                StopMusicStream(music);
            }
            else
            {
                //Game logic
                scene->Update();
            }
            break;

        case GameState::GAME_OVER:
            UpdateMusicStream(musicGameOver);
            if (IsKeyPressed(KEY_ESCAPE))
            {
                StopMusicStream(musicGameOver);
                state = GameState::MAIN_MENU;
            }
            PlayMusicStream(musicGameOver);
            break;
    }
    return AppStatus::OK;
}
void Game::Render()
{
    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);
    
    switch (state)
    {
        case GameState::INITIAL_SCREEN:
            DrawTexture(*img_InitialScreen, 0, 0, WHITE);
            break;
        case GameState::TRANSITION:
            if (currentGameFrame < img_InitialAnimation.size())
            {
                DrawTexture(img_InitialAnimation[currentGameFrame], 0, 0, WHITE);
            }
            break;
        case GameState::MAIN_MENU:
            DrawTexture(*img_menu, 0, 0, WHITE);            
            break;

        case GameState::PLAYING:
            DrawTexture(*img_UI, 0, 0, WHITE);
            scene->Render();
            break;
        case GameState::GAME_OVER:
            DrawTexture(*img_GameOver, 0, 0, WHITE);
            break;
    }
    
    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    EndDrawing();
}
void Game::Cleanup()
{
    UnloadResources();
    CloseWindow();
}
void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_MENU);

    UnloadRenderTexture(target);
}