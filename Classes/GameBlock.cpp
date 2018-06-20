#include "GameBlock.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

bool GameBlock::init()
{
	if (!Sprite::initWithFile("block.png")) 
	{
		return false;
	}

	return true;
}

bool RedBlock::init()
{
	if (!Sprite::initWithFile("redscore.png"))
	{
		return false;
	}

	value = 7;

	return true;
}

bool OrangeBlock::init()
{
	if (!Sprite::initWithFile("orangescore.png"))
	{
		return false;
	}

	value = 5;

	return true;
}

bool GreenBlock::init()
{
	if (!Sprite::initWithFile("greenscore.png"))
	{
		return false;
	}

	value = 3;

	return true;
}

bool YellowBlock::init()
{
	if (!Sprite::initWithFile("yellowscore.png"))
	{
		return false;
	}

	value = 1;

	return true;
}
