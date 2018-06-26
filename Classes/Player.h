#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

using namespace cocos2d;

class Ball;

class Player : public Sprite
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(Player);
	bool init() override;
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	bool half();

protected:
	void update(float dt) override;

private:
	bool move(float dt);
	bool moving;

	// 0 - Left, 1 - Right
	int direction;
};

#endif // __PLAYER_H__
