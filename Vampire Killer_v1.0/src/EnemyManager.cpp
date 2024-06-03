#include "EnemyManager.h"
#include "Skeleton.h"
#include "Panther.h"
#include "Bat.h"

EnemyManager::EnemyManager()
{
	shots = nullptr;
}
EnemyManager::~EnemyManager()
{
	Release();
}
AppStatus EnemyManager::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ENEMIES, "images/Enemies1.png") != AppStatus::OK)
	{
		LOG("Failed to load enemies sprite texture");
		return AppStatus::ERROR;
	}
	ResourceManager& data2 = ResourceManager::Instance();
	if (data2.LoadTexture(Resource::IMG_BATS, "images/Bats.png") != AppStatus::OK)
	{
		LOG("Failed to load enemies sprite texture");
		return AppStatus::ERROR;
	}

	return AppStatus::OK;
}
void EnemyManager::SetShotManager(ShotManager* shots)
{
	this->shots = shots;
}

void EnemyManager::Add(const Point& pos, EnemyType type, const AABB& area, Look look)
{
	Enemy *enemy;
	if(type == EnemyType::SKELETON)
	{
		enemy = new Skeleton(pos, SLIME_PHYSICAL_WIDTH, SLIME_PHYSICAL_HEIGHT, SLIME_FRAME_SIZE, SLIME_FRAME_SIZE);
	}
	else if(type == EnemyType::TURRET)
	{
		enemy = new Panther(pos, TURRET_PHYSICAL_WIDTH, TURRET_PHYSICAL_HEIGHT, TURRET_FRAME_SIZE, TURRET_FRAME_SIZE);
	}
	else if (type == EnemyType::BATS)
	{
		enemy = new Bat(pos, 16, 16, 16, 16);
	}
	else
	{
		LOG("Internal error: trying to add a new enemy with invalid type");
		return;
	}
		
	enemy->Initialise(look, area);
	enemies.push_back(enemy);

}

void  EnemyManager::IsEnemyOut()
{
	Point p, d;
	for (Enemy* enemy : enemies)
	{
		enemy->GetShootingPosDir(&p, &d);;
		if (p.x<= -20||p.x>=280)
		{
				delete enemy;
				enemies.clear();
		}
	}
}
bool EnemyManager::GetEnemyP(int PlayerX, int PlayerY)
{
	Point p, d;
	for (Enemy* enemy : enemies)
	{
		enemy->GetShootingPosDir(&p, &d);;
		if(p.y <= PlayerY && p.y >= PlayerY-64)
		{ 
			if (p.x <= PlayerX+25 && p.x >= PlayerX-20) {
				delete enemy;
				enemies.clear();
				return true;
			}
		}
	}
	return false;
}
bool EnemyManager::EnemyHit(int PlayerX, int PlayerY)
{
	Point p, d;
	for (Enemy* enemy : enemies) 
	{
		enemy->GetShootingPosDir(&p, &d);;
 		if (p.y <= PlayerY && p.y >= PlayerY - 64)
		{
			if (p.x <= PlayerX && p.x >= PlayerX - 20) {
				return true;
			}
		}

	}
	return false;
}
AABB EnemyManager::GetEnemyHitBox(const Point& pos, EnemyType type) const
{
	int width, height;
	if (type == EnemyType::SKELETON)
	{
		width = SLIME_PHYSICAL_WIDTH;
		height = SLIME_PHYSICAL_HEIGHT;
	}
	else if (type == EnemyType::TURRET)
	{
		width = TURRET_PHYSICAL_WIDTH;
		height = TURRET_PHYSICAL_HEIGHT;
	}
	else
	{
		LOG("Internal error while computing hitbox for an invalid enemy type");
		return {};
	}
	Point p(pos.x, pos.y - (height - 1));
	AABB hitbox(p, width, height);
	return hitbox;
}
void EnemyManager::Update(const AABB& player_hitbox)
{
	bool shoot;
	Point p, d;

	for (Enemy* enemy : enemies)
	{
		shoot = enemy->Update(player_hitbox);
		if (shoot)
		{
			enemy->GetShootingPosDir(&p, &d);
			shots->Add(p, d);
		}
	}
}
void EnemyManager::Draw() const
{
	for (const Enemy* enemy : enemies)
		enemy->Draw();
}
void EnemyManager::DrawDebug() const
{
	for (const Enemy* enemy : enemies)
	{	
		enemy->DrawVisibilityArea(DARKGRAY);
		enemy->DrawHitbox(RED);
	}
}
void EnemyManager::Release()
{
	for (Enemy* enemy : enemies)
		delete enemy;
	enemies.clear();
}