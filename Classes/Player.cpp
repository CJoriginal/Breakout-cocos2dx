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

	// Add Physics Body
	auto body = PhysicsBody::createBox(getContentSize());
	body->setDynamic(true);
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

void Player::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	log("Key with keycode %d pressed", keyCode);
	switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			direction = 0;
			moving = true;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			direction = 1;
			moving = true;
			break;
	}
}

void Player::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	log("Key with keycode %d released", keyCode);
	switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		default:
			moving = false;
			break;
	}
}

bool Player::move(float dt)
{
	if (!moving)
	{
		return false;
	}

	Vec2 currentPosition = getPosition();
	
	float newX = currentPosition.x;

	// Update current x co-ordinate with new movement
	if (direction)
	{
		newX += 450.0f * dt;
	}
	else
	{
		newX += -450.0f * dt;
	}

	setPosition(newX,currentPosition.y);

	return true;
}

bool Player::half()
{
	setScale(0.5f);
	return true;
}
