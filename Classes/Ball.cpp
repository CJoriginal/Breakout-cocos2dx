#include "Ball.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

bool Ball::init()
{
	if (!Sprite::initWithFile("Ball.png"))
	{
		return false;
	}

	return true;
}
