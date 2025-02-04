#pragma once
#include "Enemy.h"
#include "Field.h"
#include "Player.h"

class EnemyManager
{
public:
	//初期化
	void Initialize(const std::string& filename, int enemyCount = 3);
	//更新
	void Update();
	//トランスフォーム更新
	void UpdateTransform();
	//ImGui
	void UpdateImGui();
	//描画
	void Draw();
	//使う敵を選ぶ
	Enemy* SelectEnemyToMove();
	//隣接判定
	bool IsAdjacent(const Vector3& pos1, const Vector3& pos2);
	//挟み込み判定
	bool IsSandwiching() const { return isSandwiching_; }

public: //アクセッサ
	void SetField(Field* field) { field_ = field; }
	void SetPlayer(const std::vector<std::unique_ptr<Player>>& player);
	void SetPlayer(std::vector<Player*> player) { playerList_ = player; }
	void SetEnemyTurn(bool isEnemyTurn);
	bool IsEnemyTurn() const { return isEnemyTurn_; }
	const std::vector<std::unique_ptr<Enemy>>& GetEnemies() { return enemies_; }
	//挟み込み判定
	void CheckSandwiching();

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
	//ボールを持ったプレイヤーが挟みこまれているか
	bool isSandwiching_ = false;
};

