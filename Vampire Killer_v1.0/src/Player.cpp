#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Player::Player(const Point& p, State s, Look view) :
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	state = s;
	look = view;
	jump_delay = PLAYER_JUMP_DELAY;
	map = nullptr;
	posChange = false;
	score = 0;
	playerLife = 64;
}
Player::~Player()
{

}
AppStatus Player::Initialise()
{
	int i;
	const int n = PLAYER_FRAME_SIZE;
	const int N = PLAYER_FRAME_BIG_SIZE;

	sfx = LoadMusicStream("Vamp Killer ost/35.wav");
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_PLAYER, "images/ALL OF THEM.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	render = new Sprite(data.GetTexture(Resource::IMG_PLAYER));

	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}


	Sprite* sprite = dynamic_cast<Sprite*>(render);

	sprite->SetNumberAnimations((int)PlayerAnim::NUM_ANIMATIONS);
	
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_RIGHT, ANIM_DELAY_BASIC);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT, { 0, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_LEFT, ANIM_DELAY_BASIC);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT, { 0, 0, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_RIGHT, ANIM_DELAY_BASIC);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { (float)i*n, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_LEFT, ANIM_DELAY_BASIC);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { (float)i*n, 0, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::ATTACK_RIGHT, ANIM_DELAY_BASIC);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::ATTACK_RIGHT, { (float)i * N, 12*n,N, n });
	sprite->SetAnimationDelay((int)PlayerAnim::ATTACK_LEFT, ANIM_DELAY_BASIC);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::ATTACK_LEFT, { (float)i * N, 12 * n, -N, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CROUCH_ATTACK_RIGHT, ANIM_DELAY_BASIC);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CROUCH_ATTACK_RIGHT, { (float)i * N, 14 * n,N, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CROUCH_ATTACK_LEFT, ANIM_DELAY_BASIC);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CROUCH_ATTACK_LEFT, { (float)i * N, 14 * n, -N, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CROUCH_RIGHT, ANIM_DELAY_BASIC);
	sprite->AddKeyFrame((int)PlayerAnim::CROUCH_RIGHT, { 0, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CROUCH_LEFT, ANIM_DELAY_BASIC);
	sprite->AddKeyFrame((int)PlayerAnim::CROUCH_LEFT, { 0, n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_RIGHT, ANIM_DELAY_BASIC);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT, { 0, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_LEFT, ANIM_DELAY_BASIC);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT, { 0, n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_RIGHT, ANIM_DELAY_BASIC);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_RIGHT, { 0, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_LEFT, ANIM_DELAY_BASIC);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_LEFT, { 0, n, -n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_RIGHT, ANIM_DELAY_BASIC);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_RIGHT, { 0, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_LEFT, ANIM_DELAY_BASIC);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_LEFT, { 0, n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING, ANIM_DELAY_EXTENDED);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CLIMBING, { (float)i * n, 3 * n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_DOWN_R, ANIM_DELAY_EXTENDED);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_DOWN_R, { (float)i * n, 4 * n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_DOWN_L, ANIM_DELAY_EXTENDED);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_DOWN_L, { (float)i * n, 4 * n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::HIT, ANIM_DELAY_EXTENDED);
		sprite->AddKeyFrame((int)PlayerAnim::HIT, { 0, 6 * n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::HIT_LEFT, ANIM_DELAY_EXTENDED);
		sprite->AddKeyFrame((int)PlayerAnim::HIT_LEFT, { 0, 6 * n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::LOSE, ANIM_DELAY_EXTENDED);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::LOSE, { (float)i * n, 6 * n, n, n });

	sprite->AddKeyFrame((int)PlayerAnim::DEAD, { 2 * n, 6 * n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_PRE_TOP, ANIM_DELAY_EXTENDED);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_PRE_TOP, { 0, 3 * n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_TOP, ANIM_DELAY_EXTENDED);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_TOP, { 0, 3 * n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_LEFT, ANIM_DELAY_EXTENDED);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_LEFT, { (float)i * n, 3 * n, -n, n });
		
	sprite->SetAnimation((int)PlayerAnim::IDLE_RIGHT);

	return AppStatus::OK;
}
void Player::InitScore()
{
	score = 0;
}

Look Player::getLook()
{
	return look;
}

bool Player::getState()
{
	return attacking;
}
int Player::getPos()
{
	return pos.x;
}
int Player::getPosY()
{
	return pos.y;
}
void Player::IncrScore(int n)
{
	score += n;
}
int Player::GetScore()
{
	return score;
}
int Player::GetLife()
{
	return playerLife;
}

void Player::IncrJump()
{
	PLAYER_JUMP_FORCE++;
}
void Player::IncrSpeed()
{
	PLAYER_SPEED++;
}

void Player::hit()
{
	if(!hit1)
	{ 
		if (!gethit)playerLife -= 10;
		gethit = true;
		//hit1 = true;
	}

}

void Player::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Player::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Player::IsLookingLeft() const
{
	return look == Look::LEFT;
}
bool Player::IsAscending() const
{
	return dir.y < -PLAYER_LEVITATING_SPEED;
}
bool Player::IsLevitating() const
{
	return abs(dir.y) <= PLAYER_LEVITATING_SPEED;
}
bool Player::IsDescending() const
{
	return dir.y > PLAYER_LEVITATING_SPEED;
}
bool Player::IsInFirstHalfTile() const
{
	return pos.y % TILE_SIZE < TILE_SIZE / 2;
}
bool Player::IsInSecondHalfTile() const
{
	return pos.y % TILE_SIZE >= TILE_SIZE/2;
}
void Player::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}

PlayerAnim Player::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (PlayerAnim)sprite->GetAnimation();
}
void Player::Stop()
{
	dir = { 0,0 };
	state = State::IDLE;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::IDLE_RIGHT);
	else					SetAnimation((int)PlayerAnim::IDLE_LEFT);
}

void Player::StartLose()
{
	state = State::DEAD;
	SetAnimation((int)PlayerAnim::LOSE);
}
void Player::StartWalkingLeft()
{
	state = State::WALKING;
	look = Look::LEFT;
	SetAnimation((int)PlayerAnim::WALKING_LEFT);
}
void Player::StartWalkingRight()
{
	state = State::WALKING;
	look = Look::RIGHT;
	SetAnimation((int)PlayerAnim::WALKING_RIGHT);
}

void Player::StartAttacking()
{
	state = State::ATTACK;
	if (IsLookingRight()) {
		SetAnimation((int)PlayerAnim::ATTACK_RIGHT);
		pos.x -= 8;
	}
	else {
		SetAnimation((int)PlayerAnim::ATTACK_LEFT);
		posChange = true;
		pos.x -= 24;
	}
}

void Player::StartHit()
{
	state = State::HIT;
	if (IsLookingRight()) {
		SetAnimation((int)PlayerAnim::HIT);
	}
	else {
		SetAnimation((int)PlayerAnim::HIT_LEFT);
		posChange = true;
	}
}
void Player::StartCrouchAttacking()
{
	state = State::ATTACK;
	if (IsLookingRight()) {
		SetAnimation((int)PlayerAnim::CROUCH_ATTACK_RIGHT);
		pos.x -= 8;
	}
	else {
		SetAnimation((int)PlayerAnim::CROUCH_ATTACK_LEFT);
		posChange = true;
		pos.x -= 24;
	}
}
void Player::CrouchRight() 
{
	state = State::CROUCH;
	look = Look::RIGHT;
	SetAnimation((int)PlayerAnim::CROUCH_RIGHT);
}
void Player::CrouchLeft()
{
	state = State::CROUCH;
	look = Look::LEFT;
	SetAnimation((int)PlayerAnim::CROUCH_LEFT);
}
void Player::StartFalling()
{
	dir.y = PLAYER_SPEED;
	state = State::FALLING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::FALLING_RIGHT);
	else					SetAnimation((int)PlayerAnim::FALLING_LEFT);
}
void Player::StartJumping()
{
	dir.y = -PLAYER_JUMP_FORCE;
	state = State::JUMPING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
	else					SetAnimation((int)PlayerAnim::JUMPING_LEFT);
	jump_delay = PLAYER_JUMP_DELAY;
}
void Player::StartClimbingUp()
{
	state = State::CLIMBING;
	
	SetAnimation((int)PlayerAnim::CLIMBING);
	Sprite* sprite = dynamic_cast<Sprite*>(render);

}
void Player::StartClimbingDown()
{
	state = State::CLIMBING;
	SetAnimation((int)PlayerAnim::CLIMBING_DOWN_L);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
}
void Player::ChangeAnimRight()
{
	 if(state != State::CLIMBING)look = Look::RIGHT;
	switch (state)
	{
		case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_RIGHT);    break; 
		case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_RIGHT); break;
		case State::JUMPING: SetAnimation((int)PlayerAnim::JUMPING_RIGHT); break;
		case State::FALLING: SetAnimation((int)PlayerAnim::FALLING_RIGHT); break;
		case State::CLIMBING: SetAnimation((int)PlayerAnim::CLIMBING); break;
	}
}
void Player::ChangeAnimLeft()
{
	if (state != State::CLIMBING) look = Look::LEFT;
	switch (state)
	{
		case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_LEFT);    break;
		case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_LEFT); break;
		case State::JUMPING: SetAnimation((int)PlayerAnim::JUMPING_LEFT); break;
		case State::FALLING: SetAnimation((int)PlayerAnim::FALLING_LEFT); break;
		case State::CLIMBING: SetAnimation((int)PlayerAnim::CLIMBING_LEFT); break;
	}
}
void Player::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	MoveX();
	MoveY();

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Player::MoveX()
{
	AABB box;
	int prev_x = pos.x;



	if (state != State::DEAD)
	{
		if (IsKeyPressed(KEY_F4) || playerLife <= 0) {
			StartLose();
		}
		if (gethit && state != State::HIT && !hit1) {
			PlayMusicStream(sfx);
			StartHit();
			hit1 = true;
		}
		if (hit1)updateColdDown();

		if (state == State::CLIMBING)	return;

		if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT) && state != State::CROUCH && state != State::ATTACK && pos.x > 0)
		{
			pos.x += -PLAYER_SPEED;
			if (state == State::IDLE) StartWalkingLeft();
			else
			{
				if (IsLookingRight()) ChangeAnimLeft();
			}

			box = GetHitbox();
			if (map->TestCollisionWallLeft(box))
			{
				pos.x = prev_x;
				if (state == State::WALKING) Stop();
			}
		}

		else if (IsKeyDown(KEY_RIGHT) && state != State::CROUCH && state != State::ATTACK && pos.x < 250)
		{
			pos.x += PLAYER_SPEED;
			if (state == State::IDLE) StartWalkingRight();
			else
			{
				if (IsLookingLeft()) ChangeAnimRight();
			}

			box = GetHitbox();
			if (map->TestCollisionWallRight(box))
			{
				pos.x = prev_x;
				if (state == State::WALKING) Stop();
			}
		}
		else
		{
			if (state == State::WALKING) Stop();
		}
	}
	else Lose();
}
void Player::MoveY()
{
	AABB box;
	if (state != State::DEAD)
	{
		if (state == State::JUMPING)
		{
			LogicJumping();
		}
		else if (state == State::CLIMBING)
		{
			LogicClimbing();
		}
		else if (state == State::ATTACK)
		{
			LogicAttacking();
		}
		else if (state == State::HIT)
		{
			LogicHit();
		}
		else //idle, walking, falling, CROUCH
		{

			pos.y += PLAYER_SPEED;
			box = GetHitbox();
			if (map->TestCollisionGround(box, &pos.y))
			{
				if (state == State::FALLING) Stop();

				if (IsKeyDown(KEY_UP)&&!IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT) && state != State::CLIMBING)
				{
					box = GetHitbox();
					if (map->testOnLadder(box, &pos.x) == 0 ) {
						changeLook(0);
						StartClimbingUp();
					}
					else if (map->testOnLadder(box, &pos.x) == 1)
					{
						
						changeLook(1);
						StartClimbingUp();
					}

				}
				else if (IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)&&state != State::CLIMBING)
				{
					///APLICAR BAJAR  IZQUIERDA COMO ARRIBA, CAMBIANDO TestOnLadderTopDown
					box = GetHitbox();
					box.pos.y++;

					if (map->TestOnLadderTopDown(box, &pos.x, &pos.y) == 1){
						changeLook(0);
						posChange = true;
						StartClimbingDown();
						noColision = true;
					}
					else if (map->TestOnLadderTopDown(box, &pos.x, &pos.y) == 0){
						changeLook(1);
						noColision = true;
						posChange = true;
						StartClimbingDown();
					}


					else {
						if (IsLookingLeft()) CrouchLeft();
						else CrouchRight();
						if (IsKeyDown(KEY_Q))
						{
							PlayMusicStream(sfx);
							attacking = true;
							StartCrouchAttacking();
						}
					}
				}
				else if (IsKeyPressed(KEY_Q) && state != State::ATTACK)
				{
					PlayMusicStream(sfx);
					attacking = true;
					StartAttacking();
				}
				else if (IsKeyPressed(KEY_SPACE))
				{
					StartJumping();
				}
				else
				{
					if (state == State::CROUCH) Stop();
				}
			}
			else
			{
				if (state != State::FALLING) StartFalling();
			}
		}
	}
}
void Player::LogicAttacking()
{
	UpdateMusicStream(sfx);
	cnt++;
	if (cnt == 20)
	{
		if (posChange) pos.x += 24;
		else  pos.x += 8;
		Stop();
		cnt = 0;
		posChange = false;
		attacking = false;
		
	}

}

void Player::LogicHit()
{
	UpdateMusicStream(sfx);
	cnt++;
	if (cnt == 20)
	{
		Stop();
		cnt = 0;
		posChange = false;
		if (climbing) {
			state = State::CLIMBING;
			SINGLE_ANIMATION_0 = 0;
			SINGLE_ANIMATION_1 = 0;
		}
			
	}
	if (!climbing)
	{
		if (posChange)pos.x += 1;
		else pos.x -= 1;
	}

}
void Player::updateColdDown()
{
	if (HitColdDown == 80) {
		gethit = false;
		hit1 = false;
		HitColdDown = 0;
	}
	else HitColdDown++;
}

void Player::Lose()
{
	if (cnt == 20)
	{
		SetAnimation((int)PlayerAnim::DEAD);
	}

	if (cnt == 100)
	{
		ActualDead = true;
	}
		cnt++;
	
}

void Player::LogicJumping()
{
	AABB box, prev_box;
	int prev_y;

	jump_delay--;
	if (jump_delay == 0)
	{
		prev_y = pos.y;
		prev_box = GetHitbox();

		pos.y += dir.y;
		dir.y += GRAVITY_FORCE;
		jump_delay = PLAYER_JUMP_DELAY;

		//Is the jump finished?
		if (dir.y > PLAYER_JUMP_FORCE)
		{
			dir.y = PLAYER_SPEED;
			StartFalling();
		}
		else
		{
			//Jumping is represented with 3 different states
			if (IsAscending())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
				else					SetAnimation((int)PlayerAnim::JUMPING_LEFT);
			}
			else if (IsLevitating())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::LEVITATING_RIGHT);
				else					SetAnimation((int)PlayerAnim::LEVITATING_LEFT);
			}
			else if (IsDescending())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::FALLING_RIGHT);
				else					SetAnimation((int)PlayerAnim::FALLING_LEFT);
			}
		}
		//We check ground collision when jumping down
		if (dir.y >= 0)
		{
			box = GetHitbox();

			//A ground collision occurs if we were not in a collision state previously.
			//This prevents scenarios where, after levitating due to a previous jump, we found
			//ourselves inside a tile, and the entity would otherwise be placed above the tile,
			//crossing it.
			if (!map->TestCollisionGround(prev_box, &prev_y) &&
				map->TestCollisionGround(box, &pos.y))
			{
				Stop();
			}
		}
	}
}
void Player::changeLook(int dir)
{
	if (dir == 0) ChangeAnimLeft();
	if (dir == 1) ChangeAnimRight();
}
void Player::LogicClimbing()
{
	AABB box;
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	climbing = true;
	
	if ((IsKeyDown(KEY_UP) && IsLookingRight()) || movingUp)
	{

		movingUp = true;
		if (cnt2 <= 20)
		{
			if (SINGLE_ANIMATION_0 == 0) {
				ChangeAnimRight();
				SINGLE_ANIMATION_1 = 0;
				SINGLE_ANIMATION_0++;
				pos.y += 1;
			}
			if (posChange) {
				pos.y += 2;

			}
			pos.y -= PLAYER_LADDER_SPEED;
			pos.x += PLAYER_LADDER_SPEED;
			posChange = false;
			sprite->NextFrame();
			cnt2++;
		}
		else {
			cnt2 = 0;
			movingUp = false;
		}

	}
	else if (IsKeyDown(KEY_UP) && IsLookingLeft() || movingUpL)
	{
		if (cnt2 <= 20)
		{
			if (SINGLE_ANIMATION_1 == 0) {
				ChangeAnimLeft();
				SINGLE_ANIMATION_0 = 0;
				SINGLE_ANIMATION_1++;
				pos.y += 1;
			}
			if (posChange) {
				pos.y += 5;

			}
				
			pos.y -= PLAYER_LADDER_SPEED;
			pos.x -= PLAYER_LADDER_SPEED;
			sprite->NextFrame();
			cnt2++;
			posChange = false;
		}
		else {
			
			cnt2 = 0;
			movingUpL = false;
		}
	}

	else if (IsKeyDown(KEY_DOWN) && IsLookingRight() || movingDownL)
	{
		movingDownL = true;
		if (cnt2 <= 20)
		{
			if (SINGLE_ANIMATION_1 == 0) {
				SetAnimation((int)PlayerAnim::CLIMBING_DOWN_L);
				
				SINGLE_ANIMATION_0 = 0;
				SINGLE_ANIMATION_1++;
			}
			pos.y += PLAYER_LADDER_SPEED;
			pos.x -= PLAYER_LADDER_SPEED;
			sprite->NextFrame();
			cnt2++;
		}
		else {
			cnt2 = 0;
			noColision = false;
			movingDownL = false;
		}
	}
	else if (IsKeyDown(KEY_DOWN) && IsLookingLeft() || movingDown)
	{
		
		movingDown = true;
		if (cnt2 <= 11)
		{
			if (SINGLE_ANIMATION_0 == 0) {
				SetAnimation((int)PlayerAnim::CLIMBING_DOWN_R);
				
				SINGLE_ANIMATION_1 = 0;
				SINGLE_ANIMATION_0++;
			}
			pos.y += PLAYER_LADDER_SPEED;
			pos.x += PLAYER_LADDER_SPEED;
			sprite->NextFrame();
			cnt2++;
		}
		else {
			cnt2 = 0;
			noColision = false;
			movingDown = false;
		}
	}
	else if (movingUp == false && movingUpL == false && movingDown == false && movingDownL == false) {
		sprite->SetManualMode();
	}

	box = GetHitbox();

	if (!noColision)
	{
		if (map->TestCollisionGround(box, &pos.y)) {
			if(movingDown) look = Look::RIGHT;
			else if (movingDownL)look = Look::LEFT;
			if (movingUp) look = Look::RIGHT;
			else if (movingUpL)look = Look::LEFT;
			cnt2 = 0;
			SINGLE_ANIMATION_1 = 0;
			SINGLE_ANIMATION_0 = 0;
			posChange = false;
			movingDown = false;
			movingUp = false;
			movingUpL = false;
			movingDownL = false;
			climbing = false;
			noColision = false;
			Stop();
			sprite->SetAutomaticMode();
		}
	}

}


bool Player::getCondition() 
{
	return ActualDead;
}

void Player::DrawDebug(const Color& col) const
{	
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
	
	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18*16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}