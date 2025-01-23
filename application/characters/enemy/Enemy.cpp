#include "Enemy.h"
#include <algorithm>

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
#ifdef _DEBUG
	ImGui::Begin("enemy");
	ImGui::DragFloat3("position", &transform_.translate.x, 0.1f);
	ImGui::DragFloat3("scale", &transform_.scale.x, 0.1f);
	ImGui::DragFloat3("rotate", &transform_.rotate.x, 0.1f);
	ImGui::End();
#endif

	//
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
	// 移動中は移動を受け付けない
    if (isEaseStart_) { return; }

    //移動先
	Vector3 newPosition = transform_.translate + distance;

    // マップの範囲内か確認
    if ((newPosition.x >= 0 && newPosition.x < WIDTH) && (newPosition.z>= 0 && newPosition.z < DEPTH))
    {
		//プレイヤーがいる場所には移動しない
        if (newPosition.x == player_->GetPosition().x && newPosition.z == player_->GetPosition().z) { return; }
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

    // プレイヤー位置の取得
    Vector3 playerPos = player_->GetPosition();

    // 現在位置との差分を計算
    Vector3 diff = playerPos - transform_.translate;

    // 最短方向に基づいて移動（X および Z 両方）
    Vector3 distance{ 0.0f, 0.0f, 0.0f };

    if (std::abs(diff.x) > 0.1f) { // 小さな差分は無視
        distance.x = (diff.x > 0) ? 1.0f : -1.0f;
    }
	else if (std::abs(diff.z) > 0.1f) { // 小さな差分は無視
        distance.z = (diff.z > 0) ? 1.0f : -1.0f;
    }

    // 移動指示
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
		transform_.translate.x = EasingToEnd(moveStartPosition_.x, moveTargetPosition_.x, EaseInSine, moveProgress_);
		transform_.translate.z = EasingToEnd(moveStartPosition_.z, moveTargetPosition_.z, EaseInSine, moveProgress_);
    	if (moveProgress_ >= 1.0f) {
            isEaseStart_ = false;
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
