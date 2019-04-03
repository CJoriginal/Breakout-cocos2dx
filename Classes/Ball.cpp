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

	// Grab Ball Parameters
	_radius = getContentSize().width / 2;

	// Calculate Screen bounds
	_leftSide = origin.x;
	_rightSide = origin.x + winSize.width;
	_topSide = origin.y + winSize.height;
	_bottomSide = origin.y;

	scheduleUpdate();

	return true;
}

void Ball::update(float dt)
{
	// If moving, update the ball position based on our magnitude and given velocity
	if (_moving)
	{
		_currentPosition = Vec2(clampf(_currentPosition.x + (_magnitude * _velocity.x * dt), _leftSide, _rightSide), clampf(_currentPosition.y + (_magnitude * _velocity.y * dt), _bottomSide, _topSide));
		if (_magnitude != 200.0f)
		{
			_magnitude = 200.0f;
		}

		setPosition(_currentPosition);
	}
}

void Ball::setup()
{
	// If the ball is invisible, make it visible
	if (!isVisible())
	{
		setVisible(true);
	}

	_currentPosition = _startPosition;

	// Set to initial position
	setPosition(_currentPosition);

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

void Ball::handlePlayerCollision(const cocos2d::Sprite* player) 
{
	const cocos2d::Size halfWidth = player->getBoundingBox().size;
	const cocos2d::Vec2 playerPos = player->convertToWorldSpace(player->getPosition());

	const float playerLeft = playerPos.x + halfWidth.width / 3;
	const float playerRight = playerPos.x + (halfWidth.width / 3) * 2;
	
	const cocos2d::Size ballSize = getBoundingBox().size;
	const cocos2d::Vec2 ballPos = convertToWorldSpace(getPosition());

	const float ballCenter = ballPos.x + ballSize.width / 2;

	if (ballCenter > playerLeft && ballCenter < playerRight)
	{
		_velocity.x *= 1.0f;
	} 
	else if (ballCenter < playerLeft || ballCenter > playerRight)
	{
		log("outer %f %f %f", _velocity.x, ballCenter, playerLeft);
		if (ballCenter < playerLeft)
		{
			if (_velocity.x < 0.0f)
			{
				_velocity.x *= 1.0f;
			}
			else
			{
				_velocity.x *= -1.0f;
			}
		}
		else
		{
			if (_velocity.x < 0.0f)
			{
				_velocity.x *= -1.0f;
			}
			else
			{
				_velocity.x *= 1.0f;
			}
		}
	}

	_velocity.y *= -1.0f;
}

void Ball::handleBlockCollision()
{
	_velocity.x *= -1.0f;
	_velocity.y *= -1.0f;
}

bool Ball::checkBounds(SoundManager* sound) {
	if (_currentPosition.x >= _rightSide || _currentPosition.x <= _leftSide)
	{
		// We are bouncing off the sides of the screen, invert the x velocity
		_velocity.x *= -1.0f;

		sound->PlayCollisionSound();
	}
	else if (_topSide <= _currentPosition.y)
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
	else if (_bottomSide >= _currentPosition.y)
	{
		// We have hit the bottom of the screen, stop moving and prepare to be reset.
		_moving = false;
		return false;
	}

	return true;
}
