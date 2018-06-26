#include "Ball.h"
#include "SimpleAudioEngine.h"

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

void Ball::handleCollision(const Size& size, const Vec2& position) {
	// Grab Ball Parameters
	float radius = getContentSize().width / 2;
	Vec2 ballPosition = getPosition();

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

	// If ball position is between the left and right, we are colliding on the y-axis
	if (ballPosition.x > left && ballPosition.x < right)
	{
		float maxY = abs(top - ballPosition.y);
		float minY = abs(bottom - ballPosition.y);

		// If within radius, we have collided on the y-axis so flip y velocity
		if (maxY <= radius || minY <= radius)
		{
			_velocity.y *= -1.0f;
		}
	}
	else
	{
		float maxX = abs(right - ballPosition.x);
		float minX = abs(left - ballPosition.x);

		// If within radius, we have collided on the x-axis so flip x velocity
		if (minX <= radius)
		{
			if (_velocity.x != -1.0f)
			{
				_velocity.x *= -1.0f;
			}
		}
		else if (maxX <= radius)
		{
			if (_velocity.x != 1.0f)
			{
				_velocity.x *= -1.0f;
			}
		}
	}
}

bool Ball::checkBounds() {
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
	}
	else if (abs(bottomSide - position.y) <= radius)
	{
		// We have hit the bottom of the screen, stop moving and prepare to be reset.
		_moving = false;
		return false;
	}

	return true;
}
