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
	_player = Sprite::create("player.png");
	_player->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.2f));
	this->addChild(_player);

	// Spawn Collidable Blocks
	spawnBlocks(5, Vec2(winSize.width * 0.2f, winSize.height * 0.2f), Vec2(winSize.width * 0.8f, winSize.height * 0.8f));

	// Spawn Wrecking Ball
	_ball = Ball::create();
	_ball->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.2f));
	this->addChild(_ball);

	// Check Ball Exists. 
	// If not, deduct life and spawn next ball if lives remain

	// Add Event Listeners

	// Add Collision Handlers

    return true;
}

bool Level::spawnBlocks(const int num, const Vec2& min, const Vec2& max) {
	for (int i = 0; i < num; i++) {
		auto block = GameBlock::create();
		auto blockSize = block->getContentSize();

		auto randomX = (rand() % (int)(max.x - min.x)) + min.x;
		auto randomY = (rand() % (int)(max.y - min.y)) + min.y;

		block->setPosition(Vec2(randomX, randomY));
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
