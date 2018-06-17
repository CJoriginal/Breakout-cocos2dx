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
	//All = (PhysicsCategory::Player | PhysicsCategory::Block) // 3
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

	// Draw Background
	auto background = DrawNode::create();
	background->drawSolidRect(origin, winSize, Color4F(0.6f, 0.6f, 0.6f, 1.0f));
	this->addChild(background);

	// Spawn player
	_player = Player::create();
	_player->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.2f));
	this->addChild(_player);

	// Fill Block Locations
	std::vector<Vec2> blockLocations = {
		// First Row
		Vec2(200.0f, -100.0f),
		Vec2(350.0f, -100.0f),
		Vec2(500.0f, -100.0f),
		Vec2(650.0f, -100.0f),
		Vec2(800.0f, -100.0f),
		// Second Row
		Vec2(275.0f, -150.0f),
		Vec2(425.0f, -150.0f),
		Vec2(575.0f, -150.0f),
		Vec2(725.0f, -150.0f),
		// Third Row
		Vec2(350.0f, -200.0f),
		Vec2(500.0f, -200.0f),
		Vec2(650.0f, -200.0f),
		// Fourth Row
		Vec2(425.0f, -250.0f),
		Vec2(575.0f, -250.0f),
		// Sixth Row
		Vec2(500.0f, -300.0f),
	};

	// Spawn Collidable Blocks
	spawnBlocks(blockLocations);

	// Spawn Wrecking Ball
	_ball = Ball::create();
	_ball->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.4f));
	this->addChild(_ball);

	// Check Ball Exists. 
	// If not, deduct life and spawn next ball if lives remain

	// Add Event Listeners

	// Add Collision Handlers

    return true;
}

bool Level::spawnBlocks(const std::vector<Vec2>& locations) {
	if (!locations.size())
	{
		return false;
	}

	auto winSize = Director::getInstance()->getVisibleSize();
	const Vec2 top = Vec2(0.0f, winSize.height);

	for (const Vec2& location : locations) {
		auto block = GameBlock::createScoreBlock(top + location);
		this->addChild(block);
		_blocks.push_back(block);
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
