#pragma once
#include "Enemy.h"

#define BAT_SPEED			1
#define BAT_ANIM_DELAY	(4*ANIM_DELAY)

#define BAT_SHOT_OFFSET_X_LEFT	-14
#define BAT_SHOT_OFFSET_X_RIGHT	 26
#define BAT_SHOT_OFFSET_Y			-22

#define BAT_SHOT_SPEED	4

enum class batState { ROAMING, ATTACK };
enum class batAnim {  IDLE_LEFT, IDLE_RIGHT, WALKING_LEFT, WALKING_RIGHT,
						ATTACK_LEFT, ATTACK_RIGHT, NUM_ANIMATIONS };

struct StepBat
{
	Point speed;	//direction
	int frames;		//duration in number of frames
	int anim;		//graphical representation
};

class Bat : public Enemy
{
public:
	Bat(const Point& p, int width, int height, int frame_width, int frame_height);
	~Bat();

	
	virtual bool getPosEnemy(Point player);
	//Initialize the enemy with the specified look and area
	AppStatus Initialise(Look look, const AABB& area) override;

	//Update the enemy according to its logic, return true if the enemy must shoot
	bool Update(const AABB& box) override;

	//Retrieve the position and direction of the shot to be thrown
	void GetShootingPosDir(Point* pos, Point* dir) const override;

private:
	//Create the pattern behaviour
	void InitPattern(Look look);
	
	//Update looking direction according to the current step of the pattern
	void UpdateLook(int anim_id);

	int attack_delay;	//delay between attacks
	batState state;

	int current_step;	//current step of the pattern
	int current_frames;	//number of frames in the current step
	std::vector<StepBat> pattern;

	int counterYplus;
	int counterYminor;

	bool moveLeft;
};

