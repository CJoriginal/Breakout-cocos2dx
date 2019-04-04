#include "SoundManager.h"
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;

USING_NS_CC;

void SoundManager::Init(float dt)
{
	// Preload Common Sound Effects
	AudioEngine::preload(PLAYER_COLLISION_SFX);
	AudioEngine::preload(NORMAL_COLLISION_SFX);
}

void SoundManager::PlayPlayerSound(float dt)
{
	AudioEngine::play2d(PLAYER_COLLISION_SFX);
}

void SoundManager::PlayCollisionSound(float dt)
{
	AudioEngine::play2d(NORMAL_COLLISION_SFX);
}

void SoundManager::PlayDeathSound(float dt)
{
	AudioEngine::play2d(PLAYER_DEATH_SFX);
}

void SoundManager::PlayFailureSound(float dt)
{
	AudioEngine::play2d(FAIL_SFX);
}

void SoundManager::PlayWinSound(float dt)
{
	AudioEngine::play2d(WIN_SFX);
}
