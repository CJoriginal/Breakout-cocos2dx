#include "SoundManager.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

SoundManager::SoundManager()
{
	// Set audio paths
	playerSound = "PlayerSound.mp3";
	collisionSound = "CollisionSound.mp3";
	deathSound = "DeathSound.mp3";
	failureSound = "FailureSound.mp3";
	winSound = "WinSound.mp3";

	if (!PreloadSound())
	{
		log("SoundManager::PreloadSound: Could not preload sound effects - check files");
		return;
	}
}


SoundManager::~SoundManager()
{
}

bool SoundManager::PreloadSound()
{
	// Preload Sound Effects
	CocosDenshion::SimpleAudioEngine* audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadEffect(playerSound.c_str());
	audio->preloadEffect(collisionSound.c_str());
	audio->preloadEffect(deathSound.c_str());
	audio->preloadEffect(failureSound.c_str());
	audio->preloadEffect(winSound.c_str());

	return true;
}

bool SoundManager::PlayPlayerSound()
{
	soundsToPlay.push_back(playerSound.c_str());
	return true;
}

bool SoundManager::PlayCollisionSound()
{
	soundsToPlay.push_back(collisionSound.c_str());
	return true;
}

bool SoundManager::PlayDeathSound()
{
	soundsToPlay.push_back(deathSound.c_str());
	return true;
}

bool SoundManager::PlayFailureSound()
{
	soundsToPlay.push_back(failureSound.c_str());
	return true;
}

bool SoundManager::PlayWinSound()
{
	soundsToPlay.push_back(winSound.c_str());
	return true;
}

void SoundManager::PlayEffects(float dt)
{
	if (!soundsToPlay.size())
	{
		return;
	}


	CocosDenshion::SimpleAudioEngine* audio = CocosDenshion::SimpleAudioEngine::getInstance();
	while (soundsToPlay.size())
	{
		audio->playEffect(soundsToPlay.back().c_str());
		soundsToPlay.pop_back();
	}
}