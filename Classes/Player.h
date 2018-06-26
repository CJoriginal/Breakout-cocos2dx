#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

class Ball;

class Player : public cocos2d::Sprite
{
private:
	bool move(float dt);
	bool moving;

	// 0 - Left, 1 - Right
	int direction;
protected:
	void update(float dt) override;
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
public:
	// implement the "static create()" method manually
	CREATE_FUNC(Player);
	bool init() override;
	bool half();
};

#endif // __PLAYER_H__
