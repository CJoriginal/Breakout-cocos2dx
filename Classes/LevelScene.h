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

#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__

#include "cocos2d.h"

class Player;
class Ball;
class GameBlock;
class BlockManager;
class SoundManager;

class Level : public cocos2d::Scene {
private:
	Player* _player;
	Ball* _ball;
	BlockManager* _blocks;
	SoundManager* _sound;

	cocos2d::Label* _scoreLabel;
	cocos2d::Label* _livesLabel;
	cocos2d::Label* _titleLabel;

	cocos2d::Label* _resultLabel;
	cocos2d::Label* _startLabel;

	int _score;
	int _lives;
	int _blockCollisions;

	bool _start;
	bool _reset;
	bool _isFirstScreen;
	bool _firstBoost;
	bool _secondBoost;
	bool _orangeHit;
	bool _redHit;
	bool _halvedPlayer;
	bool _blockHit;
	bool spawnLabels(const cocos2d::Vec2& origin, const cocos2d::Size& winSize);

	// Scene Utility Functions
	bool spawnBlocks();

	void checkBallModifiers(int tag);
	void checkPlayerModifiers();
	void resetModifiers();

	void updateLabelText(cocos2d::Label* label, std::string text, int value);
	void displayResultLabels(bool didWin);

protected:
	virtual bool init();
	void onMouseUp(cocos2d::Event *event);
	bool onContactBegin(cocos2d::PhysicsContact &contact);
	void update(float dt) override;

public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(Level);
};

#endif // __LEVEL_SCENE_H__
