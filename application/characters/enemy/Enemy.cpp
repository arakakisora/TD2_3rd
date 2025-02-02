#include "Enemy.h"
#include <algorithm>
#include "EnemyManager.h"

float EaseInSine(float x) { return 1.0f - std::cos((x * std::numbers::pi_v<float>) / 2.0f); }

template<class T> T EasingToEnd(T start, T end, T(*func)(T), T progress) { return start + func(progress) * (end - start); }


void Enemy::Initialize(Object3DCommon* objectCommon, const std::string& filename)
{
	// 3Dオブジェクトの初期化
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize(objectCommon);
	object3D_->SetModel(filename);
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
    if (newX >= 0 && newX < WIDTH && newZ >= 0 && newZ < DEPTH) {
        // 他の敵やプレイヤーがいないか確認
        bool blocked = false;

        // プレイヤーの位置と比較
        int playerX = static_cast<int>(player_->GetPosition().x);
        int playerZ = static_cast<int>(player_->GetPosition().z);
        if (newX == playerX && newZ == playerZ) {
            blocked = true;
        }

        // 他の敵の位置と比較
        for (const auto& otherEnemy : enemyManager_->GetEnemies()) {
            if (otherEnemy.get() != this) {
                int enemyX = static_cast<int>(otherEnemy->GetPosition().x);
                int enemyZ = static_cast<int>(otherEnemy->GetPosition().z);
                if (newX == enemyX && newZ == enemyZ) {
                    blocked = true;
                    break;
                }
            }
        }

        if (!blocked) {
            // 移動開始位置と目標位置を設定
            moveStartPosition_ = transform_.translate;
            moveTargetPosition_ = { static_cast<float>(newX), 0.0f, static_cast<float>(newZ) };

            // 移動開始
            moveProgress_ = 0.0f;
            isEaseStart_ = true;
        } else {
            // 移動できない場合はターン終了
            isTurnEnd_ = true;
        }
    } else {
        // マップ外の場合はターン終了
        isTurnEnd_ = true;
    }
}

void Enemy::HandleAI()
{
    if (!player_) return; // プレイヤー情報がない場合はスキップ
    if (isEaseStart_) return; // 移動中はスキップ

    // 敵とプレイヤーの整数座標を取得
    int enemyX = static_cast<int>(transform_.translate.x);
    int enemyZ = static_cast<int>(transform_.translate.z);
    int playerX = static_cast<int>(player_->GetPosition().x);
    int playerZ = static_cast<int>(player_->GetPosition().z);

    // 隣接している敵がいるか確認
    bool adjacentEnemyFound = false;
    int dx = 0, dz = 0;

    for (const auto& otherEnemy : enemyManager_->GetEnemies()) {
        if (otherEnemy.get() != this) {
            int otherX = static_cast<int>(otherEnemy->GetPosition().x);
            int otherZ = static_cast<int>(otherEnemy->GetPosition().z);

            // プレイヤーに隣接している敵を探す
            if (abs(otherX - playerX) + abs(otherZ - playerZ) == 1) {
                // その反対側に移動する
                dx = playerX - otherX;
                dz = playerZ - otherZ;

                // 反対側の位置を計算
                int targetX = playerX + dx;
                int targetZ = playerZ + dz;

                // マップの範囲内か確認
                if (targetX >= 0 && targetX < WIDTH && targetZ >= 0 && targetZ < DEPTH) {
                    // 他の敵やプレイヤーがいないか確認
                    bool blocked = false;

                    // プレイヤーの位置と比較
                    if (targetX == playerX && targetZ == playerZ) {
                        blocked = true;
                    }

                    // 他の敵の位置と比較
                    for (const auto& checkEnemy : enemyManager_->GetEnemies()) {
                        if (checkEnemy.get() != this) {
                            int checkX = static_cast<int>(checkEnemy->GetPosition().x);
                            int checkZ = static_cast<int>(checkEnemy->GetPosition().z);
                            if (targetX == checkX && targetZ == checkZ) {
                                blocked = true;
                                break;
                            }
                        }
                    }

                    if (!blocked) {
                        // 移動を試みる
                        Move(targetX - enemyX, targetZ - enemyZ);
                        adjacentEnemyFound = true;
                        break;
                    }
                }
            }
        }
    }

    if (!adjacentEnemyFound) {
        // プレイヤーに向かうための方向ベクトルを計算
        dx = 0;
        dz = 0;

        if (enemyX < playerX) {
            dx = 1;
        } else if (enemyX > playerX) {
            dx = -1;
        }

        if (enemyZ < playerZ) {
            dz = 1;
        } else if (enemyZ > playerZ) {
            dz = -1;
        }

        // 優先的に移動する軸を決定
        bool moved = false;

        // X軸方向に移動を試みる
        if (dx != 0) {
            Move(dx, 0);
            if (isEaseStart_) {
                moved = true;
            }
        }

        // X軸で移動できなかった場合、Z軸方向に移動を試みる
        if (!moved && dz != 0) {
            Move(0, dz);
            if (isEaseStart_) {
                moved = true;
            }
        }

        // 移動できなかった場合、ターン終了
        if (!moved) {
            isTurnEnd_ = true;
        }
    }
}


void Enemy::UpdateEasingMovement()
{
    if (isEaseStart_) {
        // 移動中の場合、イージングを適用して現在の位置を更新
        moveProgress_ += 1.0f / 60.0f * 2.0f;
        moveProgress_ = (std::min)(moveProgress_, 1.0f);
		transform_.translate.x = EasingToEnd(moveStartPosition_.x, moveTargetPosition_.x, EaseInSine, moveProgress_);
		transform_.translate.z = EasingToEnd(moveStartPosition_.z, moveTargetPosition_.z, EaseInSine, moveProgress_);
    	if (moveProgress_ >= 1.0f) {
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
	object3D_->SetTranslate(transform_.translate);
	object3D_->SetRotate(transform_.rotate);
	object3D_->SetScale(transform_.scale);
	object3D_->Update();
}
