#pragma once
#include "Enemy.h"
#include "Field.h"
#include "Player.h"

class EnemyManager
{
public:
	void Initialize(const std::string& filename, int enemyCount = 3);
	void Update();
	void UpdateTransform();
	void UpdateImGui();
	void Draw();
	Enemy* SelectEnemyToMove();
	bool IsAdjacent(const Vector3& pos1, const Vector3& pos2);

public: //アクセッサ
	void SetField(Field* field) { field_ = field; }
	void SetPlayer(const std::vector<std::unique_ptr<Player>>& player);
	void SetPlayer(std::vector<Player*> player) { playerList_ = player; }
	void SetEnemyTurn(bool isEnemyTurn);
	bool IsEnemyTurn() const { return isEnemyTurn_; }
	const std::vector<std::unique_ptr<Enemy>>& GetEnemies() { return enemies_; }

private:
	//敵のリスト
	std::vector<std::unique_ptr<Enemy>> enemies_;
	//フィールドのポインタ
	Field* field_ = nullptr;
	//プレイヤーのポインタ
	std::vector<Player*> playerList_;
	//今の敵のターンかどうか
	bool isEnemyTurn_ = false;
	//選択された敵
	Enemy* currentEnemy_ = nullptr;
};

