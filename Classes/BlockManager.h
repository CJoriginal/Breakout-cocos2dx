#ifndef __BLOCKMANAGER_H__
#define __BLOCKMANAGER_H__

#include "cocos2d.h"

using namespace cocos2d;

class GameBlock;

class BlockManager
{
	private:
		std::vector<std::vector<GameBlock*>> _blocks;
		int _size;
	public:
		BlockManager();
		~BlockManager();

		bool spawnBlocks();
		void removeBlock(GameBlock* block);

		inline int getSize() const {return _size;}
		std::vector<std::vector<GameBlock*>> getBlocks() const;
};

#endif __BLOCKMANAGER_H__