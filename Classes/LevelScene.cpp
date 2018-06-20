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
	auto scene = Scene::create();
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

	start = false;

	// Draw Background
	auto background = DrawNode::create();
	background->drawSolidRect(origin, winSize, Color4F(0.6f, 0.6f, 0.6f, 1.0f));
	this->addChild(background);

	// Spawn player
	_player = Player::create();
	_player->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.2f));
	this->addChild(_player);

	// Spawn Collidable Blocks
	spawnBlocks();

	// Check Ball Exists. 
	// If not, deduct life and spawn next ball if lives remain

	// Add Event Listeners
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseUp = CC_CALLBACK_1(Level::onMouseUp, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

	// Add Collision Handlers
	this->scheduleUpdate();

    return true;
}

void Level::onMouseUp(cocos2d::Event* event) {
	// If we have already performed the startup, do not execute.
	if (start)
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
		start = true;

		auto origin = Director::getInstance()->getVisibleOrigin();
		auto winSize = Director::getInstance()->getVisibleSize();

		// Spawn Wrecking Ball
		_ball = Ball::create();
		_ball->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.4f));
		this->addChild(_ball);
	}
}

bool Level::spawnBlocks() {
	// Initialise Block Container
	_blocks = std::vector<std::vector<GameBlock*>>(8);

	// Set starting position for grid iteration
	float startingX = 125.0f;
	float startingY = 800.0f;

	// Fill Block Container with blocks
	for (int i = 0; i < 8; i++) {
		std::vector<GameBlock*> row(14);

		for (int j = 0; j < 14; j++) {
			Vec2 location = Vec2(startingX + (60.0f * j), startingY + (-20.0f * i));

			GameBlock* block;

			if (i < 2) {
				block = RedBlock::create();
			}
			else if (i >= 2 && i < 4) {
				block = OrangeBlock::create();
			}
			else if (i >= 2 && i < 6) {
				block = GreenBlock::create();
			}
			else {
				block = YellowBlock::create();
			}

			if (!block)
			{
				return false;
			}

			block->setPosition(location);

			this->addChild(block);
			row.push_back(block);
		}

		_blocks.push_back(row);
	}

	if (!_blocks.size())
	{
		return false;
	}

	return true;
}

bool Level::checkBall() {
	return true;
}
