#pragma once
#include <vector>
#include "Player.h"

class PlayerManeger
{
public:
	PlayerManeger();
	~PlayerManeger();
	void Initialize(Ball*ball);
	void Finalize();
	void Update();
	void Draw();
private:

	std::vector<std::unique_ptr<Player>> pPlayer_;

	Ball* ball_;
};

