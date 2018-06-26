#ifndef __GAMEBLOCK_H__
#define __GAMEBLOCK_H__

#include "cocos2d.h"

// Base class of GameBlocks
class GameBlock : public cocos2d::Sprite
{
protected:
	int value;

	bool setupPhysics();
public:
	CREATE_FUNC(GameBlock);
	bool init() override;
	virtual bool init(std::string filename);
	inline const int getValue() {return value;}
};

class RedBlock : public GameBlock
{
public:
	CREATE_FUNC(RedBlock);
	bool init() override;
};

class OrangeBlock : public GameBlock
{
public:
	CREATE_FUNC(OrangeBlock);
	bool init() override;
};

class GreenBlock : public GameBlock
{
public:
	CREATE_FUNC(GreenBlock);
	bool init() override;
};

class YellowBlock : public GameBlock
{
public:
	CREATE_FUNC(YellowBlock);
	bool init() override;
};


#endif // __GAMEBLOCK_H__
