#include "Player.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

bool Player::init()
{
	if (!Sprite::initWithFile("player.png"))
	{
		return false;
	}

	return true;
}
