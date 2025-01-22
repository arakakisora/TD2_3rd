#pragma once

#include "BaseTurnState.h"

class EnemyTurn : public BaseTurnState
{
public:
	
	EnemyTurn(GamePlayScene* _pScene);

	void Update();
};

