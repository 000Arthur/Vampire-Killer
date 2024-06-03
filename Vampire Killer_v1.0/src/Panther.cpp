#include "Panther.h"
#include "Sprite.h"

Panther::Panther(const Point& p, int width, int height, int frame_width, int frame_height) :
	Enemy(p, width, height, frame_width, frame_height)
{
	attack_delay = 0;
	state = PantherState::ROAMING;

	current_step = 0;
	current_frames = 0;
}
Panther::~Panther()
{
}
AppStatus Panther::Initialise(Look look, const AABB& area)
{
	int i;
	const int n = SLIME_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_ENEMIES));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for slime sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)PantherAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)PantherAnim::IDLE_RIGHT, Panther_ANIM_DELAY);
	sprite->AddKeyFrame((int)PantherAnim::IDLE_RIGHT, { 0, n, -n, n });
	sprite->SetAnimationDelay((int)PantherAnim::IDLE_LEFT, Panther_ANIM_DELAY);
	sprite->AddKeyFrame((int)PantherAnim::IDLE_LEFT, { 0, n, n, n });

	sprite->SetAnimationDelay((int)PantherAnim::WALKING_RIGHT, Panther_ANIM_DELAY);
	for (i = 1; i < 4; ++i)
		sprite->AddKeyFrame((int)PantherAnim::WALKING_RIGHT, { (float)i * n, n, -n, n });
	sprite->SetAnimationDelay((int)PantherAnim::WALKING_LEFT, Panther_ANIM_DELAY);
	for (i = 1; i < 4; ++i)
		sprite->AddKeyFrame((int)PantherAnim::WALKING_LEFT, { (float)i * n, n, n, n });


	this->look = look;


	visibility_area = area;
	InitPattern(look);

	return AppStatus::OK;
}
void Panther::InitPattern(Look look)
{

	//Multiplying by 3 ensures sufficient time for displaying all 3 frames of the
	//walking animation, resulting in smoother transitions and preventing the animation
	//from appearing rushed or incomplete

	const int n = Panther_ANIM_DELAY * 3;
	if (look == Look::LEFT) {
		pattern1.push_back({ {0, 0}, 2 * n, (int)PantherAnim::IDLE_RIGHT });
		moveLeft = false;
	}
	else if (look == Look::RIGHT) {
	pattern1.push_back({ {0, 0}, 2 * n, (int)PantherAnim::IDLE_LEFT });
	moveLeft = true;
	}
		
	current_step = 0;
	current_frames = 0;
}
bool Panther::Update(const AABB& box)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	bool shoot = false;
	int anim_id;

	if (pos.x == 240) {
		int z = 2;
	}


	if (state == PantherState::ROAMING)
	{
		if (IsVisible(box))
		{
			state = PantherState::ATTACK;
			//The attack animation consists of 2 frames, with the second one being when
			//we throw the shot. Wait for a frame before initiating the attack.
			attack_delay = Panther_ANIM_DELAY;

			if (look == Look::LEFT)	sprite->SetAnimation((int)PantherAnim::WALKING_LEFT);
			else					sprite->SetAnimation((int)PantherAnim::WALKING_RIGHT);
		}
		else
		{
			pos += pattern1[current_step].speed;
			current_frames++;

			if (current_frames == pattern1[current_step].frames)
			{
				current_step++;
				current_step %= pattern1.size();
				current_frames = 0;

				anim_id = pattern1[current_step].anim;
				sprite->SetAnimation(anim_id);
				UpdateLook(anim_id);
			}
		}
	}
	else if (state == PantherState::ATTACK)
	{
			if(moveLeft) pos.x -= Panther_SPEED;
			else pos.x += Panther_SPEED;;
			
			SetPos(pos);
			attack_delay--;
			if (attack_delay == 0)
			{
				//shoot = true;

				//The attack animation consists of 2 frames. Wait for a complete loop
				//before shooting again
				attack_delay = 2* Panther_ANIM_DELAY;
			}
	}
	sprite->Update();

	return shoot;
}
void Panther::UpdateLook(int anim_id)
{
	PantherAnim anim = (PantherAnim)anim_id;
	look = (anim == PantherAnim::IDLE_LEFT ||
		anim == PantherAnim::WALKING_LEFT ||
		anim == PantherAnim::ATTACK_LEFT) ? Look::LEFT : Look::RIGHT;
}
void Panther::GetShootingPosDir(Point* p, Point* d) const
{
	if (look == Look::LEFT)
	{
		p->x = pos.x + Panther_SHOT_OFFSET_X_LEFT;
		*d = { -Panther_SHOT_SPEED, 0 };
	}
	else
	{
		p->x = pos.x + Panther_SHOT_OFFSET_X_RIGHT;
		*d = { Panther_SHOT_SPEED, 0 };
	}
	p->y = pos.y + Panther_SHOT_OFFSET_Y;
}