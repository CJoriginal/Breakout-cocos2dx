#include "Ball.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

bool Ball::init()
{
	if (!Sprite::initWithFile("Ball.png"))
	{
		return false;
	}

	auto body = PhysicsBody::createCircle(getContentSize().width / 2);
	body->setDynamic(true);
	body->setCollisionBitmask(2);
	body->setContactTestBitmask(true);
	setPhysicsBody(body);

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getVisibleSize();

	_startPosition = Vec2(winSize.width * 0.5f, winSize.height * 0.4f);
	
	setup();

	scheduleUpdate();

	return true;
}

void Ball::update(float dt)
{
	if (_moving) {
		Vec2 position = getPosition();
		Vec2 newPosition = Vec2(position.x + (_magnitude * _velocity.x * dt), position.y + (_magnitude * _velocity.y * dt));
		setPosition(newPosition);
	}
}

void Ball::setup()
{
	setPosition(_startPosition);

	_moving = false;
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
}

void Ball::handleCollision(const Size& size, const Vec2& position) {
	cocos2d::log("handling collision");

	// Grab Ball Parameters
	float radius = getContentSize().width / 2;
	const Vec2 ballPosition = getPosition();

	// Grab Sprite Parameters
	float height = size.height;
	float width = size.width;
	float x = position.x;
	float y = position.y;

	// Grab Box Dimensions
	float top = y + height / 2;
	float bottom = y - height / 2;
	float left = x - width / 2;
	float right = x + width / 2;

	// If ball position is between the top and bottom, we are colliding on the x-axis
	if (ballPosition.y < top && ballPosition.y > bottom)
	{
		float maxX = abs(right - ballPosition.x);
		float minX = abs(left - ballPosition.x);

		// If within radius, we have collided on the x-axis so flip x velocity
		if (abs(left - ballPosition.x) <= radius || abs(right - ballPosition.x) <= radius)
		{
			_velocity.x *= -1.0f;
		}
	}
	else
	{
		float maxY = abs(top - ballPosition.y);
		float minY = abs(bottom - ballPosition.y);

		// If within radius, we have collided on the y-axis so flip y velocity
		if (maxY <= radius || minY <= radius)
		{
			_velocity.y *= -1.0f;
		}
	}
}

bool Ball::checkBounds() {
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getVisibleSize();

	Vec2 position = getPosition();
	float radius = getContentSize().width / 2;

	float leftSide = origin.x;
	float rightSide = origin.x + winSize.width;
	float topSide = origin.y + winSize.height;
	float bottomSide = origin.y;

	if (abs(leftSide - position.x) <= radius || abs(rightSide - position.x) <= radius)
	{
		_velocity.x *= -1.0f;
	}
	else if (abs(topSide - position.y) <= radius)
	{
		_velocity.y *= -1.0f;
		if (!_hitTop)
		{
			_hitTop = true;
		}
	}
	else if (abs(bottomSide - position.y) <= radius)
	{
		setup();
		startMovement();

		return false;
	}

	return true;
}
