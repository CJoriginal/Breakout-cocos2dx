#ifndef __BALL_H__
#define __BALL_H__

#include "cocos2d.h"

using namespace cocos2d;

class Ball : public Sprite
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(Ball);
	bool init() override;
};

#endif // __BALL_H__
