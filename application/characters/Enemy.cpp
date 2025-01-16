#include "Enemy.h"

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


    Vector3 distance{ 0.0f, 0.0f, 0.0f };

    if (Input::GetInstans()->TriggerKey(DIK_W))
    {
        distance.y += 1.0f;
    }
    if (Input::GetInstans()->TriggerKey(DIK_S))
    {
        distance.y -= 1.0f;
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
    if (distance.x != 0.0f || distance.y != 0.0f)
    {
        Move(distance);
    }
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
	Vector3 newPosition = transform_.translate + distance;

    // マップの範囲内か確認
    if (newPosition.x >= 0 && newPosition.x < WIDTH && newPosition.y >= 0 && newPosition.y < HEIGHT)
    {
        // 進む先のブロックのtypeが0なら進む
        if (field_->GetBlockType((int)newPosition.x, (int)newPosition.y, 0) == 0)
        {
			transform_.translate = newPosition;
			transform_.translate.z = -2.0f;
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
