#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>

TileMap::TileMap()
{
	map = nullptr;
	width = 0;
	height = 0;
	candle = nullptr;
	fire = nullptr;

	img_tiles = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (map != nullptr)
	{
		delete[] map;
		map = nullptr;
	}
	if (candle != nullptr)
	{
		candle->Release();
		delete candle;
		candle = nullptr;
	}
	if (fire != nullptr)
	{
		fire->Release();
		delete fire;
		fire = nullptr;
	}
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;

	dict_rect[(int)Tile::BLOCK_SQUARE_L] = { 0,  0, n, n };
	dict_rect[(int)Tile::BLOCK_SQUARE_R] = { n,  0, n, n };
	dict_rect[(int)Tile::BLOCK_SQUARE_M] = { 0, n, n, n };

	dict_rect[(int)Tile::COLUMN_1] = { 2 * n,  0, n, n };
	dict_rect[(int)Tile::COLUMN_2] = { 3 * n,  0, n, n };
	dict_rect[(int)Tile::COLUMN_3] = { 2 * n, n, n, n };
	dict_rect[(int)Tile::COLUMN_4] = { 3 * n, n, n, n };
	dict_rect[(int)Tile::COLUMN_5] = { 2 * n, 2*n, n, n };
	dict_rect[(int)Tile::COLUMN_6] = { 3 * n, 2*n, n, n };

	dict_rect[(int)Tile::FINAL_DOOR_1] = { 2 * n, 3 * n, n, n };
	dict_rect[(int)Tile::FINAL_DOOR_2] = { 3 * n, 3 * n, n, n };
	dict_rect[(int)Tile::FINAL_DOOR_3] = { 2 * n, 4 * n, n, n };
	dict_rect[(int)Tile::FINAL_DOOR_4] = { 3 * n, 4 * n, n, n };
	dict_rect[(int)Tile::FINAL_DOOR_5] = { 2 * n, 5 * n, n, n };
	dict_rect[(int)Tile::FINAL_DOOR_6] = { 3 * n, 5 * n, n, n };

	dict_rect[(int)Tile::WINDOW] = { 4 * n, 0, n, n };
	dict_rect[(int)Tile::BROKEN_COLUMN_1] = { 4 * n, n, n, n };
	dict_rect[(int)Tile::BROKEN_COLUMN_2] = { 4 * n, 2*n, n, n };


	dict_rect[(int)Tile::WALL] = { 5 * n, 0, n, n };
	dict_rect[(int)Tile::BROKEN_WALL_1] = { 5 * n, n, n, n };
	dict_rect[(int)Tile::BROKEN_WALL_2] = { 5 * n, 2*n, n, n };
	dict_rect[(int)Tile::BROKEN_WALL_3] = { 6 * n, 0, n, n };
	dict_rect[(int)Tile::BROKEN_WALL_4] = { 7 * n, 0, n, n };
	dict_rect[(int)Tile::BROKEN_WALL_5] = { 6 * n, n, n, n };
	dict_rect[(int)Tile::BROKEN_WALL_6] = { 7 * n, n, n, n };
	dict_rect[(int)Tile::BROKEN_WALL_7] = { 6 * n, 2*n, n, n };
	dict_rect[(int)Tile::BROKEN_WALL_8] = { 7 * n, 2*n, n, n };
	dict_rect[(int)Tile::BROKEN_WALL_9] = { 6 * n, 3 * n, n, n };
	dict_rect[(int)Tile::BROKEN_WALL_10] = { 7 * n, 3 * n, n, n };

	dict_rect[(int)Tile::BIG_CURTAIN_1] = { 8 * n, 0, n, n };
	dict_rect[(int)Tile::BIG_CURTAIN_2] = { 9 * n, 0, n, n };
	dict_rect[(int)Tile::BIG_CURTAIN_3] = { 8 * n, n, n, n };
	dict_rect[(int)Tile::BIG_CURTAIN_4] = { 9 * n, n, n, n };
	dict_rect[(int)Tile::BIG_CURTAIN_5] = { 8 * n, 2*n, n, n };
	dict_rect[(int)Tile::BIG_CURTAIN_6] = { 9 * n, 2*n, n, n };
	dict_rect[(int)Tile::BIG_CURTAIN_7] = { 8 * n, 3*n, n, n };

	dict_rect[(int)Tile::SMALL_CURTAIN_1] = { 10 * n, 0, n, n };
	dict_rect[(int)Tile::SMALL_CURTAIN_2] = { 10 * n, n, n, n };
	dict_rect[(int)Tile::SMALL_CURTAIN_3] = { 10 * n, 2*n, n, n };
	dict_rect[(int)Tile::SMALL_CURTAIN_4] = { 10 * n, 3*n, n, n };

	dict_rect[(int)Tile::LADDER_L] = { 0, 3 * n, n, n };
	dict_rect[(int)Tile::LADDER_R] = { n, 3 * n, n, n };
	dict_rect[(int)Tile::START_LADDER_L] = { 0, 3 * n, n, n };
	dict_rect[(int)Tile::START_LADDER_R] = { n, 3 * n, n, n };
	dict_rect[(int)Tile::LADDER_TOP_L] = { 0, 3 * n, n, n };
	dict_rect[(int)Tile::LADDER_TOP_R] = { n, 3 * n, n, n };

	dict_rect[(int)Tile::WALL_LADDER_L] = { 0, 4 * n, n, n };
	dict_rect[(int)Tile::WALL_LADDER_R] = { n, 4 * n, n, n };

	dict_rect[(int)Tile::CANDLE_FRAME_1] = { 4*n, 3 * n, n, n };
	dict_rect[(int)Tile::CANDLE_FRAME_2] = { 5*n, 3 * n, n, n };

	dict_rect[(int)Tile::BOOTS] = { 4 * n, 4 * n, n, n };
	dict_rect[(int)Tile::WING] = { 5 * n, 4 * n, n, n };


	//Stage 0
	dict_rect[(int)Tile::GROUND] = { 0,  0, n, n };
	dict_rect[(int)Tile::FENCE_0] = { 0,  n, n, n };
	dict_rect[(int)Tile::FENCE_1] = { 0,  2*n, n, n };
	dict_rect[(int)Tile::FENCE_2] = { 0,  3 * n, n, n };
	dict_rect[(int)Tile::FENCE_3] = { n,  3 * n, n, n };
	dict_rect[(int)Tile::FENCE_4] = { 2*n,  3 * n, n, n };

	dict_rect[(int)Tile::BLACK_BLOCK] = { n,  4 * n, n, n };

	dict_rect[(int)Tile::STATUE_0] = { n,  0, n, n };
	dict_rect[(int)Tile::STATUE_1] = { 2*n,  0, n, n };
	dict_rect[(int)Tile::STATUE_2] = { n,  n, n, n };
	dict_rect[(int)Tile::STATUE_3] = { 2 * n,  n, n, n };
	dict_rect[(int)Tile::STATUE_4] = { n,  2*n, n, n };
	dict_rect[(int)Tile::STATUE_5] = { 2*n,  2 * n, n, n };

	dict_rect[(int)Tile::FOREST_0] = { 3*n,  0, n, n };
	dict_rect[(int)Tile::FOREST_1] = { 4 * n,  0, n, n };
	dict_rect[(int)Tile::FOREST_2] = { 5 * n,  0, n, n };
	dict_rect[(int)Tile::FOREST_3] = { 6 * n,  0, n, n };

	dict_rect[(int)Tile::FOREST_4] = { 3 * n,  n, n, n };
	dict_rect[(int)Tile::FOREST_5] = { 4 * n,  n, n, n };
	dict_rect[(int)Tile::FOREST_6] = { 5 * n,  n, n, n };
	dict_rect[(int)Tile::FOREST_7] = { 6 * n,  n, n, n };

	dict_rect[(int)Tile::FOREST_8] = { 3 * n,  2*n, n, n };
	dict_rect[(int)Tile::FOREST_9] = { 4 * n,  2*n, n, n };
	dict_rect[(int)Tile::FOREST_10] = { 5 * n,  2*n, n, n };
	dict_rect[(int)Tile::FOREST_11] = { 6 * n,  2*n, n, n };

	dict_rect[(int)Tile::FOREST_12] = { 3 * n,  3*n, n, n };
	dict_rect[(int)Tile::FOREST_13] = { 4 * n, 3* n, n, n };
	dict_rect[(int)Tile::FOREST_14] = { 5 * n, 3* n, n, n };
	dict_rect[(int)Tile::FOREST_15] = { 6 * n, 3* n, n, n };

	dict_rect[(int)Tile::FOREST_16] = { 3 * n,  4 * n, n, n };
	dict_rect[(int)Tile::FOREST_17] = { 4 * n, 4 * n, n, n };
	dict_rect[(int)Tile::FOREST_18] = { 5 * n, 4 * n, n, n };
	dict_rect[(int)Tile::FOREST_19] = { 6 * n, 4 * n, n, n };

	dict_rect[(int)Tile::FOREST_2_0] = { 7*n,  0, n, n };
	dict_rect[(int)Tile::FOREST_2_1] = { 8 * n,  0, n, n };
	dict_rect[(int)Tile::FOREST_2_2] = { 9 * n,  0, n, n };
	dict_rect[(int)Tile::FOREST_2_3] = { 10 * n,  0, n, n };
	dict_rect[(int)Tile::FOREST_2_4] = { 7 * n,  n, n, n };
	dict_rect[(int)Tile::FOREST_2_5] = { 8* n,  n, n, n };

	dict_rect[(int)Tile::FOREST_3_0] = { 7 * n,  2 * n, n, n };
	dict_rect[(int)Tile::FOREST_3_1] = { 8 * n,  2 * n, n, n };
	dict_rect[(int)Tile::FOREST_3_2] = { 9 * n,  2 * n, n, n };
	dict_rect[(int)Tile::FOREST_3_3] = { 10 * n,  2 * n, n, n };
	dict_rect[(int)Tile::FOREST_3_4] = { 7 * n,  3*n, n, n };
	dict_rect[(int)Tile::FOREST_3_5] = { 8 * n, 3* n, n, n };
	dict_rect[(int)Tile::FOREST_3_6] = { 9 * n, 3* n, n, n };

	dict_rect[(int)Tile::FIRE_FRAME_1] = { 7 * n,  4 * n, n, n };
	dict_rect[(int)Tile::FIRE_FRAME_2] = { 8 * n, 4 * n, n, n };

	dict_rect[(int)Tile::CASTLE_0] = { 11 * n,  0, n, n };
	dict_rect[(int)Tile::CASTLE_1] = { 12 * n,  0, n, n };
	dict_rect[(int)Tile::CASTLE_2] = { 13 * n,  0, n, n };

	dict_rect[(int)Tile::CASTLE_3] = { 11 * n,  n, n, n };
	dict_rect[(int)Tile::CASTLE_4] = { 12 * n,  n, n, n };
	dict_rect[(int)Tile::CASTLE_5] = { 13 * n,  n, n, n };

	dict_rect[(int)Tile::CASTLE_6] = { 15 * n,  n, n, n };
	dict_rect[(int)Tile::CASTLE_7] = { 16 * n,  n, n, n };

	dict_rect[(int)Tile::CASTLE_8] = { 11 * n, 2* n, n, n };
	dict_rect[(int)Tile::CASTLE_9] = { 12 * n,  2*n, n, n };
	dict_rect[(int)Tile::CASTLE_10] = { 13 * n,  2*n, n, n };
	dict_rect[(int)Tile::CASTLE_11] = { 14 * n,  2 * n, n, n };
	dict_rect[(int)Tile::CASTLE_12] = { 15 * n,  2 * n, n, n };
	dict_rect[(int)Tile::CASTLE_13] = { 16 * n,  2 * n, n, n };

	dict_rect[(int)Tile::CASTLE_14] = { 11 * n, 3 * n, n, n };
	dict_rect[(int)Tile::CASTLE_15] = { 12 * n,  3 * n, n, n };
	dict_rect[(int)Tile::CASTLE_16] = { 13 * n,  3 * n, n, n };
	dict_rect[(int)Tile::CASTLE_17] = { 14 * n,  3 * n, n, n };
	dict_rect[(int)Tile::CASTLE_18] = { 15 * n,  3 * n, n, n };
	dict_rect[(int)Tile::CASTLE_19] = { 16 * n,  3 * n, n, n };

	dict_rect[(int)Tile::CASTLE_21] = { 12 * n,  4 * n, n, n };
	dict_rect[(int)Tile::CASTLE_22] = { 13 * n,  4 * n, n, n };

	dict_rect[(int)Tile::CASTLE_23] = { 16 * n,  4 * n, n, n };

	dict_rect[(int)Tile::CASTLE_24] = { 11 * n, 5 * n, n, n };
	dict_rect[(int)Tile::CASTLE_25] = { 12 * n,  5 * n, n, n };
	dict_rect[(int)Tile::CASTLE_26] = { 13 * n,  5 * n, n, n };

	dict_rect[(int)Tile::CASTLE_27] = { 15 * n,  5 * n, n, n };
	dict_rect[(int)Tile::CASTLE_28] = { 16 * n,  5 * n, n, n };
}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES_0, "images/TILESET BG STAGE.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles_0 = data.GetTexture(Resource::IMG_TILES_0);

	if (data.LoadTexture(Resource::IMG_TILES, "images/TILESET BG STAGE 01 FIXED + ITEMS.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);

	candle = new Sprite(img_tiles);
	if (candle == nullptr)
	{
		LOG("Failed to allocate memory for laser sprite");
		return AppStatus::ERROR;
	}
	candle->SetNumberAnimations(1);
	candle->SetAnimationDelay(0, ANIM_DELAY_EXTENDED);
	candle->AddKeyFrame(0, dict_rect[(int)Tile::CANDLE_FRAME_1]);
	candle->AddKeyFrame(0, dict_rect[(int)Tile::CANDLE_FRAME_2]);
	candle->SetAnimation(0);

	fire = new Sprite(img_tiles_0);
	if (fire == nullptr)
	{
		LOG("Failed to allocate memory for laser sprite");
		return AppStatus::ERROR;
	}
	fire->SetNumberAnimations(1);
	fire->SetAnimationDelay(0, ANIM_DELAY_EXTENDED );
	fire->AddKeyFrame(0, dict_rect[(int)Tile::FIRE_FRAME_1]);
	fire->AddKeyFrame(0, dict_rect[(int)Tile::FIRE_FRAME_2]);
	fire->SetAnimation(0);

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w*h;
	width = w;
	height = h;

	if (map != nullptr)	delete[] map;

	map = new Tile[size];
	if (map == nullptr)	
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(map, data, size * sizeof(int));

	return AppStatus::OK;
}
void TileMap::ClearObjectEntityPositions()
{
	int i;
	Tile tile;
	for (i = 0; i < size; ++i)
	{
		tile = map[i];
		if (IsTileEntity(tile) || IsTileObject(tile) || tile == Tile::EMPTY)
			map[i] = Tile::WALL;
	}
}
bool TileMap::IsTileEntity(Tile tile) const
{
	return Tile::ENTITY_FIRST <= tile && tile <= Tile::ENTITY_LAST;
}
bool TileMap::IsTileObject(Tile tile) const
{
	return Tile::OBJECT_FIRST <= tile && tile <= Tile::OBJECT_LAST;
}
void TileMap::Update()
{
	candle->Update();
	fire->Update();

}
Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y*width;
	if(idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
		return Tile::WALL;
	}
	return map[x + y * width];
}
bool TileMap::IsTileSolid(Tile tile) const
{
	return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST);
}
bool TileMap::IsTileLadderTop(Tile tile) const
{
	return tile == Tile::LADDER_R || tile == Tile::LADDER_TOP_R || tile == Tile::LADDER_L || tile == Tile::LADDER_TOP_L;
}
bool TileMap::IsTileLadder(Tile tile) const
{
	return tile == Tile::LADDER_L || tile == Tile::LADDER_R ;
}

//cambio
bool TileMap::IsTileLadderLeft(Tile tile) const
{
	return tile == Tile::START_LADDER_L;
}
bool TileMap::IsTileLadderRight(Tile tile) const
{
	return tile == Tile::START_LADDER_R;
}


bool TileMap::IsTileLadderDown(Tile tile) const
{
	return tile == Tile::LADDER_TOP_L || tile == Tile::LADDER_TOP_R;
}
bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionGround(const AABB& box, int *py) const
{
	Point p(box.pos.x, *py);	//control point
	int tile_y;

	if (CollisionY(p, box.width))
	{
		tile_y = p.y / TILE_SIZE;

		*py = tile_y * TILE_SIZE - 1;
		return true;
	}
	return false;
}
bool TileMap::TestFalling(const AABB& box) const
{
	return !CollisionY(box.pos + Point(0, box.height), box.width);
}
bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;
	
	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileSolid(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile))
			return true;
	}
	return false;
}
int TileMap::TestOnLadderTopDown(const AABB& box, int* px, int* py) const
{
	int left,right, bottom;
	int tx1,tx2, ty;
	Tile tile1, tile2;

	//Control points
	left = box.pos.x;
	right = box.pos.x + box.width - 1;
	bottom = box.pos.y + box.height - 1;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	
	ty = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile1 = GetTileIndex(tx1, ty);
	tile2 = GetTileIndex(tx2, ty);

	if (IsTileLadderDown(tile1))
	{
		*px = GetLadderCenterPos(left, bottom);
		*px -= 6;
		*py += 6;
		return 0;
	}
	else if (IsTileLadderDown(tile2)) {
 		*px = GetLadderCenterPos(right, bottom);
		*px -= 6;
		*py += 10;
		return 1;
	}
	return 2;
}

bool TileMap::TestOnLadder(const AABB& box, int* px) const
{
	int left, right, bottom;
	int tx1, tx2, ty;
	Tile tile1, tile2;

	//Control points
	left = box.pos.x;
	right = box.pos.x + box.width-1;
	bottom = box.pos.y + box.height-1;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile1 = GetTileIndex(tx1, ty);

	if (IsTileLadderRight(tile1) )
	{
		*px = GetLadderCenterPos(left, bottom) - box.width/2;
		return true;
	}
	return false;
}

int TileMap::testOnLadder(const AABB& box, int* px) const
{
	int left, right, bottom;
	int tx1, tx2, ty;
	Tile tile1, tile2;

	//Control points
	left = box.pos.x + box.width - 1;
	right = box.pos.x + box.width - 5;
	bottom = box.pos.y + box.height - 1;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	
	ty = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile1 = GetTileIndex(tx1, ty);

	if (IsTileLadderLeft(tile1))
	{
		*px = GetLadderCenterPos(right, bottom) - box.width / 2;

		return 0;
	}
	else if (IsTileLadderRight(tile1))
	{
		*px = GetLadderCenterPos(left, bottom) - box.width / 2;
		return 1;
	}
	return 2;
}

bool TileMap::TestOnLadderTop(const AABB& box, int* px) const
{
	int left, right, bottom;
	int tx1, tx2, ty;
	Tile tile1, tile2;

	//Control points
	left = box.pos.x;
	right = box.pos.x + box.width - 1;
	bottom = box.pos.y + box.height - 1;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile1 = GetTileIndex(tx1, ty-1);
	if (IsTileLadderTop(tile1))
	{
		return false;
	}
	return true;
}
int TileMap::GetLadderCenterPos(int pixel_x, int pixel_y) const
{
	int tx, ty;
	
	tx = pixel_x / TILE_SIZE;
	ty = pixel_y / TILE_SIZE;
	Tile tile = GetTileIndex(tx, ty);

	if (tile == Tile::START_LADDER_L || tile == Tile::LADDER_L || tile == Tile::LADDER_TOP_L)		return tx * TILE_SIZE + TILE_SIZE;
	else if (tile == Tile::START_LADDER_R || tile == Tile::LADDER_R || tile == Tile::LADDER_TOP_R)	return tx * TILE_SIZE;
	else
	{
		LOG("Internal error, tile should be a LADDER, coord: (%d,%d), tile type: %d", pixel_x, pixel_y, (int)tile);
		return 0;
	}
}
AABB TileMap::GetSweptAreaX(const AABB& hitbox) const
{
	AABB box;
	int column, x, y, y0, y1;
	bool collision;

	box.pos.y = hitbox.pos.y;
	box.height = hitbox.height;

	column = hitbox.pos.x / TILE_SIZE;
	y0 = hitbox.pos.y / TILE_SIZE;
	y1 = (hitbox.pos.y + hitbox.height - 1) / TILE_SIZE;

	//Compute left tile index
	collision = false;
	x = column - 1;
	while (!collision && x >= 0)
	{
		//Iterate over the tiles within the vertical range
		for (y = y0; y <= y1; ++y)
		{
			//One solid tile is sufficient
			if (IsTileSolid(GetTileIndex(x, y)))
			{
				collision = true;
				break;
			}
		}
		if (!collision) x--;
	}
	box.pos.x = (x + 1) * TILE_SIZE;

	//Compute right tile index
	collision = false;
	x = column + 1;
	while (!collision && x < LEVEL_WIDTH)
	{
		//Iterate over the tiles within the vertical range
		for (y = y0; y <= y1; ++y)
		{
			//One solid tile is sufficient
			if (IsTileSolid(GetTileIndex(x, y)))
			{
				collision = true;
				break;
			}
		}
		if (!collision) x++;
	}
	box.width = x * TILE_SIZE - box.pos.x;

	return box;
}
void TileMap::Render(int numStage)
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;
	if (numStage > 3)
	{
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				tile = map[i * width + j];

				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

				
				if (tile != Tile::CANDLE && tile != Tile::FIRE)
				{
					rc = dict_rect[(int)tile];
					DrawTextureRec(*img_tiles, rc, pos, WHITE);
				}
				else if(tile == Tile::CANDLE)
				{
					candle->Draw((int)pos.x, (int)pos.y);
				}
				else
				{
					fire->Draw((int)pos.x, (int)pos.y);
				}

			}
		}
	}
	else 
	{
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				tile = map[i * width + j];

				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

				

				if (tile != Tile::CANDLE && tile != Tile::FIRE)
				{
					rc = dict_rect[(int)tile];
					DrawTextureRec(*img_tiles_0, rc, pos, WHITE);
				}
				else if (tile == Tile::CANDLE)
				{
					candle->Draw((int)pos.x, (int)pos.y);
				}
				else
				{
					fire->Draw((int)pos.x, (int)pos.y);
				}

			}
		}
	}
}
void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance(); 
	data.ReleaseTexture(Resource::IMG_TILES);

	candle->Release();
	fire->Release();

	dict_rect.clear();
}