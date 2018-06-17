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

GameBlock* GameBlock::createScoreBlock(const Vec2& location)
{
	GameBlock* block = GameBlock::create();
	block->setPosition(location);
	return block;
}

