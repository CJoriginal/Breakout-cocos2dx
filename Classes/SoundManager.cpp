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
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->preloadEffect(playerSound.c_str());
	audio->preloadEffect(collisionSound.c_str());
	audio->preloadEffect(deathSound.c_str());
	audio->preloadEffect(failureSound.c_str());
	audio->preloadEffect(winSound.c_str());

	return true;
}

bool SoundManager::PlayPlayerSound()
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect(playerSound.c_str());

	return true;
}

bool SoundManager::PlayCollisionSound()
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect(collisionSound.c_str());

	return true;
}

bool SoundManager::PlayDeathSound()
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect(deathSound.c_str());

	return true;
}

bool SoundManager::PlayFailureSound()
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect(failureSound.c_str());

	return true;
}

bool SoundManager::PlayWinSound()
{
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect(winSound.c_str());

	return true;
}
