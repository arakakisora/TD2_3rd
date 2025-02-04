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


	//パスモデルの初期化
	passObject3D_ = new Object3D;
	passObject3D_->SetModel("cube.obj");
	passObject3D_->Initialize(Object3DCommon::GetInstance());
	passObject3D_->SetTranslate(playerData.position+Vector3(0.0f, 0.0f, -1.0f));
	passObject3D_->SetRotate(playerData.rotate);
	passObject3D_->SetScale({1.0f,1.0f,1.0f});

	//ドリブルモデルの初期化
	dribbleObject3D_ = new Object3D;
	dribbleObject3D_->SetModel("cube2.obj");
	dribbleObject3D_->Initialize(Object3DCommon::GetInstance());
	dribbleObject3D_->SetTranslate(playerData.position+Vector3(0.0f,0.0f,1.0f));
	dribbleObject3D_->SetRotate(playerData.rotate);
	dribbleObject3D_->SetScale({ 1.0f,1.0f,1.0f });


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

	object3D_->SetTranslate(playerData.position);
	object3D_->SetRotate(playerData.rotate);
	object3D_->SetScale(playerData.scale);
	object3D_->Update();

	//パスモデルの更新
	passObject3D_->SetTranslate(playerData.position + Vector3(0.0f, 0.0f, -1.0f));
	passObject3D_->Update();

	//ドリブルモデルの更新0
	dribbleObject3D_->SetTranslate(playerData.position + Vector3(0.0f, 0.0f, 1.0f));
	dribbleObject3D_->Update();



	ImGui();

}

void Player::Draw()
{
	object3D_->Draw();

	if (isPassDribbleVisible)
	{
		passObject3D_->Draw();
		dribbleObject3D_->Draw();
	}
	

}

void Player::Finalize()
{
	delete object3D_;

	delete model;
	delete passObject3D_;
	delete dribbleObject3D_;

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

// **オブジェクトのクリック判定**
bool Player::CheckObjectClick(Object3D* object, const Vector3& mousePos)
{
	Vector3 objPos = object->GetTransform().translate; // オブジェクトの位置
	Vector3 objSize = object->GetTransform().scale;   // オブジェクトのサイズ


	return (mousePos.x >= objPos.x - objSize.x / 2 &&
		mousePos.x <= objPos.x + objSize.x / 2 &&
		mousePos.z >= objPos.z - objSize.z / 2 &&
		mousePos.z <= objPos.z + objSize.z / 2);
}


void Player::HandleMouseClick(const Vector3& mousePos, Field* field, Player*& selectedPlayer)
{
	// **新しいプレイヤーが選択された場合、前のプレイヤーの選択状態をリセット**
	if (CheckObjectClick(object3D_, mousePos)) {
		if (selectedPlayer != this) {
			// **前に選択していたプレイヤーの選択を解除**
			if (selectedPlayer) {
				selectedPlayer->isPassDribbleVisible = false;  // **表示をオフ**
				selectedPlayer->isPassing = false;
				selectedPlayer->isDribbling = false;
			}
			// **新しいプレイヤーを選択**
			selectedPlayer = this;
			isPassDribbleVisible = true;
		}
		return;
	}

	// **選択されていない場合は何もしない**
	if (selectedPlayer != this) return;

	// **ドリブルモードを先に処理して、移動可能ならすぐ移動**
	if (isDribbling) {
		playerDribble(mousePos, field, selectedPlayer);
		return;
	}

	// **パスオブジェクトのクリック判定**
	if (CheckObjectClick(passObject3D_, mousePos) && isPassDribbleVisible) {
		isPassing = true;
		isDribbling = false;
		isPassDribbleVisible = false;  // **選択後は非表示**
		return;
	}

	// **ドリブルオブジェクトのクリック判定**
	if (CheckObjectClick(dribbleObject3D_, mousePos) && isPassDribbleVisible) {
		isDribbling = true;
		isPassing = false;
		isPassDribbleVisible = false;  // **選択後は非表示**
		return;
	}

	// **パスモードの場合にパス**
	if (isPassing) {
		playerPass(selectedPlayer);
		return;
	}
}






// **移動時の処理**
void Player::playerDribble(const Vector3& mousePos, Field* field, Player*& selectedPlayer)
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
					isDribbling = false; // **移動後にドリブル解除**
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



void Player::playerPass(Player*& selectedPlayer)
{
	isPassing = false;
	isPassDribbleVisible = false;  // **パス後に選択UIを非表示**
	ispsMoved = true;  // **パスフラグを立てる**
	selectedPlayer = nullptr;  // **選択を解除**
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



