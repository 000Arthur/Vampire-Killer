#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "Entity.h"
#include "Panther.h"
#include "EnemyManager.h"
#include "ShotManager.h"


#define NUM_BOOT 10
#define NUM_WING 20

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

class Scene
{
public:
    Scene();
    ~Scene();

    AppStatus Init();
    void Update();
    void Render();
    void Release();

    bool IsPlayerDead();

private:
    AppStatus LoadLevel(int stage);
    
    void CheckCollisions(); 
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    int numStage;

    Player *player;
    TileMap *level;
    std::vector<Object*> objects;

    EnemyManager* enemies;
    ShotManager* shots;
    //Enemies present in the level

    Camera2D camera;
    DebugMode debug;
};

