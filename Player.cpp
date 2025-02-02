#include "Player.h"
#include "Field.h"
#include "CameraManager.h"
#include "Ball.h"
Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(int posZ, Ball* ball)
{
	// ボールの設定
	SetBall(ball);
	//Vector3 ballPos = Vector3(2.0f, 0.0f, 10.0f);
	//ball->SetPosition(ballPos);

	// プレイヤーの初期化
	this->posZ = posZ;
	playerData.position = Vector3((float)posX, 0.0f, 10.0f);
	playerData.rotate = Vector3(0.0f, 0.0f, 0.0f);
	playerData.scale = Vector3(0.5f, 0.5f, 0.5f);
	ModelManager::GetInstans()->LoadModel("Player.obj");
	// モデルの生成
	object3D_ = new Object3D;
	object3D_->SetModel("Player.obj");
	object3D_->Initialize(Object3DCommon::GetInstance());
	object3D_->SetTranslate(playerData.position);
	object3D_->SetRotate(playerData.rotate);
	object3D_->SetScale(playerData.scale);

	if (HasBall())
	{
		ball->SetPosition(playerData.position);
	}
}

void Player::Update()
{
	playerData.rotate.z += 0.01f;
	playerData.rotate.y += 0.01f;

	UpdateTransform();
	Move(WIDTH, DEPTH);
	ImGui();
	UpdateTransform();
}

void Player::Draw()
{
	object3D_->Draw();
}

void Player::Finalize()
{
	delete object3D_;
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


	////マウスでクリックした位置に移動
	//if (Input::GetInstans()->TriggerMouse(0))
	//{
	//	// マウス座標を取得
	//	Vector3 mousePos = Input::GetInstans()->GetMouseWorldPosition(CameraManager::GetInstans()->GetActiveCamera()->GetTransform().translate.y);
	//	// マウス座標をマス座標に変換
	//	posX = static_cast<int>(mousePos.x);
	//	posZ = static_cast<int>(mousePos.z);
	//}

	// プレイヤーの位置を更新
	playerData.position = Vector3(static_cast<float>(posX), 0.0f, static_cast<float>(posZ));

	// ボールの位置を更新 / ドリブル処理 / パス処理
	// 空へここの二つのドリブルとパスを分岐できるように頼んだ
	Dribble();
	

	//Pass();
}

void Player::UpdateTransform()
{
	object3D_->SetTranslate(playerData.position);
	object3D_->SetRotate(playerData.rotate);
	object3D_->SetScale(playerData.scale);
	object3D_->Update();
}


void Player::HandleMouseClick(const Vector3& mousePos, Field* field, Player*& selectedPlayer)
{
	for (int z = 0; z < DEPTH; z++) {
		for (int x = 0; x < WIDTH; x++) {
			Vector3 blockPos = field->GetBlockPositionAt(x, 0, z);

			// **マウスがブロックの範囲内にあるか判定**
			if (mousePos.x >= blockPos.x - 0.5f &&
				mousePos.x <= blockPos.x + 0.5f &&
				mousePos.z >= blockPos.z - 0.5f &&
				mousePos.z <= blockPos.z + 0.5f) {

				// **隣接1マスのみ移動を許可**
				if (CanMoveTo(x, z)) {
					SetPlayerPos(x, z);
					isMoved = true;  // **移動フラグを立てる**
					selectedPlayer = nullptr;  // **移動後に選択解除**
					return;
				}
			}
		}
	}
}


// **指定した座標に移動可能かを判定**
bool Player::CanMoveTo(int x, int z)
{
	return (abs(x - posX) + abs(z - posZ) == 1); // 1マスのみ移動許可
}


void Player::ImGui()
{
	ImGui::Text("Player");
	ImGui::Text("posX : %d", posX);
	ImGui::Text("posY : %d", posY);
	ImGui::Text("posZ : %d", posZ);

	//マウス座標の取得
	//Vector3 mousePos = Input::GetInstans()->GetMouseWorldPosition(CameraManager::GetInstans()->GetActiveCamera()->GetTransform().translate.y);
	//ImGui::Text("mousePos : %f %f %f", mousePos.x, mousePos.y, mousePos.z);

	ImGui::Text("HasBall : %d", ball);
}

void Player::Dribble()
{
	// ドリブル処理
	if (hasBall)
	{
		// ボールの位置をプレイヤーの位置に設定
		ball->SetPosition(playerData.position);
	}
}

void Player::Pass()
{
	// パス処理
	if (ball != nullptr)
	{
		// ボールの位置をプレイヤーの位置に設定
		ball->SetPosition(playerData.position);
		// ボールの所持を解除
		SetBall(nullptr);
	}
}



