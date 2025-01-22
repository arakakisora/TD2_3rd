#include "Enemy.h"
#include <algorithm>

void Enemy::Initialize(Object3DCommon* objectCommon, const std::string& filename)
{
	// 3Dオブジェクトの初期化
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize(objectCommon);
	object3D_->SetModel(filename);
}

void Enemy::Update()
{
#ifdef _DEBUG
	ImGui::Begin("enemy");
	ImGui::DragFloat3("position", &transform_.translate.x, 0.1f);
	ImGui::DragFloat3("scale", &transform_.scale.x, 0.1f);
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);
	ImGui::End();
#endif

	// 入力処理
	HandleInput();

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
	// 移動中は移動を受け付けない
    if (isEaseStart_) { return; }

	Vector3 newPosition = transform_.translate + distance;

    // マップの範囲内か確認
    if (newPosition.x >= 0 && newPosition.x < WIDTH && newPosition.z>= 0 && newPosition.z < DEPTH)
    {
        // 進む先のブロックのtypeが0なら進む
        if (field_->GetBlockType((int)newPosition.x, 0, (int)newPosition.z) == 0)
        {
        	// 現在の位置を記録
            moveStartPosition_ = transform_.translate;
			// 目標位置を設定
            moveTargetPosition_ = newPosition;
			// 移動開始
            moveProgress_ = 0.0f;
			isEaseStart_ = true;
        }
    }
}

void Enemy::ImGui()
{
	ImGui::Text("Enemy Position %.2f %.2f %.2f", transform_.translate.x, transform_.translate.y, transform_.translate.z);
}

void Enemy::HandleInput()
{
	// 移動ベクトルの初期化
    Vector3 distance{ 0.0f, 0.0f, 0.0f };

    // キー入力の確認
    if (Input::GetInstans()->TriggerKey(DIK_W))
    {
        distance.z -= 1.0f;
    }
    if (Input::GetInstans()->TriggerKey(DIK_S))
    {
        distance.z += 1.0f;
    }
    if (Input::GetInstans()->TriggerKey(DIK_A))
    {
        distance.x -= 1.0f;
    }
    if (Input::GetInstans()->TriggerKey(DIK_D))
    {
        distance.x += 1.0f;
    }

    // 移動がある場合のみ実行
    if (distance.x != 0.0f || distance.z != 0.0f)
    {
        Move(distance);
    }
}

void Enemy::UpdateEasingMovement()
{
    if (isEaseStart_) {
        // 移動中の場合、イージングを適用して現在の位置を更新
        moveProgress_ += 1.0f / 60.0f * 2.0f;
        moveProgress_ = (std::min)(moveProgress_, 1.0f);
        transform_.translate = moveStartPosition_ + (moveTargetPosition_ - moveStartPosition_) * EaseInSine(moveProgress_);
        if (moveProgress_ >= 1.0f) {
            isEaseStart_ = false;

			isTurnEnd_ = true; // ターンエンド 追加
        }
    }
}

void Enemy::UpdateTransform()
{
	object3D_->SetTranslate(transform_.translate);
	object3D_->SetRotate(transform_.rotate);
	object3D_->SetScale(transform_.scale);
	object3D_->Update();
}
