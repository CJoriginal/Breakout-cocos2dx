#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

using namespace cocos2d;

class Player : public Sprite
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(Player);
	bool init() override;
};

#endif // __PLAYER_H__
