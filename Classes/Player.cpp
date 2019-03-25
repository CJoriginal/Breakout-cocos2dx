#include "Player.h"
#include "SimpleAudioEngine.h"
#include "Ball.h"

USING_NS_CC;

bool Player::init()
{
	if (!Sprite::initWithFile("player.png"))
	{
		return false;
	}

	speed = 600.0f;

	// Add Physics Body
	auto body = PhysicsBody::createBox(getContentSize());
	body->setDynamic(false);
	body->setCollisionBitmask(1);
	body->setContactTestBitmask(true);
	setPhysicsBody(body);

	// Setup Player Movement
	auto kbListener = EventListenerKeyboard::create();
	kbListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
	kbListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(kbListener, this);

	scheduleUpdate();

	return true;
}

void Player::update(float dt) 
{
	if (moving)
	{
		move(dt);
	}
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			// Move Player Left
			direction = 0;
			moving = true;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			// Move Player Right
			direction = 1;
			moving = true;
			break;
	}
}

void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		default:
			// Player is idle, stop moving
			moving = false;
			break;
	}
}

bool Player::move(float dt)
{
	// Check we are moving
	if (!moving)
	{
		return false;
	}

	Vec2 currentPosition = getPosition();
	
	float newX = currentPosition.x;

	// Update current x co-ordinate with new movement
	if (direction)
	{
		newX += speed * dt;
	}
	else
	{
		newX += -speed * dt;
	}

	if (checkBounds(newX))
	{
		setPosition(newX, currentPosition.y);
	}
	else
	{
		setPosition(currentPosition.x, currentPosition.y);
	}

	return true;
}

bool Player::half()
{
	// Reduce player by half
	setScale(0.5f);
	return true;
}

bool Player::checkBounds(float newX) {
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getVisibleSize();

	// Grab Player Parameters
	Vec2 position = getPosition();
	float length = getContentSize().width / 2;

	// Calculate Screen bounds
	float leftSide = origin.x;
	float rightSide = origin.x + winSize.width;
	float topSide = origin.y + winSize.height;
	float bottomSide = origin.y;

	if (abs(leftSide - newX) <= length || abs(rightSide - newX) <= length)
	{
		// We are bouncing off the sides of the screen, invert the x velocity
		return false;
	}

	return true;
}