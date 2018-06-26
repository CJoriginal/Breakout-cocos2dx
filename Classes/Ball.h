#ifndef __BALL_H__
#define __BALL_H__

#include "cocos2d.h"

using namespace cocos2d;

class Ball : public Sprite
{
private:
	Vec2 _startPosition;
	float _magnitude;
	Vec2 _velocity;
	bool _moving;
	bool _hitTop;
protected:
	void update(float dt) override;
public:
	// implement the "static create()" method manually
	CREATE_FUNC(Ball);
	bool init() override;
	inline void startMovement() { _moving = true;}

	inline Vec2& getVelocity() { return _velocity; }
	inline void setVelocity(const Vec2& newVelocity) { _velocity = newVelocity; };

	void handleCollision(const Size& size, const Vec2& position);
	inline bool const increaseSpeed() { _magnitude *= 1.1f; return true; }
	inline bool const hasTouchedTop() { return _hitTop; }

	bool checkBounds();
};

#endif // __BALL_H__
