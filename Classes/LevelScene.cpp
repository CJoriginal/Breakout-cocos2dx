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
#include "Player.h"
#include "Ball.h"
#include "GameBlock.h"
#include "BlockManager.h"
#include "SoundManager.h"

USING_NS_CC;

Scene* Level::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();

	// Disable Gravity
	scene->getPhysicsWorld()->setGravity(Vec2(0.0f, 0.0f));

	// Display Debug Drawing for Physic Objects
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);

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
	_reset = false;
	_isFirstScreen = true;
	_firstBoost = false;
	_secondBoost = false;
	_orangeHit = false;
	_redHit = false;
	_halvedPlayer = false;
	_blockHit = false;

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

	// Initialise Sound Manager
	scheduleOnce(schedule_selector(SoundManager::init), 0.1f);

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
		// If ball has left the game, deduct a life
		if (_ball->isOutOfBounds())
		{
			_lives -= 1;
			updateLabelText(_livesLabel, "Lives: ", _lives);
			resetModifiers();
			
			if (_lives)
			{
				scheduleOnce(schedule_selector(SoundManager::playDeathSound), 0.1f);
				_ball->setup();
			}
		}

		// Check remaining lives left. If we are out of lives, we must stop the game and display
		// the game over screen, else reset the ball to continue
		if (!_lives)
		{
			// Display Game Over Screen
			displayResultLabels(false);
		}
		else
		{
			if (!_blocks->getSize())
			{
				// If this is the first screen, reset and prepare for the second screen
				if (_isFirstScreen)
				{
					_blocks->revealBlocks();
					_ball->setup();
					_isFirstScreen = false;
				}
				else
				{
					// Display Victory Screen
					displayResultLabels(true);
				}
			}
		}
	}
}

void Level::onMouseUp(Event* event) 
{
	// If we have already performed the startup, do not execute.
	if (_start)
	{
		return;
	}

	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);

	// Check Mouse Event is valid
	if (!mouseEvent)
	{
		log("Could not start game due to bad cast");
		return;
	}

	// If player clicks left button, start the game
	if (EventMouse::MouseButton::BUTTON_LEFT == mouseEvent->getMouseButton() || EventMouse::MouseButton::BUTTON_RIGHT == mouseEvent->getMouseButton())
	{
		// Set start variables
		_score = 0;
		_lives = 3;
		_start = true;

		// If this is another attempt, reset the scene before starting
		if (_reset)
		{
			_blocks->revealBlocks();
			updateLabelText(_scoreLabel, "Score: ", _score);
			updateLabelText(_livesLabel, "Lives: ", _lives);
			_reset = false;
		}

		// Spawn Wrecking Ball if not already created
		if (!_ball)
		{
			_ball = Ball::create();
			this->addChild(_ball);
		}
		_ball->setup();

		// Hide Labels when necessary
		if (_startLabel->isVisible())
		{
			_startLabel->setVisible(false);
		}

		if (_resultLabel->isVisible())
		{
			_resultLabel->setVisible(false);
		}
	}
}

bool Level::onContactBegin(PhysicsContact &contact)
{
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();

	// Check if the ball has collided with an object
	if ((1 == a->getCollisionBitmask() && 2 == b->getCollisionBitmask()) || (2 == a->getCollisionBitmask() && 1 == b->getCollisionBitmask()))
	{
		if (a->getTag() == 1)
		{
			// Check if the ball collided with player, if so, handle that collision
			if (b->getTag() == 0)
			{
				_ball->handlePlayerCollision(_player);

				// Play Player Sound
				scheduleOnce(schedule_selector(SoundManager::playPlayerSound), 0.1f);

				_blockHit = false;
			}

			// Check if the ball collided with a block. If so, handle collision and update the score
			// before removing the block;
			if (b->getTag() >= 2 && !_blockHit)
			{
				_ball->handleBlockCollision();
				
				//TODO Move Game Update Logic to Schedule Function
				// Update Score
				_bodiesHit.push_back(b);
				scheduleOnce(schedule_selector(Level::updateBlockHit), 0.1f);

				// Disable Block
				b->getOwner()->setVisible(false);
				b->setEnabled(false);

				// Play Sound Effect
				scheduleOnce(schedule_selector(SoundManager::playCollisionSound), 0.1f);

				_blockHit = true;
			}

			return true;
		}
	}

	return false;
}

void Level::updateBlockHit(float dt)
{
	if (!_bodiesHit.size())
	{
		return;
	}

	while (_bodiesHit.size())
	{
		const PhysicsBody* block = _bodiesHit.back();
		_bodiesHit.pop_back();

		_score += (block->getTag() - 1);

		_blockCollisions++;

		updateLabelText(_scoreLabel, "Score: ", _score);

		checkBallModifiers(block->getTag());
		checkPlayerModifiers();
	}
}

bool Level::spawnLabels(const Vec2& origin, const Size& winSize)
{
	// Create Title Label
	_titleLabel = Label::createWithTTF("Breakout Demo", "fonts/Marker Felt.ttf", 24);
	if (!_titleLabel)
	{
		return false;
	}

	_titleLabel->setPosition(Vec2(origin.x + winSize.width * 0.5f,
		origin.y + winSize.height - _titleLabel->getContentSize().height));

	// Create Score Label
	_scoreLabel = Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", 24);
	if (!_scoreLabel)
	{
		return false;
	}

	_scoreLabel->setPosition(Vec2(origin.x + winSize.width * 0.25f,
		origin.y + winSize.height - _scoreLabel->getContentSize().height - 100.0f));

	// Create Lives Label
	_livesLabel = Label::createWithTTF("Lives: 3", "fonts/Marker Felt.ttf", 24);
	if (!_livesLabel)
	{
		return false;
	}

	_livesLabel->setPosition(Vec2(origin.x + winSize.width * 0.75f,
		origin.y + winSize.height - _livesLabel->getContentSize().height - 100.0f));

	// Create Result Label
	_resultLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
	if (!_resultLabel)
	{
		return false;
	}

	_resultLabel->setPosition(Vec2(origin.x + winSize.width * 0.5f,
		origin.y + winSize.height / 3 - _resultLabel->getContentSize().height));

	_resultLabel->setVisible(false);

	// Create Start Label
	_startLabel = Label::createWithTTF("Click to start", "fonts/Marker Felt.ttf", 24);
	if (!_startLabel)
	{
		return false;
	}

	_startLabel->setPosition(Vec2(origin.x + winSize.width * 0.5f,
		origin.y + winSize.height / 3 - (_resultLabel->getContentSize().height + _startLabel->getContentSize().height)));

	// Add labels as children
	this->addChild(_titleLabel, 1);
	this->addChild(_scoreLabel, 1);
	this->addChild(_livesLabel, 1);
	this->addChild(_resultLabel, 1);
	this->addChild(_startLabel, 1);

	return true;
}

bool Level::spawnBlocks() 
{
	// Check we spawned blocks successfully
	if (!_blocks->spawnBlocks(this))
	{
		log("we have failed to spawn the blocks");
		return false;
	}

	return true;
}

void Level::checkBallModifiers(int tag)
{
	// Check Collision Modifiers
	if (!_redHit || !_orangeHit)
	{
		// If we hit an red block, mark it
		if (tag == 8)
		{
			_redHit = true;
		}

		// If we hit an orange block, mark it
		if (tag == 6)
		{
			_orangeHit = true;
		}

		// If we have hit both a red and a orange block, increase the ball's speed
		if (_redHit && _orangeHit)
		{
			_ball->increaseSpeed();
		}
	}

	// Check Block Collision Modifiers
	if (!_firstBoost)
	{
		// If we have hit 4 blocks, increase speed
		if (_blockCollisions == 4)
		{
			_firstBoost = _ball->increaseSpeed();
		}
	}

	if (!_secondBoost) {
		// if we have hit 12 blocks, increase speed
		if (_blockCollisions == 12)
		{
			_secondBoost = _ball->increaseSpeed();
		}
	}
}

void Level::checkPlayerModifiers()
{
	// Check if we have hit a red block and the top boundary.
	// If so, half the player's size
	if (!_halvedPlayer)
	{
		if (_redHit && _ball->hasTouchedTop())
		{
			_halvedPlayer = _player->scale();
		}
	}
}

void Level::resetModifiers()
{
	// Reset Modifier Flags
	_firstBoost = false;
	_secondBoost = false;
	_orangeHit = false;
	_redHit = false;
	_halvedPlayer = false;

	// Reset Player Size
	_player->scale(true);

	// Reset Collisions
	_blockCollisions = 0;
}

void Level::updateLabelText(Label* label, std::string text, int value)
{
	text += std::to_string(value);
	label->setString(text);
}

void Level::displayResultLabels(bool didWin)
{
	// Choose the result text based on the player's result
	std::string resultText;
	if (didWin)
	{
		resultText = "Congratulations! You won!";
		scheduleOnce(schedule_selector(SoundManager::playWinSound), 0.1f);
	}
	else
	{
		resultText = "Game Over";
		scheduleOnce(schedule_selector(SoundManager::playFailureSound), 0.1f);
	}

	std::string startText = "Click to play again.";

	// Set text
	_startLabel->setString(startText);
	_resultLabel->setString(resultText);

	// Set labels to visible
	_startLabel->setVisible(true);
	_resultLabel->setVisible(true);

	// Hide Ball
	_ball->setVisible(false);

	// Stop Game Logic
	_start = false;
	_reset = true;
}
