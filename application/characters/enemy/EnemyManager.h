#pragma once
#include "Enemy.h"
#include "Field.h"
#include "Player.h"

class EnemyManager
{
public:
	void Initialize(Object3DCommon* objectCommon, const std::string& filename, int enemyCount);
	void Update();
	void Draw();
	void SetField(Field* field) { field_ = field; }
	void SetPlayer(Player* player) { player_ = player; }

private:
	std::vector<std::unique_ptr<Enemy>> enemies_;
	Field* field_ = nullptr;
	Player* player_ = nullptr;
};

