#ifndef __BALL_H__
#define __BALL_H__

#include "cocos2d.h"

// A Sprite Class to represent and handle collision for the ball.
class Ball : public cocos2d::Sprite
{
private:
	cocos2d::Vec2 _startPosition;
	cocos2d::Vec2 _velocity;
	float _magnitude;
	bool _moving;
	bool _hitTop;
protected:
	void update(float dt) override;
public:
	// implement the "static create()" method manually
	CREATE_FUNC(Ball);
	bool init() override;

	void setup();

	inline cocos2d::Vec2& getVelocity() { return _velocity; }
	inline void setVelocity(const cocos2d::Vec2& newVelocity) { _velocity = newVelocity; };

	inline bool const increaseSpeed() { _magnitude *= 1.33f; return true; }
	inline bool const hasTouchedTop() { return _hitTop; }

	void handleCollision(const cocos2d::Size& size, const cocos2d::Vec2& position);

	bool checkBounds();
};

#endif // __BALL_H__
