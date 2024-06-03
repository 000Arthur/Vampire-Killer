#include "Bat.h"
#include "Sprite.h"

Bat::Bat(const Point& p, int width, int height, int frame_width, int frame_height) :
	Enemy(p, width, height, frame_width, frame_height)
{
	attack_delay = 0;
	state = batState::ROAMING;

	current_step = 0;
	current_frames = 0;
}
Bat::~Bat()
{
}
AppStatus Bat::Initialise(Look look, const AABB& area)
{
	int i;
	const int n = 16;

	ResourceManager& data2 = ResourceManager::Instance();
	render = new Sprite(data2.GetTexture(Resource::IMG_BATS));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for slime sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)batAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)batAnim::IDLE_RIGHT, BAT_ANIM_DELAY);
	sprite->AddKeyFrame((int)batAnim::IDLE_RIGHT, { 0, 0, -n, n });
	sprite->SetAnimationDelay((int)batAnim::IDLE_LEFT, BAT_ANIM_DELAY);
	sprite->AddKeyFrame((int)batAnim::IDLE_LEFT, { 0, 0, n, n });

	sprite->SetAnimationDelay((int)batAnim::WALKING_RIGHT, BAT_ANIM_DELAY);
	for (i = 1; i < 3; ++i)
		sprite->AddKeyFrame((int)batAnim::WALKING_RIGHT, { (float)i*n, 0, -n, n });
	sprite->SetAnimationDelay((int)batAnim::WALKING_LEFT, BAT_ANIM_DELAY);
	for (i = 1; i < 3; ++i)
		sprite->AddKeyFrame((int)batAnim::WALKING_LEFT, { (float)i*n, 0, n, n });


	this->look = look;
	visibility_area = area;

	InitPattern(look);

	return AppStatus::OK;
}
void Bat::InitPattern(Look look)
{
	//Multiplying by 3 ensures sufficient time for displaying all 3 frames of the
	//walking animation, resulting in smoother transitions and preventing the animation
	//from appearing rushed or incomplete

	const int n = BAT_ANIM_DELAY*3;

	if (look == Look::RIGHT)
	{
		UpdateLook((int)batAnim::WALKING_LEFT);
		pattern.push_back({ {-BAT_SPEED, 0}, n, (int)batAnim::WALKING_LEFT });
		moveLeft = true;
	}
	else 
	{
		UpdateLook((int)batAnim::WALKING_RIGHT);
		pattern.push_back({ {-BAT_SPEED, 0}, n, (int)batAnim::WALKING_RIGHT });
		moveLeft = false;
	}

	current_step = 0;
	current_frames = 0;
}

bool Bat::getPosEnemy(Point player)
{
	if (player.x == pos.x)return true;
	else return false;
}
bool Bat::Update(const AABB& box)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	bool shoot = false;
	int anim_id;

	if (state == batState::ROAMING)
	{
		if (moveLeft)pos += pattern[current_step].speed;
		
		else pos -= pattern[current_step].speed;
		current_frames++;

		if (counterYplus >= 3)
		{
			if (counterYminor <= 15) {
				pos.y -= 1;
				counterYminor++;
			}
			else if (counterYminor <= 30) {
				pos.y += 1;
				counterYminor++;
			}
			else {
				counterYminor = 0;
			}
			counterYplus = 0;
		}
		else counterYplus++;

			
		if (current_frames == pattern[current_step].frames)
		{
			current_step++;
			current_step %= pattern.size();
			current_frames = 0;
				
			anim_id = pattern[current_step].anim;
			sprite->SetAnimation(anim_id);
			UpdateLook(anim_id);
		}
	}

	sprite->Update();

	return shoot;
}
void Bat::UpdateLook(int anim_id)
{
	batAnim anim = (batAnim)anim_id;
	look = (anim == batAnim::IDLE_LEFT ||
			anim == batAnim::WALKING_LEFT ||
			anim == batAnim::ATTACK_LEFT) ? Look::LEFT : Look::RIGHT;
}

void Bat::GetShootingPosDir(Point* p, Point* d) const
{
	if (look == Look::LEFT)
	{
		p->x = pos.x + BAT_SHOT_OFFSET_X_LEFT;
		*d = { -BAT_SHOT_SPEED, 0 };
	}
	else
	{
		p->x = pos.x + BAT_SHOT_OFFSET_X_RIGHT;
		*d = { BAT_SHOT_SPEED, 0 };
	}
	p->y = pos.y + BAT_SHOT_OFFSET_Y;
}