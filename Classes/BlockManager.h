#ifndef __BLOCKMANAGER_H__
#define __BLOCKMANAGER_H__

#include "cocos2d.h"

class GameBlock;

// A Manager Class dedicated to the management of GameBlocks within a Scene.
class BlockManager
{
private:
	std::vector<std::vector<GameBlock*>> _blocks;
	int _size;
public:
	BlockManager();
	~BlockManager();

	// Getter Functions
	inline const int getSize() { return _size; }
	const std::vector<std::vector<GameBlock*>> getBlocks();

	// Utility Functions
	bool spawnBlocks(cocos2d::Scene* parent);
	void removeBlock(GameBlock* block);
};

#endif __BLOCKMANAGER_H__
