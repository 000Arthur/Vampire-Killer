#include "Skeleton.h"
#include "Sprite.h"

Skeleton::Skeleton(const Point& p, int width, int height, int frame_width, int frame_height) :
	Enemy(p, width, height, frame_width, frame_height)
{
	attack_delay = 0;
	state = SkeletonState::ROAMING;

	current_step = 0;
	current_frames = 0;
}
Skeleton::~Skeleton()
{
}
AppStatus Skeleton::Initialise(Look look, const AABB& area)
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
	sprite->SetNumberAnimations((int)SkeletonAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)SkeletonAnim::IDLE_RIGHT, Skeleton_ANIM_DELAY);
	sprite->AddKeyFrame((int)SkeletonAnim::IDLE_RIGHT, { 0, 0, -n, n });
	sprite->SetAnimationDelay((int)SkeletonAnim::IDLE_LEFT, Skeleton_ANIM_DELAY);
	sprite->AddKeyFrame((int)SkeletonAnim::IDLE_LEFT, { 0, 0, n, n });

	sprite->SetAnimationDelay((int)SkeletonAnim::WALKING_RIGHT, Skeleton_ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)SkeletonAnim::WALKING_RIGHT, { (float)i*n, 0, -n, n });
	sprite->SetAnimationDelay((int)SkeletonAnim::WALKING_LEFT, Skeleton_ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)SkeletonAnim::WALKING_LEFT, { (float)i*n, 0, n, n });


	this->look = look;
	visibility_area = area;

	InitPattern(look);

	return AppStatus::OK;
}
void Skeleton::InitPattern(Look look)
{
	//Multiplying by 3 ensures sufficient time for displaying all 3 frames of the
	//walking animation, resulting in smoother transitions and preventing the animation
	//from appearing rushed or incomplete

	const int n = Skeleton_ANIM_DELAY *3;

	if (look == Look::RIGHT)
	{
		UpdateLook((int)SkeletonAnim::WALKING_LEFT);
		pattern.push_back({ {-Skeleton_SPEED, 0}, n, (int)SkeletonAnim::WALKING_LEFT });
		moveLeft = true;
	}
	else 
	{
		UpdateLook((int)SkeletonAnim::WALKING_RIGHT);
		pattern.push_back({ {-Skeleton_SPEED, 0}, n, (int)SkeletonAnim::WALKING_RIGHT });
		moveLeft = false;
	}

	current_step = 0;
	current_frames = 0;
}

bool Skeleton::getPosEnemy(Point player)
{
	if (player.x == pos.x)return true;
	else return false;
}
bool Skeleton::Update(const AABB& box)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	bool shoot = false;
	int anim_id;

	if (state == SkeletonState::ROAMING)
	{

		if (moveLeft)pos += pattern[current_step].speed;
		else pos -= pattern[current_step].speed;
		current_frames++;

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
void Skeleton::UpdateLook(int anim_id)
{
	SkeletonAnim anim = (SkeletonAnim)anim_id;
	look = (anim == SkeletonAnim::IDLE_LEFT ||
			anim == SkeletonAnim::WALKING_LEFT ||
			anim == SkeletonAnim::ATTACK_LEFT) ? Look::LEFT : Look::RIGHT;
}

void Skeleton::GetShootingPosDir(Point* p, Point* d) const
{
	if (look == Look::LEFT)
	{
		p->x = pos.x + Skeleton_SHOT_OFFSET_X_LEFT;
		*d = { -Skeleton_SHOT_SPEED, 0 };
	}
	else
	{
		p->x = pos.x + Skeleton_SHOT_OFFSET_X_RIGHT;
		*d = { Skeleton_SHOT_SPEED, 0 };
	}
	p->y = pos.y + Skeleton_SHOT_OFFSET_Y;
}