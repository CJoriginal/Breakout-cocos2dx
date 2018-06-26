/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "LevelScene.h"
#include "SimpleAudioEngine.h"
#include "Player.h"
#include "Ball.h"
#include "GameBlock.h"
#include "BlockManager.h"

USING_NS_CC;

enum class PhysicsCategory {
	None = 0,
	Player = (1 << 0),    // 1
	Block = (1 << 1), // 2
	All = (Player | Block) // 3
};

Scene* Level::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();

	// Disable Gravity
	scene->getPhysicsWorld()->setGravity(Vec2(0.0f, 0.0f));

	// Display Debug Drawing for Physic Objects
	scene->getPhysicsWorld()->setDebugDrawMask(0xffff);

	auto layer = Level::create();

	scene->addChild(layer);

	return scene;
}

bool Level::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getVisibleSize();

	// Intialise Game Variables
	_start = false;
	_isFirstScreen = true;
	_score = 0;
	_lives = 3;
	_firstBoost = false;
	_secondBoost = false;
	_orangeHit = false;
	_redHit = false;
	_halvedPlayer = false;


	// Draw Background
	auto background = DrawNode::create();
	background->drawSolidRect(origin, winSize * 2, Color4F(0.6f, 0.6f, 0.6f, 1.0f));
	this->addChild(background);

	// Spawn Labels
	spawnLabels(origin, winSize);

	// Spawn player
	_player = Player::create();
	_player->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.2f));
	this->addChild(_player);

	// Spawn Collidable Blocks
	_blocks = new BlockManager();
	spawnBlocks();

	// Add Event Listeners
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseUp = CC_CALLBACK_1(Level::onMouseUp, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

	// Add Collision Handlers
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Level::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	this->scheduleUpdate();

    return true;
}

void Level::update(float dt)
{
	if (_start)
	{
		if (_ball && !_ball->checkBounds())
		{
			_lives -= 1;
			updateLabelText(_livesLabel, "Lives: ", _lives);
			resetModifiers();
		}

		if (!_lives)
		{
			// Display Game Over Screen
			// GameOver(false);
		}
		else
		{
			if (!_blocks->getSize())
			{
				if (_isFirstScreen)
				{
					_ball->setup();
					spawnBlocks();
					_ball->startMovement();
					_isFirstScreen = false;
				}

				// Display Victory Screen
			}
		}
	}
}

void Level::onMouseUp(cocos2d::Event* event) {
	// If we have already performed the startup, do not execute.
	if (_start)
	{
		return;
	}

	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);

	// Check Mouse Event is valid
	if (!mouseEvent)
	{
		cocos2d::log("Could not start game due to bad cast");
		return;
	}

	// If player clicks left button, start the game
	if (EventMouse::MouseButton::BUTTON_LEFT == mouseEvent->getMouseButton())
	{
		_start = true;

		auto origin = Director::getInstance()->getVisibleOrigin();
		auto winSize = Director::getInstance()->getVisibleSize();

		// Spawn Wrecking Ball
		_ball = Ball::create();
		this->addChild(_ball);
		_ball->startMovement();
	}
}

bool Level::onContactBegin(cocos2d::PhysicsContact &contact)
{
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();

	if ((1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()) || (2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask()))
	{
		Ball* ball = static_cast<Ball*>(a->getOwner());

		if (ball)
		{
			Player* player = dynamic_cast<Player*>(b->getOwner());
			if (player)
			{
				_ball->handleCollision(player->getContentSize(), player->getPosition());
			}

			GameBlock* block = dynamic_cast<GameBlock*>(b->getOwner());
			if (block)
			{
				_ball->handleCollision(block->getContentSize(), block->getPosition());
				
				_score += block->value;
				_blockCollisions += 1;

				updateLabelText(_scoreLabel, "Score: ", _score);

				checkBallModifiers(block);
				checkPlayerModifiers();

				_blocks->removeBlock(block);
			}

			return true;
		}
	}

	cocos2d::log("Did not collide");

	return false;
}

bool Level::spawnLabels(const Vec2& origin, const Size& winSize)
{
	// Create Title Label
	_titleLabel = Label::createWithTTF("Breakout Demo", "fonts/Marker Felt.ttf", 24);
	if (!_titleLabel)
	{
		return false;
	}

	// position the label on the center of the screen
	_titleLabel->setPosition(Vec2(origin.x + winSize.width * 0.5f,
		origin.y + winSize.height - _titleLabel->getContentSize().height));

	// Create Score Label
	_scoreLabel = Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", 24);
	if (!_scoreLabel)
	{
		return false;
	}

	// position the label on the center of the screen
	_scoreLabel->setPosition(Vec2(origin.x + winSize.width * 0.25f,
		origin.y + winSize.height - _scoreLabel->getContentSize().height - 100.0f));

	// Create Lives Label
	_livesLabel = Label::createWithTTF("Lives: 3", "fonts/Marker Felt.ttf", 24);
	if (!_livesLabel)
	{
		return false;
	}

	// position the label on the center of the screen
	_livesLabel->setPosition(Vec2(origin.x + winSize.width * 0.75f,
		origin.y + winSize.height - _livesLabel->getContentSize().height - 100.0f));

	// Add labels as children
	this->addChild(_titleLabel, 1);
	this->addChild(_scoreLabel, 1);
	this->addChild(_livesLabel, 1);

	return true;
}

bool Level::spawnBlocks() {
	if (!_blocks->spawnBlocks())
	{
		return false;
	}

	for (const std::vector<GameBlock*> row : _blocks->getBlocks())
	{
		for (GameBlock* block : row)
		{
			this->addChild(block);
		}
	}

	return true;
}

void Level::checkBallModifiers(GameBlock* block)
{
	if (!_redHit || !_orangeHit)
	{
		if (dynamic_cast<RedBlock*>(block))
		{
			_redHit = true;
		}

		if (dynamic_cast<OrangeBlock*>(block))
		{
			_orangeHit = true;
		}

		if (_redHit && _orangeHit)
		{
			_ball->increaseSpeed();
		}
	}

	if (!_firstBoost)
	{
		if (_blockCollisions == 4)
		{
			_firstBoost = _ball->increaseSpeed();
		}
	}

	if (!_secondBoost) {
		if (_blockCollisions == 12)
		{
			_secondBoost = _ball->increaseSpeed();
		}
	}
}

void Level::checkPlayerModifiers()
{
	if (!_halvedPlayer)
	{
		if (_redHit && _ball->hasTouchedTop())
		{
			_halvedPlayer = _player->half();
		}
	}
}

void Level::resetModifiers()
{
	_firstBoost = false;
	_secondBoost = false;
	_orangeHit = false;
	_redHit = false;
	_halvedPlayer = false;

	_player->setScale(1.0f);
}

void Level::updateLabelText(Label* label, std::string text, int value)
{
	text += std::to_string(value);
	label->setString(text);
}
