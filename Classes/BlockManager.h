#ifndef __BLOCKMANAGER_H__
#define __BLOCKMANAGER_H__

#include "cocos2d.h"

class GameBlock;

// A Manager Class dedicated to the management of GameBlocks within a Scene.
class BlockManager
{
private:
	std::vector<std::vector<GameBlock*>> _blocks;
public:
	BlockManager();
	~BlockManager();

	// Getter Functions
	inline const int getSize() { int size = 0; for (const std::vector<GameBlock*> vector : _blocks) { size += vector.size(); }  return size; }
	const std::vector<std::vector<GameBlock*>> getBlocks();

	// Utility Functions
	bool spawnBlocks(cocos2d::Scene* parent);
};

#endif __BLOCKMANAGER_H__
