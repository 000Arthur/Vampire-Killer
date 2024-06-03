#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

Scene::Scene()
{
	player = nullptr;
    level = nullptr;
	enemies = nullptr;
	shots = nullptr;
	numStage = 1;

	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { 0, MARGIN_GUI_Y };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom

	debug = DebugMode::OFF;
}
Scene::~Scene()
{
	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
    if (level != nullptr)
    {
		level->Release();
        delete level;
        level = nullptr;
    }
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	if (enemies != nullptr)
	{
		enemies->Release();
		delete enemies;
		enemies = nullptr;
	}
	if (shots != nullptr)
	{
		delete shots;
		shots = nullptr;
	}
}
AppStatus Scene::Init()
{
	//Create player
	player = new Player({ 0,0 }, State::IDLE, Look::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	//Initialise player
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}

	//Create enemy manager
	enemies = new EnemyManager();
	if (enemies == nullptr)
	{
		LOG("Failed to allocate memory for Enemy Manager");
		return AppStatus::ERROR;
	}
	//Initialise enemy manager
	if (enemies->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy Manager");
		return AppStatus::ERROR;
	}

	//Create shot manager 
	shots = new ShotManager();
	if (shots == nullptr)
	{
		LOG("Failed to allocate memory for Shot Manager");
		return AppStatus::ERROR;
	}
	//Initialise shot manager
	if (shots->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Shot Manager");
		return AppStatus::ERROR;
	}

	//Create level 
    level = new TileMap();
    if (level == nullptr)
    {
        LOG("Failed to allocate memory for Level");
        return AppStatus::ERROR;
    }
	//Initialise level
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	//Load level
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}
	//Assign the tile map reference to the player to check collisions while navigating
	player->SetTileMap(level);
	//Assign the tile map reference to the shot manager to check collisions when shots are shot
	shots->SetTileMap(level);
	//Assign the shot manager reference to the enemy manager so enemies can add shots
	enemies->SetShotManager(shots);
    return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int *map = nullptr;
	Object *obj;
	AABB hitbox, area;
	
	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 1)
	{
		numStage = 1;
		map = new int[size] {
			130, 131, 132, 133, 130, 131, 132, 133, 130, 131, 132, 133, 130, 131, 132, 133,
				134, 135, 136, 137, 134, 135, 136, 137, 134, 135, 136, 137, 134, 135, 136, 137,
				138, 139, 140, 141, 138, 139, 140, 141, 138, 139, 140, 141, 138, 139, 140, 141,
				142, 143, 144, 145, 142, 143, 144, 145, 142, 143, 144, 145, 142, 143, 144, 145,
				146, 147, 148, 149, 146, 147, 148, 149, 146, 147, 148, 149, 146, 147, 148, 149,
				150, 151, 152, 153, 160, 161, 162, 163, 150, 161, 162, 163, 160, 161, 162, 163,
				154, 155, 120, 121, 164, 165, 154, 165, 154, 165, 154, 155, 166, 165, 154, 165,
				103, 102, 122, 123, 63, 102, 104, 102, 104, 102, 104, 102, 63, 102, 103, 102,
				300, 100, 124, 125, 101, 100, 100, 100, 100, 100, 100, 100, 101, 100, 100, 100,
				14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
				105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105

			};
		player->InitScore();
	}
	else if (stage == 2)
	{
		map = new int[size] {
			130, 131, 132, 133, 130, 131, 132, 133, 130, 131, 132, 133, 130, 131, 132, 133,
				134, 135, 136, 137, 134, 135, 136, 137, 134, 135, 136, 137, 134, 135, 136, 137,
				138, 139, 140, 141, 138, 139, 140, 141, 138, 139, 140, 141, 138, 139, 140, 141,
				142, 143, 144, 145, 142, 143, 144, 145, 142, 143, 144, 145, 142, 143, 144, 145,
				146, 147, 148, 149, 146, 147, 148, 149, 146, 147, 148, 149, 146, 147, 148, 149,
				150, 151, 152, 153, 160, 161, 162, 163, 150, 161, 162, 163, 160, 161, 162, 163,
				154, 155, 120, 121, 164, 165, 154, 165, 154, 165, 154, 155, 166, 165, 154, 165,
				103, 102, 122, 123, 63, 102, 104, 102, 104, 102, 104, 102, 63, 102, 103, 102,
				100, 100, 124, 125, 101, 100, 100, 100, 100, 100, 100, 100, 101, 100, 100, 100,
				14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
				105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105
			};
	}
	else if (stage == 3)
	{
		map = new int[size] {
			130, 131, 132, 133, 130, 131, 132, 133, 130, 131, 180, 181, 182, 181, 182, 181,
				134, 135, 136, 137, 134, 135, 136, 137, 134, 135, 183, 184, 185, 185, 186, 187,
				138, 139, 140, 141, 138, 139, 140, 141, 138, 139, 188, 189, 190, 191, 192, 193,
				142, 143, 144, 145, 142, 143, 144, 145, 142, 143, 194, 195, 196, 197, 198, 199,
				146, 147, 148, 149, 146, 147, 148, 149, 146, 147, 188, 201, 202, 197, 198, 203,
				150, 151, 152, 153, 160, 161, 162, 163, 150, 161, 194, 195, 196, 197, 198, 199,
				154, 155, 120, 121, 164, 165, 154, 165, 154, 165, 188, 201, 202, 197, 198, 203,
				103, 102, 122, 123, 63, 102, 103, 102, 103, 102, 204, 205, 206, 197, 207, 208,
				100, 100, 124, 125, 101, 100, 100, 100, 100, 100, 204, 205, 206, 197, 207, 208,
				14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
				105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105
			};
	}

	else if (stage == 4)
	{
		map = new int[size] {
			20,  21,  30,  31,   0,   0,   5,   6,  20,  21,  34,  35,   0,    0,   9,  10,  
			22,  23,  32,  33,   0,   0,   7,   8,  22,  23,  39,   0,   0,    0,   0,   0,  
			22,  23,  34,  35,   0,   0,   9,  10,  22,  23,  40,   0,   0,    0,   0,   0,
			22,  23,  36,   0,  15,  15,   0,   0,  22,  23,   3,   4,   15,  15,   0,   0,
			22,  23,  37,   0,  15,  15,   0,   0,  22,  23,   5,   6,   15,  15,   0,   0,
			22,  23,  38,   0,  15,  15,   0,   0,  22,  23,   7,   8,   15,  15,   0,   0,
			22,  23,  38,   0,  15,  15,   0,   0,  22,  23,   9,  10,   15,  15,   0,   0,
			22,  23,  39,   0,  15,  15,   0,  60,  22,  23,   0,  60,   15,  15,   0,   0,
			22,  23,  40,  90,  15,  15,   0,  91,  22,  23,   0,   0,   15,  15,   0,   0,
			24,  25,   0,   0,   0,   0,   0,   0,  24,  25,   0,   0,    0,   0,   0,   0,  
			11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,   11,  12,  11,   12
			};
	}

	else if (stage == 5)
	{
		map = new int[size] {
			20,   21,   30,  31,   0,   0,   5,   6,   0,   1,   80,   0,   0,   0,   9,  10,  
			22,   23,   32,  33,  15,  15,  15,  15,   0,   0,   82,   80,   0,   0,   0,   0,  
			22,   23,   34,  35,  15,  15,  15,  15,   0,  60,   0,   1,   80,   0,   0,   0,  
			22,   23,   36,   0,  15,  15,  15,  15,   0,   0,   0,   0,   82,   86,   0,   0,  
			22,   23,   37,   0,  15,  15,  15,  15,   0,   0,  85,  13,  11,  12,  11,  12,  
			22,   23,   38,   0,   7,   8,   0,   0,   0,  87,  83,   0,  20,  21,   3,   4,
			22,   23,   38,   0,   9,  10,  85,  13,  11,  12,   2,  60,  22,  23,   5,   6,  
			22,   23,   39,  60,   0,  81,  83,  60,   0,   0,   0,   0,  22,  23,   7,   8,  
			22,   23,   40,   0,  81,  83,   2,   0,   0,   0,   0,   0,  22,  23,   9,  10,  
			24,   25,    0,  87,  83,   0,   0,   0,   0,   0,   0,   0,  24,  25,   0,   0,  
			11,   12,   11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,   12
			};
	}

	else if (stage == 6)
	{
		map = new int[size] {
			 5,   6,  20,  21,  34,  35,   0,   0,   0,   0,   0,   0,   9,  10,   0,   0,
			 7,   8,  22,  23,  39,   0,   0,   0,  15,  15,   0,   0,   0,   0,   0,   0,
			 9,  10,  22,  23,  40,   0,   0,   0,  15,  15,   0,   0,   0,   0,  85,  13,
			 0,   0,  24,  25,   0,   0,   0,   0,  15,  15,   0,   0,   0,  81,  83,   0,
			11,  12,  11,  12,  13,  84,   0,   0,  15,  15,   0,   0,  81,  83,   2,   0,
			 0,   0,  20,  21,   0,  82,  86,   0,   0,   0,   0,  87,  83,   0,   3,   4,
			 0,   0,  22,  23,  11,  12,  11,  12,  11,  12,  11,  12,   2,   0,   5,   6,
			 0,   0,  22,  23,   7,   8,  11,  12,  11,  12,   0,   0,   0,   0,   7,   8,
			 0,   0,  22,  23,   9,  10,  11,  12,  11,  12,   0,   0,   0,   0,   9,  10,
			 0,   0,  22,  23,   0,   0,  11,  12,  11,  12,   0,   0,   0,   0,   0,   0,
			11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,
			};
	}

	else if (stage == 7)
	{
		map = new int[size] {
			 9,  10,   0,   0,  81,   0,   2,   0,   0,   0,   5,   6,  20,  21,  30,  31,
			 0,   0,   0,  87,  83,   0,   0,   0,   0,   0,   7,   8,  22,  23,  32,  33,  
			11,  12,  11,  12,  11,  12,  13,  84,   0,   0,   9,  10,  22,  23,  34,  35,
			20,  21,   7,   8,   0,   0,   0,  82,  80,   0,   0,   0,  22,  23,  36,   0,
			22,  23,   9,  10,   0,   0,   0,   1,  82,  80,   0,   0,  22,  23,  37,   0,
			22,  23,   0,   0,   0,   0,   0,   0,   0,  82,  86,   0,  26,  23,  38,   0,
			22,  23,  11,  12,  11,  0,  85,  13,  11,  12,  11,  12,  27,  23,  38,   0,
			22,  23,   0,   0,   0,  81,  83,   0,   0,   0,   7,   8,  22,  23,  38,   0,
			26,  23,   0,   0,  81,  83,   2,   0,   0,   0,   9,  10,  22,  23,  40,   0,
			22,  23,   0,  87,  83,   0,   0,   0,   0,   0,   0,   0,  22,  23,   0,   0,
			11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,
			};
	}

	else if (stage ==14)
	{
		map = new int[size] {
			11,  12,   0,   0,  20,  21,   0,   0,   0,   0,   0,   0,   9,  10,  20,  21,
			11,  12,   0,   0,  22,  23,   0,   0,   0,   0,   0,   0,   3,   4,  22,  23,
			11,  12,  11,  12,  22,  23,  11,  12,  13,  84,   0,   0,   5,   6,  22,  23,
			11,  12,   0,   0,  22,  23,   7,   8,   0,  82,  80,   0,   7,   8,  22,  23,
			11,  12,   0,   0,  22,  23,   9,  10,   0,   1,  82,  80,   9,  10,  22,  23,
			11,  12,   0,   0,  22,  23,   0,   0,   3,   4,   0,  82,  86,   0,  22,  23,
			11,  12,  11,  12,  11,  12,  11,  12,   5,   6,   0,   1,  11,  12,  11,  12,
			11,  12,   7,   8,  20,  21,   0,   0,   7,   8,   0,   0,   0,   0,  20,  21,
			11,  12,   9,  10,  22,  23,   0,   0,   9,  10,   0,   0,   0,   0,  22,  23,
			11,  12,   0,   0,  24,  25,   0,   0,   0,   0,   0,   0,   0,   0,  24,  25,
			11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,
			};
	}

	else if (stage ==15)
	{
		map = new int[size] {
			 0,   0,  20,  21,  34,  35,   0,   0,   5,   6,   0,   0,  20,  21,  30,  31,
			 3,   4,  22,  23,  36,   0,  15,  15,  15,  15,   0,   0,  22,  23,  32,  33,
			 5,   6,  22,  23,  37,   0,  15,  15,  15,  15,   0,   0,  22,  23,  34,  35,
			 7,   8,  22,  23,  39,   0,  15,  15,  15,  15,   0,   0,  22,  23,  36,   0,
			 9,  10,  26,  23,  40,   0,  15,  15,  15,  15,   0,   0,  22,  23,  37,   0,
			 0,   0,  24,  25,   0,   0,   7,   8,   0,   0,   3,   4,  22,  23,  39,   0,
			11,  12,  11,  12,  13,  84,   9,  10,   0,   0,   5,   6,  22,  23,  40,   0,
			 0,   0,   7,   8,   0,  82,  80,   0,   0,   0,   7,   8,  26,  23,   0,   0,
			 0,   0,   9,  10,   0,   1,  82,  80,   0,   0,   9,  10,  27,  23,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,  82,  86,   0,   0,   0,  24,  25,   0,   0,
			11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  13,  84,  11,  12,  11,  12,
			};
	}

	else if (stage == 16)
	{
		map = new int[size] {
			 0,   0,   9,  10,  20,  21,  30,  31,   0,   0,   5,   6,   0,   0,   5,   6,
			 3,   4,   0,   0,  22,  23,  32,  33,   0,   0,  15,  15,   0,   0,  15,  15,
			 5,   6,   0,   0,  22,  23,  34,  35,   0,   0,  15,  15,   0,   0,  15,  15,
			15,  15,   0,   0,  22,  23,  36,   0,   3,   4,  15,  15,   0,   0,  15,  15,
			15,  15,   0,   0,  22,  23,  37,   0,   5,   6,  15,  15,   0,   0,  15,  15,
			15,  15,   3,   4,  22,  23,  38,   0,   7,   8,   0,   0,   0,   0,   0,   0,
			15,  15,   5,   6,  22,  23,  38,   0,   9,  10,  85,  13,  11,  12,  11,  12,
			15,  15,   7,   8,  22,  23,  39,   0,   0,  81,  83,   0,   7,   8,   0,   0,
			15,  15,   9,  10,  22,  23,  40,   0,  81,  83,   2,   0,   9,  10,   0,   0,
			 0,   0,   0,   0,  24,  25,   0,  87,  83,   0,   0,   0,   0,   0,   0,   0,
			11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,
			};
	}

	else if (stage == 17)
	{
		map = new int[size] {
			20,  21,  30,  31,   0,   0,   5,   6,   0,   0,   0,   0,   5,   6,  11,  12,
			22,  23,  32,  33,   3,   4,  15,  15,   0,   0,   0,   0,   7,   8,  50,  51,
			22,  23,  34,  35,   5,   6,  15,  15,   0,   0,   0,   0,   9,  10,  52,  53,
			26,  23,  39,   0,   7,   8,  15,  15,   0,   0,   0,   0,   0,   0,  54,  55,
			27,  23,  40,   0,   9,  10,  15,  15,   0,   0,  85,  13,  11,  12,  11,  12,
			22,  23,   0,   0,   0,   0,   0,   0,   0,  87,  83,   0,   3,   4,  11,  12,
			11,  12,  11,  12,  11,  12,  11,  12,  11,  12,   2,   0,   5,   6,  11,  12,
			20,  21,   7,   8,   0,   0,   0,   0,   7,   8,   0,   0,   7,   8,  11,  12,
			22,  23,   9,  10,   0,   0,   0,   0,   9,  10,   0,   0,  11,  12,  11,  12,
			24,  25,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  11,  12,  11,  12,
			11,  12,  11,  12,  85,  13,  11,  12,  11,  12,  11,  12,  11,  12,  11,  12,
			};
	}

	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;	
	}

	//Entities and objects
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			if (tile == Tile::EMPTY)
			{
				map[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				player->SetPos(pos);
				map[i] = 100;
			}
			else if (tile == Tile::BOOTS)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::BOOTS);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::WING)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::WING);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::SLIME)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;

				hitbox = enemies->GetEnemyHitBox(pos, EnemyType::SKELETON);
				area = level->GetSweptAreaX(hitbox);
				enemies->Add(pos, EnemyType::SKELETON, area);
			}
			else if (tile == Tile::TURRET_LEFT)
			{
				hitbox = enemies->GetEnemyHitBox(pos, EnemyType::TURRET);
				area = level->GetSweptAreaX(hitbox);
				enemies->Add(pos, EnemyType::TURRET, area, Look::LEFT);
			}
			else if (tile == Tile::TURRET_RIGHT)
			{
				hitbox = enemies->GetEnemyHitBox(pos, EnemyType::TURRET);
				area = level->GetSweptAreaX(hitbox);
				enemies->Add(pos, EnemyType::TURRET, area, Look::RIGHT);
			}
			++i;
		}
	}
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);

	level->ClearObjectEntityPositions();

	delete map;

	return AppStatus::OK;
}
void Scene::Update()
{
	Point p1, p2, pos;
	AABB box,area;

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}
	//Debug levels instantly
	if ((player->getPos() <= 0) && numStage > 1 && numStage != 4|| IsKeyPressed(KEY_F3) && numStage != 1) {
		numStage--;
		LoadLevel(numStage);

		pos.x = 239;
		pos.y = player->getPosY();
		player->SetPos(pos);
		
	}
	else if (((player->getPos() >= 240)  ||IsKeyPressed(KEY_F2)) && numStage != 7 && numStage != 17) {
		numStage++;
		LoadLevel(numStage);
		pos.x = 10;
		pos.y = player->getPosY();
		player->SetPos(pos);

	}
	else if (player->getPosY() <= 0) {
		numStage+=10;
		LoadLevel(numStage);
		if (numStage == 15)pos.x = player->getPos() + 35;
		else pos.x = player->getPos() - 25;
		pos.y = 190;
		player->SetPos(pos);

	}
	else if(IsKeyPressed(KEY_F10) && numStage >=4 && numStage <= 7)
	{ 
		numStage += 10;
		LoadLevel(numStage);

		pos.x = 35;
		pos.y = player->getPosY()+5;
		player->SetPos(pos);

	}
	else if (player->getPosY() >= 200) {
		numStage-=10;
		LoadLevel(numStage);
		if (numStage == 5) pos.x = player->getPos() - 60;
		else pos.x = player->getPos() + 40;
		pos.y = 0;
		player->SetPos(pos);

	}
	else if (IsKeyPressed(KEY_F9) && numStage >= 14 && numStage <= 17)
	{
		numStage -= 10;
		LoadLevel(numStage);
		pos.x = 10;
		pos.y = player->getPosY();
		player->SetPos(pos);
	}
	else if (IsKeyPressed(KEY_F5))
	{
		enemies->Release();
	}
	else if (IsKeyPressed(KEY_F6))
	{
		Look look = player->getLook();

		if (look == Look::RIGHT)
		{
			pos.x = 220;
			pos.y = 9 * TILE_SIZE + TILE_SIZE +8;

			box = enemies->GetEnemyHitBox(pos, EnemyType::TURRET);
			area = level->GetSweptAreaX(box);
			area.width += 150;
			area.height += 150;
			area.pos.x -= 100;
			enemies->Add(pos, EnemyType::TURRET, area, Look::RIGHT);
		}

		else
		{
			pos.x = 0;
			pos.y = 9 * TILE_SIZE + TILE_SIZE + 8;

			box = enemies->GetEnemyHitBox(pos, EnemyType::TURRET);
			area = level->GetSweptAreaX(box);
			area.width += 150;
			area.height += 150;
			enemies->Add(pos, EnemyType::TURRET, area, Look::LEFT);
		}

	}
	else if (IsKeyPressed(KEY_F7))
	{

		Look look = player->getLook();

		if(look == Look::RIGHT)
		{ 
			pos.x =280;
			pos.y = 9 * TILE_SIZE + TILE_SIZE - 1;

			box = enemies->GetEnemyHitBox(pos, EnemyType::SKELETON);
			area = level->GetSweptAreaX(box);
			enemies->Add(pos, EnemyType::SKELETON, area, Look::RIGHT);
		}
		else
		{
			pos.x = - 40;
			pos.y = 9 * TILE_SIZE + TILE_SIZE - 1;

			box = enemies->GetEnemyHitBox(pos, EnemyType::SKELETON);
			area = level->GetSweptAreaX(box);
			enemies->Add(pos, EnemyType::SKELETON, area, Look::LEFT);
		
		}
	}
	else if (IsKeyPressed(KEY_F8))
	{

		Look look = player->getLook();

		if (look == Look::RIGHT)
		{
			pos.x = 280;
			pos.y = 100;

			box = enemies->GetEnemyHitBox(pos, EnemyType::BATS);
			area = level->GetSweptAreaX(box);
			enemies->Add(pos, EnemyType::BATS, area, Look::RIGHT);
		}
		else
		{
			pos.x = -40;
			pos.y = 100;

			box = enemies->GetEnemyHitBox(pos, EnemyType::BATS);
			area = level->GetSweptAreaX(box);
			enemies->Add(pos, EnemyType::BATS, area, Look::LEFT);

		}
	}

	if (player->getState() == true && enemies->GetEnemyP(player->getPos(), player->getPosY())) player->IncrScore(20);
	else if (enemies->EnemyHit(player->getPos(), player->getPosY())) player->hit();

	enemies->IsEnemyOut();

	level->Update();
	player->Update();
	CheckCollisions();

	box = player->GetHitbox();
	enemies->Update(box);
	shots->Update(box);
}
void Scene::Render()
{
	BeginMode2D(camera);
	
    level->Render(numStage);
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects(); 
		player->Draw();
		enemies->Draw();
		shots->Draw();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		enemies->DrawDebug();
		player->DrawDebug(GREEN);
		shots->DrawDebug(GRAY);
	}
	
	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
    level->Release();
	player->Release();
	ClearLevel();
}
void Scene::CheckCollisions()
{
	AABB player_box, obj_box;
	
	player_box = player->GetHitbox();
	auto it = objects.begin();
	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();
		if(player_box.TestAABB(obj_box))
		{
			int point = (*it)->Points();
			player->IncrScore(point);
			
			if (point == NUM_WING) player->IncrJump();
			else if (point == NUM_BOOT) player->IncrSpeed();
			//Delete the object
			delete* it; 
			//Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it); 
		}
		else
		{
			//Move to the next object
			++it; 
		}
	}
}
void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	enemies->Release();
	shots->Clear();
}
void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
}
void Scene::RenderGUI() const
{
	DrawText(TextFormat("%d", player->GetScore()), 60, 4, 8, LIGHTGRAY);
	DrawText(TextFormat("%d", numStage), 160, 4, 8, LIGHTGRAY);
	DrawText(TextFormat("%d", 3), 200, 4, 8, LIGHTGRAY); //TEMPORAL HASTA QUE TENGAMOS VIDAS
	DrawText(TextFormat("%d", 1), 230, 4, 8, LIGHTGRAY); //TEMPORAL HASTA QUE TENGAMOS VIDAS
	DrawRectangle(61, 18, player->GetLife(), 4, RED);
	DrawRectangle(61, 27, 64, 4, ORANGE);
}

bool Scene::IsPlayerDead()
{
	return player->getCondition();
}