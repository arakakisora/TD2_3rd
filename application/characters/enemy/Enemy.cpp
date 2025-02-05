#include "Enemy.h"
#include <algorithm>
#include <queue>
#include <set>

#include "EnemyManager.h"

float EaseInSine(float x) { return 1.0f - std::cos((x * std::numbers::pi_v<float>) / 2.0f); }

template<class T> T EasingToEnd(T start, T end, T(*func)(T), T progress) { return start + func(progress) * (end - start); }


void Enemy::Initialize(Object3DCommon* objectCommon, const std::string& filename)
{
	// 3Dオブジェクトの初期化
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize(objectCommon);
	object3D_->SetModel(filename);
	object3D_->SetLighting(true);
}

void Enemy::Update()
{
	// AI処理
    HandleAI();

	// 移動処理
	UpdateEasingMovement();

	// 3Dオブジェクトの更新
	UpdateTransform();
}

void Enemy::Draw()
{
	object3D_->Draw();
}

void Enemy::Move(int dx, int dz)
{
    // 移動中またはターン終了中は移動しない
    if (isEaseStart_ || isTurnEnd_) { return; }

    // 移動先の座標を計算
    int newX = static_cast<int>(transform_.translate.x) + dx;
    int newZ = static_cast<int>(transform_.translate.z) + dz;

    // マップの範囲内か確認（WIDTHとDEPTHはフィールドのサイズ）
    if (newX >= 0 && newX < WIDTH && newZ >= 0 && newZ < DEPTH) 
    {
        // 他の敵やプレイヤーがいないか確認
        bool blocked = false;

        // プレイヤーの位置と比較
        for (const auto& player : playerList_)
        {
            int playerX = static_cast<int>(player->GetPosition().x);
            int playerZ = static_cast<int>(player->GetPosition().z);
            if (newX == playerX && newZ == playerZ) 
            {
                blocked = true;
                break;
            }
        }

        // 他の敵の位置と比較
        for (const auto& otherEnemy : enemyManager_->GetEnemies()) 
        {
            if (otherEnemy.get() != this)
            {
                int enemyX = static_cast<int>(otherEnemy->GetPosition().x);
                int enemyZ = static_cast<int>(otherEnemy->GetPosition().z);
                if (newX == enemyX && newZ == enemyZ) {
                    blocked = true;
                    break;
                }
            }
        }

        if (!blocked) 
        {
            // 移動開始位置と目標位置を設定
            moveStartPosition_ = transform_.translate;
            moveTargetPosition_ = { static_cast<float>(newX), -0.5f, static_cast<float>(newZ) };

            // 移動開始
            moveProgress_ = 0.0f;
            isEaseStart_ = true;
        }
        else 
        {
            // 移動できない場合はターン終了
            isTurnEnd_ = true;
        }
    }
    else 
    {
        // マップ外の場合はターン終了
        isTurnEnd_ = true;
    }
}

void Enemy::HandleAI()
{
    if (isEaseStart_) return; // 移動中はスキップ

    // 敵の整数座標を取得
    int enemyX = static_cast<int>(transform_.translate.x);
    int enemyZ = static_cast<int>(transform_.translate.z);

	// ボールを持っているプレイヤーを取得
    Player* targetPlayer = nullptr;
    float minDistance = (std::numeric_limits<float>::max)();
    for (const auto& player : playerList_) 
    {
        if (player->HasBall()) 
        {
            targetPlayer = player;
        }
    }

    if (!targetPlayer) return; // プレイヤーが見つからない場合はスキップ

    // プレイヤーの整数座標を取得
    int playerX = static_cast<int>(targetPlayer->GetPosition().x);
    int playerZ = static_cast<int>(targetPlayer->GetPosition().z);

    // 他のプレイヤーの位置を取得（ターゲットプレイヤーを除く）
    std::set<std::pair<int, int>> occupiedPositions;
    for (const auto& player : playerList_) 
    {
        if (player != targetPlayer) 
        {
            int otherX = static_cast<int>(player->GetPosition().x);
            int otherZ = static_cast<int>(player->GetPosition().z);
            occupiedPositions.emplace(otherX, otherZ);
        }
    }

    // 他の敵の位置を取得（自分以外）
    for (const auto& otherEnemy : enemyManager_->GetEnemies()) 
    {
        if (otherEnemy.get() != this) 
        {
            int otherX = static_cast<int>(otherEnemy->GetPosition().x);
            int otherZ = static_cast<int>(otherEnemy->GetPosition().z);
            occupiedPositions.emplace(otherX, otherZ);
        }
    }

    // 幅優先探索(BFS)の初期化
    std::queue<std::pair<int, int>> queue;
    std::map<std::pair<int, int>, std::pair<int, int>> cameFrom;
    std::pair<int, int> startPos = { enemyX, enemyZ };
    std::pair<int, int> targetPos = { playerX, playerZ };

    queue.push(startPos);
    cameFrom[startPos] = { -1, -1 }; // 始点

    bool pathFound = false;

    while (!queue.empty())
    {
        auto current = queue.front();
        queue.pop();

        // 目標地点に到達した場合
        if (current == targetPos) 
        {
            pathFound = true;
            break;
        }

        // 移動可能な方向（右、左、下、上）
        std::vector<std::pair<int, int>> directions = {
            { 1, 0 },
            { -1, 0 },
            { 0, 1 },
            { 0, -1 }
        };

        for (const auto& dir : directions) 
        {
            int nextX = current.first + dir.first;
            int nextZ = current.second + dir.second;
            std::pair<int, int> nextPos = { nextX, nextZ };

            // マップ範囲内か確認
            if (nextX >= 0 && nextX < WIDTH && nextZ >= 0 && nextZ < DEPTH) 
            {
                // すでに訪問済みでないことを確認
                if (cameFrom.find(nextPos) == cameFrom.end()) 
                {
                    // 次の位置がターゲットの位置か、占有されていないかを確認
                    if (nextPos == targetPos || occupiedPositions.find(nextPos) == occupiedPositions.end())
                    {
                        queue.push(nextPos);
                        cameFrom[nextPos] = current;
                    }
                }
            }
        }
    }

    if (pathFound)
    {
        // 経路を復元
        std::vector<std::pair<int, int>> path;
        auto current = targetPos;
        while (current != startPos) 
        {
            path.push_back(current);
            current = cameFrom[current];
        }
        std::reverse(path.begin(), path.end());

        if (!path.empty()) 
        {
            // 次に移動する位置を取得
            int nextX = path.front().first;
            int nextZ = path.front().second;
            int dx = nextX - enemyX;
            int dz = nextZ - enemyZ;

            // 移動を試みる
            Move(dx, dz);
        }
        else 
        {
            // 経路が存在するが移動できない場合はターン終了
            isTurnEnd_ = true;
        }
    }
    else 
    {
        // 経路が見つからない場合はターン終了
        isTurnEnd_ = true;
    }
}
void Enemy::UpdateEasingMovement()
{
    if (isEaseStart_) 
    {
        // 移動中の場合、イージングを適用して現在の位置を更新
        moveProgress_ += 1.0f / 60.0f * 2.0f;
        moveProgress_ = (std::min)(moveProgress_, 1.0f);
		transform_.translate.x = EasingToEnd(moveStartPosition_.x, moveTargetPosition_.x, EaseInSine, moveProgress_);
        transform_.translate.y = -0.5f;
		transform_.translate.z = EasingToEnd(moveStartPosition_.z, moveTargetPosition_.z, EaseInSine, moveProgress_);
    	if (moveProgress_ >= 1.0f) 
        {
			// 移動完了
    		isEaseStart_ = false;
            //ターン終了
			isTurnEnd_ = true;
        }
    }
}

void Enemy::ImGui()
{
#ifdef _DEBUG
    ImGui::Begin("enemy");
    ImGui::DragFloat3("position", &transform_.translate.x, 0.1f);
    ImGui::DragFloat3("scale", &transform_.scale.x, 0.1f);
    ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);
    ImGui::End();
#endif
}

void Enemy::UpdateTransform()
{
    transform_.translate.y = -0.5f;
	object3D_->SetTranslate(transform_.translate);
	object3D_->SetRotate(transform_.rotate);
	object3D_->SetScale(transform_.scale);
	object3D_->Update();
}
