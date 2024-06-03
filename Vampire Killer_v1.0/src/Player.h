#pragma once
#include "Entity.h"
#include "TileMap.h"
#include "Sprite.h"

//Representation model size: 32x32
#define PLAYER_FRAME_SIZE		32
#define PLAYER_FRAME_BIG_SIZE		64

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	12
#define PLAYER_PHYSICAL_HEIGHT	28

//Horizontal speed and vertical speed while falling down
//#define PLAYER_SPEED			1 //MEJORA VELOCIDAD

//Vertical speed while on a ladder
#define PLAYER_LADDER_SPEED		1

//Frame animation delay while on a ladder
#define ANIM_LADDER_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
//#define PLAYER_JUMP_FORCE		8 //MEJORA SALTO

//Frame delay for updating the jump velocity
#define PLAYER_JUMP_DELAY		2

//Player is levitating when abs(speed) <= this value
#define PLAYER_LEVITATING_SPEED	4

//Gravity affects jumping velocity when jump_delay is 0
#define GRAVITY_FORCE			1


//Logic states
enum class State { IDLE, WALKING, JUMPING, FALLING, CLIMBING,CROUCH , ATTACK, HIT, DEAD };

//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT,
	WALKING_LEFT, WALKING_RIGHT,
	JUMPING_LEFT, JUMPING_RIGHT,
	LEVITATING_LEFT, LEVITATING_RIGHT,
	FALLING_LEFT, FALLING_RIGHT,
	CLIMBING, CLIMBING_PRE_TOP, CLIMBING_TOP,
	CLIMBING_LEFT, CLIMBING_DOWN_R, CLIMBING_DOWN_L,
	SHOCK_LEFT, SHOCK_RIGHT,
	TELEPORT_LEFT, TELEPORT_RIGHT,
	CROUCH_LEFT, CROUCH_RIGHT,
	ATTACK_LEFT, ATTACK_RIGHT,
	CROUCH_ATTACK_LEFT, CROUCH_ATTACK_RIGHT,
	WIN, LOSE,DEAD,HIT,HIT_LEFT,
	NUM_ANIMATIONS,
};

class Player: public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();
	
	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);

	void IncrSpeed();
	void IncrJump();


	int GetScore();
	int GetLife();
	Look getLook();

	void hit();

	bool getCondition();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();
	 
	bool getState();
	int getPos();
	int getPosY();
	void changeLook(int dir);

	int PLAYER_SPEED = 1;
	int PLAYER_JUMP_FORCE = 8;

private:
	bool noColision;
	bool IsLookingRight() const;
	bool IsLookingLeft() const;

	//Player mechanics
	void MoveX();
	void MoveY();
	void LogicJumping();
	void LogicClimbing();
	void LogicAttacking();

	void LogicHit();
	void StartHit();

	void StartLose();
	void Lose();

	//Animation management
	void SetAnimation(int id);
	PlayerAnim GetAnimation();

	void Stop();

	void StartWalkingLeft();
	void StartWalkingRight();
	void StartAttacking();
	void StartCrouchAttacking();

	void CrouchRight();
	void CrouchLeft();

	void StartFalling();
	void StartJumping();
	void StartClimbingUp();
	void StartClimbingDown();

	void ChangeAnimRight();
	void ChangeAnimLeft();

	//Jump steps
	bool IsAscending() const;
	bool IsLevitating() const;
	bool IsDescending() const;

	//Ladder get in/out steps
	bool IsInFirstHalfTile() const;
	bool IsInSecondHalfTile() const;

	int cnt;
	int cnt2;

	void updateColdDown();

	int HitColdDown;
	bool hit1;

	bool posChange;
	bool climbing;

	State state;
	Look look;
	int jump_delay;

	Sprite* sprite;

	TileMap *map;

	int SINGLE_ANIMATION_0 = 0;
	int SINGLE_ANIMATION_1 = 0;

	bool movingUp = false;
	bool movingUpL = false;
	bool movingDown = false;
	bool movingDownL = false;

	bool gethit = false;

	bool ActualDead = false;

	bool attacking = false;

	int playerLife;

	bool TopLadder;

	int score;

	Music sfx;
};

