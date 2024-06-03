#pragma once
#include "Globals.h"
#include "Scene.h"

enum class GameState { INITIAL_SCREEN, MAIN_MENU, PLAYING, SETTINGS, CREDITS, TRANSITION, GAME_OVER };

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();

private:
    AppStatus BeginPlay();
    void FinishPlay();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene *scene;
    const Texture2D *img_menu;
    const Texture2D* img_UI;
    const Texture2D* img_InitialScreen;
    const Texture2D* img_GameOver;
    std::vector<Texture2D> img_InitialAnimation;

    int currentGameFrame;
    int framesCnt;
    int gameDuration;
    int timeBtwImages;

    int cntTime;
    int count;

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;
    Music music;
    Music musicGameOver;
    Music musicInitialAnim;

};