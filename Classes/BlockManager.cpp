#include "BlockManager.h"
#include "SimpleAudioEngine.h"
#include "GameBlock.h"

USING_NS_CC;

BlockManager::BlockManager()
{
	_blocks = std::vector<std::vector<GameBlock*>>(8);
	for (int i = 0; i < 8; i++) {
		_blocks[i] = std::vector<GameBlock*>(14);
	}
	
	_size = 0;
}

BlockManager::~BlockManager()
{

}

bool BlockManager::spawnBlocks()
{
	// Set starting position for grid iteration
	float startingX = 125.0f;
	float startingY = 800.0f;

	// Fill Block Container with blocks
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 14; j++) {
			Vec2 location = Vec2(startingX + (60.0f * j), startingY + (-20.0f * i));

			GameBlock* block;

			if (i < 2) {
				block = RedBlock::create();
			}
			else if (i >= 2 && i < 4) {
				block = OrangeBlock::create();
			}
			else if (i >= 2 && i < 6) {
				block = GreenBlock::create();
			}
			else {
				block = YellowBlock::create();
			}

			if (!block)
			{
				return false;
			}

			block->setPosition(location);

			_blocks[i][j] = block;
			_size++;
		}
	}

	if (!_blocks.size())
	{
		return false;
	}

	return true;
}

void BlockManager::removeBlock(GameBlock* block)
{
	block->removeFromParentAndCleanup(true);
	_size--;
}

std::vector<std::vector<GameBlock*>> BlockManager::getBlocks() const
{
	return _blocks;
}
