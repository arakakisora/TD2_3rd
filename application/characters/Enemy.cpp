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
	// 3Dオブジェクトの更新
	object3D_->Update();
}

void Enemy::Draw()
{
	// 3Dオブジェクトの描画
	object3D_->Draw();
}
