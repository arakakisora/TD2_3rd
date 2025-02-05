#include "Player.h"
#include "Field.h"
#include "CameraManager.h"
#include "Ball.h"
#include "application/characters/enemy/EnemyManager.h"
#include "PlayerManager.h"
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
	playerData.position = Vector3((float)posX, -0.5f, 10.0f);
	playerData.rotate = Vector3(0.0f, 0.0f, 0.0f);
	playerData.scale = Vector3(0.5f, 0.5f, 0.5f);
	ModelManager::GetInstans()->LoadModel("Player.obj");
	ModelManager::GetInstans()->LoadModel("uiBlock1.obj");
	ModelManager::GetInstans()->LoadModel("uiBlock2.obj");
	// モデルの生成
	object3D_ = new Object3D;
	object3D_->SetModel("Player.obj");
	object3D_->Initialize(Object3DCommon::GetInstance());
	object3D_->SetTranslate(playerData.position);
	object3D_->SetRotate(playerData.rotate);
	object3D_->SetScale(playerData.scale);


	//パスモデルの初期化
	passObject3D_ = new Object3D;
	passObject3D_->SetModel("uiBlock2.obj");
	passObject3D_->Initialize(Object3DCommon::GetInstance());
	passObject3D_->SetTranslate(playerData.position + Vector3(-1.0f, 0.0f, 0.0f));
	passObject3D_->SetRotate(playerData.rotate);
	passObject3D_->SetScale({ 0.5f,0.5f,0.5f });

	//ドリブルモデルの初期化
	dribbleObject3D_ = new Object3D;
	dribbleObject3D_->SetModel("uiBlock1.obj");
	dribbleObject3D_->Initialize(Object3DCommon::GetInstance());
	dribbleObject3D_->SetTranslate(playerData.position + Vector3(1.0f, 0.0f, 0.0f));
	dribbleObject3D_->SetRotate(playerData.rotate);
	dribbleObject3D_->SetScale({ 0.5f,0.5f,0.5f });


	if (HasBall())
	{
		ball->SetPosition(playerData.position);
	}

}

void Player::Update(PlayerManager* playerManager)
{

	UpdateTransform();
	Move(WIDTH, DEPTH,playerManager);

	object3D_->SetTranslate(playerData.position);
	object3D_->SetRotate(playerData.rotate);
	object3D_->SetScale(playerData.scale);
	object3D_->Update();

	//パスモデルの更新
	passObject3D_->SetTranslate(playerData.position + Vector3(-1.0f, -0.5f, -0.5f));
	passObject3D_->Update();

	//ドリブルモデルの更新0
	dribbleObject3D_->SetTranslate(playerData.position + Vector3(1.0f, -0.5f, -0.5f));
	dribbleObject3D_->Update();


	// **ボールの所有者が移動したらボールを追従させる**
	if (hasBall && ball)
	{
		ball->SetPosition(playerData.position);
	}
	

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

void Player::Move(int WIDTH, int DEPTH, PlayerManager* playerManager)
{
	// キー入力に応じてプレイヤーの位置を更新
	if (Input::GetInstans()->TriggerKey(DIK_S) && posZ < DEPTH - 1) { posZ += 1; }
	if (Input::GetInstans()->TriggerKey(DIK_W) && posZ > 0) { posZ -= 1; }
	if (Input::GetInstans()->TriggerKey(DIK_A) && posX > 0) { posX -= 1; }
	if (Input::GetInstans()->TriggerKey(DIK_D) && posX < WIDTH - 1) { posX += 1; }

	// プレイヤーの位置を更新
	playerData.position = Vector3(static_cast<float>(posX), -0.5f, static_cast<float>(posZ));
	

	// 🔹 **ボールの取得処理を統合**
	Ball* gameBall = playerManager->GetBall();
	if (gameBall && !hasBall)
	{
		Vector3 ballPos = gameBall->GetPosition();
		if (std::round(ballPos.x) == posX && std::round(ballPos.z) == posZ)
		{
			SetBall(gameBall);
		}
	}
}




void Player::UpdateTransform()
{
	object3D_->SetTranslate(playerData.position);
	object3D_->SetRotate(playerData.rotate);
	object3D_->SetScale(playerData.scale);
	object3D_->Update();
}



void Player::HandleMouseClick(const Vector3& mousePos, Field* field, Player*& selectedPlayer, EnemyManager* enemyManager, PlayerManager* playerManager)
{
	// プレイヤー本体がクリックされた場合
	if (CheckObjectClick(object3D_, mousePos)) {
		if (selectedPlayer != this) {
			if (selectedPlayer) {
				selectedPlayer->isPassDribbleVisible = false;
				selectedPlayer->isPassing = false;
				selectedPlayer->isDribbling = false;
			}
			selectedPlayer = this;
			isPassDribbleVisible = true;
		}
		return;
	}

	// 選択されていない場合は何もしない
	if (selectedPlayer != this) return;

	// ドリブルモードの処理
	if (isDribbling) {
		playerDribble(mousePos, field, selectedPlayer, enemyManager);
		return;
	}

	// パスオブジェクトのクリック判定
	if (CheckObjectClick(passObject3D_, mousePos) && isPassDribbleVisible) {
		isPassing = true;
		isDribbling = false;
		isPassDribbleVisible = false;
		return;
	}

	// ドリブルオブジェクトのクリック判定
	if (CheckObjectClick(dribbleObject3D_, mousePos) && isPassDribbleVisible) {
		isDribbling = true;
		isPassing = false;
		isPassDribbleVisible = false;
		return;
	}

	// パスモードでのクリック処理
	if (isPassing) {
		PlayerPass(mousePos, field, selectedPlayer, playerManager);
		return;
	}
}





// **移動時の処理**
void Player::playerDribble(const Vector3& mousePos, Field* field, Player*& selectedPlayer, EnemyManager* enemyManager)
{

	// ボールを所持している場合、ボールをプレイヤーの位置に設定
	if (hasBall && ball)
	{
		ball->SetPosition(playerData.position);
	}

	for (int z = 0; z < DEPTH; z++) {
		for (int x = 0; x < WIDTH; x++) {
			Vector3 blockPos = field->GetBlockPositionAt(x, 0, z);

			// **マウスがブロックの範囲内にあるか判定**
			if (mousePos.x >= blockPos.x - 0.5f &&
				mousePos.x <= blockPos.x + 0.5f &&
				mousePos.z >= blockPos.z - 0.5f &&
				mousePos.z <= blockPos.z + 0.5f) {

				// **移動先にエネミーがいるかチェック**
				if (enemyManager && enemyManager->IsEnemyAtPosition(x, z))
				{
					return; // エネミーがいるので移動不可
				}

				// **隣接1マスのみ移動を許可**
				if (CanMoveTo(x, z)) {
					SetPlayerPos(x, z);
					isMoved = true;  // **移動フラグを立てる**
	
					isDribbling = false; // **移動後にドリブル解除**
					selectedPlayer = nullptr;  // **移動後に選択解除**

					UpdateTransform();
					return;
				}
			}
		}
	}
}

void Player::SetBall(Ball* newBall)
{
	if (newBall)
	{
		this->ball = newBall;
		hasBall = true;
		ball->SetPosition(playerData.position);
	}
	else
	{
		this->ball = nullptr;
		hasBall = false;
	}
}







// **指定した座標に移動可能かを判定**
bool Player::CanMoveTo(int x, int z)
{
	return (abs(x - posX) + abs(z - posZ) == 1); // 1マスのみ移動許可
}

bool Player::IsValidPassPosition(const Vector3& mousePos, Field* field)
{
	// マウス位置をグリッド座標に変換
	int targetX = static_cast<int>(std::round(mousePos.x));
	int targetZ = static_cast<int>(std::round(mousePos.z));

	// 現在位置からの相対位置を計算
	int deltaX = targetX - posX;
	int deltaZ = targetZ - posZ;

	// パス可能な位置の条件：
	// 十字の2マス先のみ（上下左右のいずれか）
	bool isValidPass = (
		(std::abs(deltaX) == 2 && deltaZ == 0) ||  // 左右2マス
		(deltaX == 0 && std::abs(deltaZ) == 2)     // 上下2マス
		);

	// フィールドの範囲内かチェック
	bool isWithinField = (
		targetX >= 0 &&
		targetX < WIDTH &&
		targetZ >= 0 &&
		targetZ < DEPTH
		);

	return isValidPass && isWithinField;
}

bool Player::CheckObjectClick(Object3D* object, const Vector3& mousePos)
{
	if (!object) return false;

	Vector3 objPos = object->GetTransform().translate;
	Vector3 objSize = Vector3(1.0f, 1.0f, 1.0f);

	return (mousePos.x >= objPos.x - objSize.x / 2 &&
		mousePos.x <= objPos.x + objSize.x / 2 &&
		mousePos.z >= objPos.z - objSize.z / 2 &&
		mousePos.z <= objPos.z + objSize.z / 2);
}


void Player::PlayerPass(const Vector3& mousePos, Field* field, Player*& selectedPlayer, PlayerManager* playerManager)
{
	if (!IsValidPassPosition(mousePos, field))
	{
		// **無効な位置でもターン終了フラグを立てるように修正**
		ispsMoved = true;
		isMoved = true;
		selectedPlayer = nullptr;
		
		return;
	}

	int targetX = static_cast<int>(std::round(mousePos.x));
	int targetZ = static_cast<int>(std::round(mousePos.z));

	Player* receiver = nullptr;
	for (auto& player : playerManager->GetPlayers())
	{
		if (player->GetPosX() == targetX && player->GetPosZ() == targetZ)
		{
			receiver = player.get();
			break;
		}
	}

	if (receiver)
	{
		// 🔹 **受け取るプレイヤーにボールを渡す**
		receiver->SetBall(ball);
	}
	else
	{
		// 🔹 **誰もいなければボールだけを置く**
		if (ball)
		{
			Vector3 newBallPos = Vector3(static_cast<float>(targetX), -0.5f, static_cast<float>(targetZ));
			ball->SetPosition(newBallPos);
		}
	}


	isPassing = false;
	isPassDribbleVisible = false;

	// 🔹 **ターン終了フラグを確実に設定**
	ispsMoved = true;
	isMoved = true;
	selectedPlayer = nullptr;
	// 🔹 **元のプレイヤーのボール所有を解除**
	hasBall = false;
	ball = nullptr;  // ボールの所有者がいなくなる
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

	//パスオブジェクト
	//位置調節
	Transform passObjectTransform = passObject3D_->GetTransform();
	ImGui::DragFloat3("passObjectPos", &passObjectTransform.translate.x, 0.1f);
	ImGui::DragFloat3("passObjectRot", &passObjectTransform.rotate.x, 0.1f);
	ImGui::DragFloat3("passObjectScale", &passObjectTransform.scale.x, 0.1f);
	passObject3D_->SetTransform(passObjectTransform);


	//ドリブルオブジェクト
	//位置調節
	Transform dribbleObjectTransform = dribbleObject3D_->GetTransform();
	ImGui::DragFloat3("dribbleObjectPos", &dribbleObjectTransform.translate.x, 0.1f);
	ImGui::DragFloat3("dribbleObjectRot", &dribbleObjectTransform.rotate.x, 0.1f);
	ImGui::DragFloat3("dribbleObjectScale", &dribbleObjectTransform.scale.x, 0.1f);
	dribbleObject3D_->SetTransform(dribbleObjectTransform);




}










