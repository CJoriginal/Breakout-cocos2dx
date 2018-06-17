#ifndef __GAMEBLOCK_H__
#define __GAMEBLOCK_H__

#include "cocos2d.h"

class GameBlock : public cocos2d::Sprite
{
public:
	CREATE_FUNC(GameBlock);
	bool init() override;
};

#endif // __GAMEBLOCK_H__
