#include "EnemyManager.h"
#include <limits>

void EnemyManager::Initialize(const std::string& filename, int enemyCount)
{
	//敵の数分確保
	enemies_.resize(enemyCount);
	//敵の初期化

	for (int index = 0; index < enemyCount; index++)
	{
		enemies_[index] = std::make_unique<Enemy>();
		enemies_[index]->Initialize(Object3DCommon::GetInstance(), filename);
		enemies_[index]->SetField(field_);
		//NOTE:今は仮にプレイヤーの0番目をセット
		enemies_[index]->SetPlayer(playerList_[0]);
		enemies_[index]->SetEnemyManager(this);
		//座標をずらして配置
		enemies_[index]->SetPosition({ 5.0f , 0.0f, static_cast<float>(index) });
	}
}

void EnemyManager::Update()
{
	if (isEnemyTurn_) {
		if (currentEnemy_ == nullptr) {
			// まだ敵が選択されていない場合、選択する
			currentEnemy_ = SelectEnemyToMove();
			if (currentEnemy_) {
				// 選択した敵のターン終了フラグをリセット
				currentEnemy_->SetTurnEnd(false);
			}
		}

		if (currentEnemy_) {
			// 選択された敵のみ更新
			currentEnemy_->Update();

			if (currentEnemy_->IsTurnEnd()) {
				// 敵のターンが終了したらリセット
				currentEnemy_ = nullptr;
				isEnemyTurn_ = false; // ターン終了
			}
		}
	}
}


void EnemyManager::UpdateTransform()
{//敵のトランスフォーム更新
	for (auto& enemy : enemies_)
	{
		enemy->UpdateTransform();
		enemy->ImGui();
	}
}

void EnemyManager::UpdateImGui()
{//敵のImGui
	for (auto& enemy : enemies_)
	{
		enemy->ImGui();
	}
}

void EnemyManager::Draw()
{
	//敵の描画
	for (auto& enemy : enemies_)
	{
		enemy->Draw();
	}
}

void EnemyManager::SetPlayer(const std::vector<std::unique_ptr<Player>>& player)
{
	//プレイヤーリストをクリア
	playerList_.clear();
	//プレイヤーのポインタをセット
	for (auto& player : player)
	{
		playerList_.push_back(player.get());
	}
}

void EnemyManager::SetEnemyTurn(bool isEnemyTurn)
{
	if (isEnemyTurn) {
		// ターン開始時に全ての敵のターン終了フラグをリセット
		for (auto& enemy : enemies_) {
			enemy->SetTurnEnd(true);
		}

	}
	isEnemyTurn_ = isEnemyTurn;
	currentEnemy_ = nullptr;
}

Enemy* EnemyManager::SelectEnemyToMove()
{
	//ボールを持っているプレイヤーの位置を取得
	int playerIndex = 0;
	for (int index = 0; index < playerList_.size(); index++)
	{
		if(playerList_[index]->HasBall())
		{
			playerIndex = index;
			break;
		}
	}

	// プレイヤーの位置を取得
	Vector3 playerPos = playerList_[playerIndex]->GetPosition();

	// 挟み込みに最適な敵を選択
	Enemy* selectedEnemy = nullptr;
	float minDistance = (std::numeric_limits<float>::max)();

	for (auto& enemy : enemies_) {
		// 敵の位置を取得
		Vector3 enemyPos = enemy->GetPosition();

		// 敵がプレイヤーに隣接している場合はスキップ
		if (IsAdjacent(enemyPos, playerPos)) {
			continue;
		}

		// プレイヤーとの距離を計算
		Vector2 playerPosVec2 = Vector2(playerPos.x, playerPos.z);
		Vector2 enemyPosVec2 = Vector2(enemyPos.x, enemyPos.z);
		float distance = Vector2(playerPosVec2 - enemyPosVec2).Length();

		if (distance < minDistance) {
			minDistance = distance;
			selectedEnemy = enemy.get();
			// 選択した敵のターン終了フラグをリセット
			selectedEnemy->SetTurnEnd(false);
		}
	}
	

	// すべての敵が隣接している場合は、ターンを終了
	if (!selectedEnemy) {
		isEnemyTurn_ = false;
	}

	return selectedEnemy;
}

bool EnemyManager::IsAdjacent(const Vector3& pos1, const Vector3& pos2)
{
	float distanceX = std::abs(pos1.x - pos2.x);
	float distanceZ = std::abs(pos1.z - pos2.z);

	// 隣接しているかチェック（距離が1.0fの場合）
	return (distanceX + distanceZ == 1.0f);
}