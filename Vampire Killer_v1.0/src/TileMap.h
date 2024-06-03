#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>

enum class Tile {

	// -1: area covered by an entity
	EMPTY = -1,


	WALL = 0, BROKEN_WALL_1, BROKEN_WALL_2, BROKEN_WALL_3, BROKEN_WALL_4, BROKEN_WALL_5, BROKEN_WALL_6, BROKEN_WALL_7, BROKEN_WALL_8, BROKEN_WALL_9, BROKEN_WALL_10,
	BLOCK_SQUARE_L, BLOCK_SQUARE_R, BLOCK_SQUARE_M, GROUND,
	
	WINDOW = 15,
	COLUMN_1 = 20, COLUMN_2, COLUMN_3, COLUMN_4, COLUMN_5, COLUMN_6, BROKEN_COLUMN_1, BROKEN_COLUMN_2,
	
	BIG_CURTAIN_1 = 30, BIG_CURTAIN_2, BIG_CURTAIN_3, BIG_CURTAIN_4, BIG_CURTAIN_5, BIG_CURTAIN_6, BIG_CURTAIN_7,
	SMALL_CURTAIN_1, SMALL_CURTAIN_2, SMALL_CURTAIN_3, SMALL_CURTAIN_4,

	FINAL_DOOR_1 = 50, FINAL_DOOR_2, FINAL_DOOR_3, FINAL_DOOR_4, FINAL_DOOR_5, FINAL_DOOR_6,

	CANDLE = 60, CANDLE_FRAME_1, CANDLE_FRAME_2, FIRE,  FIRE_FRAME_1, FIRE_FRAME_2,

	SMALL_KEY = 70, BIG_KEY,

	LADDER_L = 80, LADDER_R, WALL_LADDER_L, WALL_LADDER_R, LADDER_TOP_L, LADDER_TOP_R, START_LADDER_L , START_LADDER_R,

	BOOTS = 90, WING,

	FENCE_0 = 100, FENCE_1, FENCE_2,FENCE_3, FENCE_4, BLACK_BLOCK,



	STATUE_0 = 120, STATUE_1, STATUE_2, STATUE_3, STATUE_4, STATUE_5,
	FOREST_0 = 130, FOREST_1, FOREST_2, FOREST_3, FOREST_4, FOREST_5, FOREST_6, FOREST_7, FOREST_8, FOREST_9, FOREST_10,
	FOREST_11, FOREST_12, FOREST_13, FOREST_14, FOREST_15, FOREST_16, FOREST_17, FOREST_18, FOREST_19, 
	FOREST_2_0,FOREST_2_1, FOREST_2_2, FOREST_2_3, FOREST_2_4, FOREST_2_5, 
	FOREST_3_0 = 160, FOREST_3_1, FOREST_3_2, FOREST_3_3, FOREST_3_4, FOREST_3_5, FOREST_3_6,

	

	CASTLE_0 = 180,CASTLE_1 , CASTLE_2, CASTLE_3, CASTLE_4, CASTLE_5, CASTLE_6, CASTLE_7, CASTLE_8, CASTLE_9, CASTLE_10,
	CASTLE_11, CASTLE_12, CASTLE_13, CASTLE_14, CASTLE_15, CASTLE_16, CASTLE_17, CASTLE_18, CASTLE_19, 
	CASTLE_21 = 201, CASTLE_22, CASTLE_23, CASTLE_24, CASTLE_25, CASTLE_26, CASTLE_27, CASTLE_28,

	// id >= 300: entities' initial locations
	PLAYER = 300,
	//LASER=301,
	SLIME = 301,TURRET_LEFT, TURRET_RIGHT,

	//Intervals
	STATIC_FIRST = BLOCK_SQUARE_L,
	STATIC_LAST = SMALL_CURTAIN_4,
	SOLID_FIRST = BLOCK_SQUARE_L,
	SOLID_LAST = GROUND,
	SPECIAL_FIRST = CANDLE,
	SPECIAL_LAST = FIRE_FRAME_2,
	ENTITY_FIRST = PLAYER,
	ENTITY_LAST = TURRET_RIGHT,
	OBJECT_FIRST = BOOTS,
	OBJECT_LAST = WING
};

class TileMap
{
public:
	TileMap();
	~TileMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void ClearObjectEntityPositions();

	void Update();
	void Render(int numStage);
	void Release();

	bool IsTileObject(Tile tile) const;
	bool IsTileEntity(Tile tile) const;

	//Test for collisions with walls
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;
	
	//Test collision with the ground and update 'py' with the maximum y-position to prevent
	//penetration of the grounded tile, that is, the pixel y-position above the grounded tile.
	//Grounded tile = solid tile (blocks) or ladder tops.
	bool TestCollisionGround(const AABB& box, int *py) const;
	
	//Test if there is a ground tile one pixel below the given box
	bool TestFalling(const AABB& box) const;
	
	//Test if box is on ladder and update 'px' with the x-center position of the ladder
	bool TestOnLadder(const AABB& box, int* px) const;
	
	//Test if box is on ladder top and update 'px' with the x-center position of the ladder
	bool TestOnLadderTop(const AABB& box, int* px) const;

	int TestOnLadderTopDown(const AABB& box, int* px, int* py) const;

	int testOnLadder(const AABB& box, int* px) const;

	//Given a hitbox, computes the maximum swept box model along the X-axis without solid tiles
	AABB GetSweptAreaX(const AABB& hitboxbox) const;

private:
	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	bool IsTileSolid(Tile tile) const;
	bool IsTileLadderTop(Tile tile) const;
	bool IsTileLadder(Tile tile) const;

	//cambio
	bool IsTileLadderLeft(Tile tile) const;
	bool IsTileLadderRight(Tile tile) const;

	bool IsTileLadderDown(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;
	int GetLadderCenterPos(int pixel_x, int pixel_y) const;

	//Tile map
	Tile *map;

	//Size of the tile map
	int size, width, height;
	
	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	Sprite *candle;
	Sprite* fire;
	
	//Tile sheet
	const Texture2D *img_tiles;
	const Texture2D* img_tiles_0;
};

