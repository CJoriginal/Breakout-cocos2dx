#include "Ball.h"
#include "SoundManager.h"

USING_NS_CC;

bool Ball::init()
{
	if (!Sprite::initWithFile("Ball.png"))
	{
		return false;
	}

	// Setup Physics Body
	auto body = PhysicsBody::createCircle(getContentSize().width / 2);
	body->setDynamic(true);
	body->setCollisionBitmask(2);
	body->setContactTestBitmask(true);
	body->setTag(1);
	setPhysicsBody(body);

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getVisibleSize();

	// Set Start Position and turn movement off
	_moving = false;
	_startPosition = Vec2(winSize.width * 0.5f, winSize.height * 0.4f);

	scheduleUpdate();

	return true;
}

void Ball::update(float dt)
{
	// If moving, update the ball position based on our magnitude and given velocity
	if (_moving)
	{
		Vec2 position = getPosition();
		Vec2 newPosition = Vec2(position.x + (_magnitude * _velocity.x * dt), position.y + (_magnitude * _velocity.y * dt));
		setPosition(newPosition);
	}
}

void Ball::setup()
{
	// If the ball is invisible, make it visible
	if (!isVisible())
	{
		setVisible(true);
	}

	// Set to initial position
	setPosition(_startPosition);

	// Initialise initial values
	_hitTop = false;
	_magnitude = 200.0f;

	if (std::round(rand_0_1()))
	{
		_velocity = Vec2(1.0f, -1.0f);
	}
	else
	{
		_velocity = Vec2(-1.0f, -1.0f);
	}

	// Start moving
	_moving = true;
}

void Ball::handleCollision() {
	_velocity.x *= -1.0f;
	_velocity.y *= -1.0f;
}

bool Ball::checkBounds(SoundManager* sound) {
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getVisibleSize();

	// Grab Ball Parameters
	Vec2 position = getPosition();
	float radius = getContentSize().width / 2;

	// Calculate Screen bounds
	float leftSide = origin.x;
	float rightSide = origin.x + winSize.width;
	float topSide = origin.y + winSize.height;
	float bottomSide = origin.y;

	if (abs(leftSide - position.x) <= radius || abs(rightSide - position.x) <= radius)
	{
		// We are bouncing off the sides of the screen, invert the x velocity
		_velocity.x *= -1.0f;

		sound->PlayCollisionSound();
	}
	else if (abs(topSide - position.y) <= radius)
	{
		// We are bouncing off the top of the screen, invert the y velocity
		_velocity.y *= -1.0f;

		// If we are hitting the top for the first time, mark the hitTop modifier condition
		if (!_hitTop)
		{
			_hitTop = true;
		}

		sound->PlayCollisionSound();
	}
	else if (abs(bottomSide - position.y) <= radius)
	{
		// We have hit the bottom of the screen, stop moving and prepare to be reset.
		_moving = false;
		return false;
	}

	return true;
}
