#include "Player.h"
#include "Field.h"
#include "CameraManager.h"
Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(int posZ)
{
	// カメラの初期化
	//camera = new Camera();
	//camera->SetTranslate(Vector3(0.0f, 0.0f, -5.0f));
	//CameraMaanager::GetInstance()->SetCamera(camera);
	// プレイヤーの初期化
	this->posZ = posZ;
	playerData.position = Vector3(0.0f, 0.0f, 10.0f);
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
	//playerData.rotate.z += 0.01f;
	//playerData.rotate.y += 0.01f;
	Move(WIDTH, DEPTH);
	object3D_->SetTranslate(playerData.position);
	object3D_->SetRotate(playerData.rotate);
	object3D_->SetScale(playerData.scale);
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

void Player::Move(int WIDTH, int DEPTH)
{
	// キー入力に応じてプレイヤーの位置を更新
	if (Input::GetInstans()->TriggerKey(DIK_S) && posZ < DEPTH - 1)
	{
		prePosX = posX;
		prePosZ = posZ;
		posZ += 1;
	}
	if (Input::GetInstans()->TriggerKey(DIK_W) && posZ > 0)
	{
		prePosX = posX;
		prePosZ = posZ;
		posZ -= 1;
	}
	if (Input::GetInstans()->TriggerKey(DIK_A) && posX > 0)
	{
		prePosX = posX;
		prePosZ = posZ;
		posX -= 1;
	}
	if (Input::GetInstans()->TriggerKey(DIK_D) && posX < WIDTH - 1)
	{
		prePosX = posX;
		prePosZ = posZ;
		posX += 1;
	}

	//マウスでクリックした位置に移動
	if (Input::GetInstans()->TriggerMouse(0))
	{
		// マウス座標を取得
		Vector3 mousePos = Input::GetInstans()->GetMouseWorldPosition(CameraManager::GetInstans()->GetActiveCamera()->GetTransform().translate.y);
		// マウス座標をマス座標に変換
		posX = static_cast<int>(mousePos.x);
		posZ = static_cast<int>(mousePos.z);
	}

	// プレイヤーの位置を更新
	playerData.position = Vector3(static_cast<float>(posX) * 1.0f, 0.0f, static_cast<float>(posZ) * 1.0f);
}

void Player::ImGui()
{
	ImGui::Text("Player");
	ImGui::Text("posX : %d", posX);
	ImGui::Text("posY : %d", posY);
	ImGui::Text("posZ : %d", posZ);

	Vector3 mousePos = Input::GetInstans()->GetMouseWorldPosition(CameraManager::GetInstans()->GetActiveCamera()->GetTransform().translate.y);
	//マウス座標の取得
	ImGui::Text("mousePos : %f %f %f", mousePos.x, mousePos.y, mousePos.z);


}



