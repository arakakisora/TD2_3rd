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
	// 3Dオブジェクトの描画
	object3D_->Draw();
}

void Enemy::Move(Vector3 distance)
{
	// 移動中またはターン終了中は移動しない
    if (isEaseStart_ || isTurnEnd_) { return; }

    //移動先
	Vector3 newPosition = transform_.translate + distance;

    // マップの範囲内か確認
    if ((newPosition.x >= 0 && newPosition.x < WIDTH) && (newPosition.z>= 0 && newPosition.z < DEPTH))
    {
		//プレイヤーがいる場所には移動しない
        if (newPosition.x == player_->GetPosition().x && newPosition.z == player_->GetPosition().z)
        {
			//斜め移動処理のためのフラグ
            bool isPlayerMove = false;
            //移動方向がもし斜めだったらどちらかの成分を０にする
            if (distance.x != 0.0f && distance.z != 0.0f)
            {
                if (rand() % 2 == 0)
                {
					newPosition.z -= distance.z;
                    distance.x = 0.0f;
                } else
                {
					newPosition.x -= distance.x;
                    distance.z = 0.0f;
                }
                isPlayerMove = true;
            }
			//プレイヤーがいる場所には移動しない
            if (isPlayerMove)
            {
                //ターン終了
                isTurnEnd_ = true;
                return;
            }
        }

        // 現在の位置を記録
        moveStartPosition_ = transform_.translate;
		// 目標位置を設定
        moveTargetPosition_ = newPosition;
		// 移動開始
        moveProgress_ = 0.0f;
		// 移動開始フラグを立てる
		isEaseStart_ = true;
    }
}

void Enemy::HandleAI()
{
    if (!player_) return; // プレイヤー情報がない場合はスキップ
    if (isEaseStart_) return; // 移動中はスキップ

    // プレイヤーと自分の位置を取得
    Vector3 playerPos = player_->GetPosition();
    Vector3 enemyPos = GetPosition();

    // 他の敵の位置を取得（自分以外）
    std::vector<Vector3> otherEnemyPositions;
    for (auto& otherEnemy : enemyManager_->GetEnemies()) {
        if (otherEnemy.get() != this) {
            otherEnemyPositions.push_back(otherEnemy->GetPosition());
        }
    }

    // プレイヤーの隣接マスにいる敵の方向を調べる
    Vector3 moveDir = { 0.0f, 0.0f, 0.0f };
    bool foundAdjacentEnemy = false;

    for (const auto& pos : otherEnemyPositions) {
        if (enemyManager_->IsAdjacent(pos, playerPos)) {
            Vector3 direction = pos - playerPos;
            // 反対側の方向を設定
            if (direction.x == 1.0f) {
                moveDir = { playerPos.x - 1.0f, 0.0f, playerPos.z }; // 敵がプレイヤーの右側にいる場合、左に移動
            } else if (direction.x == -1.0f) {
                moveDir = { playerPos.x + 1.0f, 0.0f, playerPos.z }; // 敵がプレイヤーの左側にいる場合、右に移動
            } else if (direction.z == -1.0f) {
                moveDir = { playerPos.x, 0.0f, playerPos.z - 1.0f }; // 敵がプレイヤーの上側にいる場合、下に移動
            } else if (direction.z == 1.0f) {
                moveDir = { playerPos.x, 0.0f, playerPos.z + 1.0f }; // 敵がプレイヤーの下側にいる場合、上に移動
            }
			//もしフィールドの範囲外だったら範囲内に収める
			if (moveDir.x < 0) moveDir.x = 0;
			if (moveDir.x >= WIDTH - 1) moveDir.x = WIDTH - 1;
			if (moveDir.z < 0) moveDir.z = 0;
			if (moveDir.z >= DEPTH - 1) moveDir.z = DEPTH - 1;

            foundAdjacentEnemy = true;
            break; // 最初の隣接する敵のみを考慮
        }
    }

    if (!foundAdjacentEnemy) {
        // 隣接する敵がいない場合、プレイヤーに近づく
        Vector3 direction = playerPos - enemyPos;

        // 縦または横の移動を優先
        if (std::abs(direction.x) > std::abs(direction.z)) {
            moveDir.x = (direction.x > 0) ? 1.0f : -1.0f;
        } else {
            moveDir.z = (direction.z > 0) ? 1.0f : -1.0f;
        }
    }

    // 移動指示
    if (moveDir.x != 0.0f || moveDir.z != 0.0f)
    {
        Move(moveDir);
    } else {
        isTurnEnd_ = true;
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
