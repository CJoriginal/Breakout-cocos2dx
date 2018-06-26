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

bool BlockManager::spawnBlocks(Scene* parent)
{
	// Check parent exists
	if (!parent)
	{
		return false;
	}

	// Set starting position for grid iteration
	float startingX = 125.0f;
	float startingY = 800.0f;

	// Fill Block Container with blocks
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 14; j++) {
			Vec2 location = Vec2(startingX + (60.0f * j), startingY + (-20.0f * i));

			GameBlock* block;

			if (i < 2) {
				// Top Rows - Red Blocks
				block = RedBlock::create();
			}
			else if (i >= 2 && i < 4) {
				// Top Mid Rows - Orange Blocks
				block = OrangeBlock::create();
			}
			else if (i >= 2 && i < 6) {
				// Bottom Mid Rows - Green Blocks
				block = GreenBlock::create();
			}
			else {
				// Bottom Rows - Yellow Blocks
				block = YellowBlock::create();
			}

			// If block failed to create, abort process and return false
			if (!block)
			{
				return false;
			}

			// Set position of block
			block->setPosition(location);

			// Add to Manager
			_blocks[i][j] = block;
			_size++;

			// Add to Parent
			parent->addChild(block);
		}
	}

	// Check we have created blocks, else return false
	if (!_blocks.size())
	{
		return false;
	}

	return true;
}

void BlockManager::removeBlock(GameBlock* block)
{
	// Cleanup Blocks
	block->removeFromParentAndCleanup(true);
	_size--;
}

const std::vector<std::vector<GameBlock*>> BlockManager::getBlocks()
{
	return _blocks;
}
