#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	ModelManager::GetInstans()->LoadModel("Player.obj");
	// モデルの生成
	object3D_ = new Object3D;
	//object3D_->SetTransform();

	

}

void Player::Update()
{
	object3D_->Update();
}

void Player::Draw()
{
	object3D_->Draw();
}

void Player::Finalize()
{
	delete object3D_;
}


