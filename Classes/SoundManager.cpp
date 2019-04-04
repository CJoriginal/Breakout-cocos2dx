#include "SoundManager.h"
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;

USING_NS_CC;

void SoundManager::init(float dt)
{
	// Preload Common Sound Effects
	AudioEngine::preload(PLAYER_COLLISION_SFX);
	AudioEngine::preload(NORMAL_COLLISION_SFX);
}

void SoundManager::playPlayerSound(float dt)
{
	AudioEngine::play2d(PLAYER_COLLISION_SFX);
}

void SoundManager::playCollisionSound(float dt)
{
	AudioEngine::play2d(NORMAL_COLLISION_SFX);
}

void SoundManager::playDeathSound(float dt)
{
	AudioEngine::play2d(PLAYER_DEATH_SFX);
}

void SoundManager::playFailureSound(float dt)
{
	AudioEngine::play2d(FAIL_SFX);
}

void SoundManager::playWinSound(float dt)
{
	AudioEngine::play2d(WIN_SFX);
}
