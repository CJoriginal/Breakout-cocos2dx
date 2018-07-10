#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__

#include "cocos2d.h"

class SoundManager
{
private:
	std::string playerSound;
	std::string collisionSound;
	std::string deathSound;
	std::string failureSound;
	std::string winSound;

	bool PreloadSound();

public:
	SoundManager();
	~SoundManager();

	bool PlayPlayerSound();
	bool PlayCollisionSound();
	bool PlayDeathSound();
	bool PlayFailureSound();
	bool PlayWinSound();
};

#endif
