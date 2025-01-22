#pragma once

#include "BaseTurnState.h"

class PlayerTurn : public BaseTurnState
{
public:

	PlayerTurn(GamePlayScene* _pScene);
	
	void Update();
};

