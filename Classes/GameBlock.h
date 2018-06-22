#ifndef __GAMEBLOCK_H__
#define __GAMEBLOCK_H__

#include "cocos2d.h"

class GameBlock : public cocos2d::Sprite
{
public:
	CREATE_FUNC(GameBlock);
	bool init() override;
	virtual bool init(std::string filename);
	int value;

protected:
	bool setupPhysics();
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
