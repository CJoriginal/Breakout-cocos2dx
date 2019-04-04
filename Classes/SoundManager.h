#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__

#include "cocos2d.h"

#define PLAYER_COLLISION_SFX	"PlayerSound.mp3"
#define NORMAL_COLLISION_SFX	"CollisionSound.mp3"
#define PLAYER_DEATH_SFX		"DeathSound.mp3"
#define FAIL_SFX				"FailureSound.mp3"
#define WIN_SFX					"WinSound.mp3"

class SoundManager : public cocos2d::Node
{
public:
	SoundManager() {};
	~SoundManager() {};

	void init(float dt);

	void playPlayerSound(float dt);
	void playCollisionSound(float dt);
	void playDeathSound(float dt);
	void playFailureSound(float dt);
	void playWinSound(float dt);
};

#endif
