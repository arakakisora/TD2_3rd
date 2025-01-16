#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	// カメラの初期化
	//camera = new Camera();
	//camera->SetTranslate(Vector3(0.0f, 0.0f, -5.0f));
	//CameraMaanager::GetInstance()->SetCamera(camera);
	// プレイヤーの初期化

	playerData.position = Vector3(0.0f, 0.0f, -5.0f);
	playerData.rotate = Vector3(0.0f, 0.0f, 0.0f);
	playerData.scale = Vector3(1.0f, 1.0f, 1.0f);
	ModelManager::GetInstans()->LoadModel("cube.obj");
	// モデルの生成
	object3D_ = new Object3D;
	object3D_->SetModel("cube.obj");
	object3D_->Initialize(Object3DCommon::GetInstance());
	object3D_->SetTranslate(playerData.position);
	object3D_->SetRotate(playerData.rotate);
	object3D_->SetScale(playerData.scale);

}

void Player::Update()
{
	playerData.rotate.z += 0.01f;
	playerData.rotate.y += 0.01f;
	object3D_->SetRotate(playerData.rotate);
	object3D_->Update();
}

void Player::Draw()
{
	object3D_->Draw();
}

void Player::Finalize()
{
	delete object3D_;
	delete model;
}


