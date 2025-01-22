#pragma once

#include <string>

class GamePlayScene;

class BaseTurnState
{
public:

	BaseTurnState(const std::string& _scene, GamePlayScene* _pPlayScene) :scene_(_scene), pPlayScene_(_pPlayScene) {};
	virtual ~BaseTurnState();

	virtual void Update() = 0;

protected:

	std::string scene_;
	GamePlayScene* pPlayScene_ = nullptr;
};

