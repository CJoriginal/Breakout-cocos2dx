#ifndef __BALL_H__
#define __BALL_H__

#include "cocos2d.h"

// A Sprite Class to represent and handle collision for the ball.
class Ball : public cocos2d::Sprite
{
private:
	cocos2d::Vec2 _currentPosition;
	cocos2d::Vec2 _startPosition;
	cocos2d::Vec2 _velocity;

	float _magnitude;
	float _radius;

	float _leftSide;
	float _rightSide;
	float _topSide;
	float _bottomSide;

	bool _moving;
	bool _hitTop;
	bool _outOfBounds;

	void const clampVelocityX(float adjust);
protected:
	void update(float dt) override;
public:
	// implement the "static create()" method manually
	CREATE_FUNC(Ball);
	bool init() override;

	void setup();

	inline cocos2d::Vec2& getVelocity() { return _velocity; }

	inline bool const increaseSpeed() { _magnitude *= 1.33f; return true; }
	inline bool const hasTouchedTop() { return _hitTop; }
	inline bool const isOutOfBounds() { return _outOfBounds; }

	void handlePlayerCollision(const cocos2d::Sprite* player);
	void handleBlockCollision();

	bool checkBounds();
};

#endif // __BALL_H__
