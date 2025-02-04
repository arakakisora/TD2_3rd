#include "GamePlayScene.h"
#include <ModelManager.h>
#include "Object3DCommon.h"
#include "SpriteCommon.h"
#include "CameraManager.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include "Input.h"
#include "TitleScene.h"

void GamePlayScene::Initialize()
{
	//カメラの生成	
	pCamera_ = new Camera();
	pCamera_->SetRotate({ 0,0,0, });
	pCamera_->SetTranslate({ 0,0,0 });
	CameraManager::GetInstans()->AddCamera("main", pCamera_);


	// 3Dオブジェクト
	ModelManager::GetInstans()->LoadModel("Field0.obj");

	// フィールド

	for (int z = 0; z < DEPTH; z++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			Object3D* pFieldObject = new Object3D();
			pFieldObject->Initialize(Object3DCommon::GetInstance());
			pFieldObject->SetModel("Field0.obj");
			pFieldObject_.push_back(pFieldObject);
		}
	}

	pField_ = std::make_unique<Field>();
	pField_->Initialize(pFieldObject_);

	MouseObject = new Object3D();
	MouseObject->Initialize(Object3DCommon::GetInstance());
	MouseObject->SetModel("cube.obj");

	ball = new Ball();
	ball->Initialize();

	//Playerの初期化
	std::vector<int> initPosZ = { 0, 2, 4 };
	for (int i = 0; i < initPosZ.size(); ++i) {
		auto player = std::make_unique<Player>();
		if(i ==0)
		{
			//１番目にボールを持たせる
			player->Initialize(initPosZ[i], ball);
		} else
		{
			player->Initialize(initPosZ[i]);
		}
		pPlayer_.push_back(std::move(player));
	}


	// プレイヤーの位置をフィールドにセット
	for (const auto& player : pPlayer_) {
		pField_->SetPlayerPos(player->GetPosX(), player->GetPosY(), player->GetPosZ());
	}

	//敵のモデル読み込み
	ModelManager::GetInstans()->LoadModel("Enemy.obj");

	//エネミーマネージャー
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetField(pField_.get());
	enemyManager_->SetPlayer(pPlayer_);
	enemyManager_->Initialize("Enemy.obj", 3);
}

void GamePlayScene::Finalize()
{
	// ボールの終了処理
	ball->Finalize();
	delete ball;
	// プレイヤーの終了処理
	for (auto& player : pPlayer_) {
		player->Finalize();
	}

	delete MouseObject;


	pField_->Finalize();
	for (auto pFieldObject_ : pFieldObject_)
	{
		delete pFieldObject_;
	}

	CameraManager::GetInstans()->RemoveCamera("main");
	delete pCamera_;

}

void GamePlayScene::Update()
{
	//カメラの更新
	CameraManager::GetInstans()->GetActiveCamera()->Update();
	mousePos = Input::GetInstans()->GetMouseWorldPosition(CameraManager::GetInstans()->GetActiveCamera()->GetTransform().translate.y);


	// 現在のカメラの位置を基準にしたマウス位置取得


	MouseObject->SetTranslate(mousePos);
	MouseObject->Update();
	SetclickPlayerPos();
	CameraManager::GetInstans()->GetActiveCamera()->SetTranslate(cameraPos_);
	CameraManager::GetInstans()->GetActiveCamera()->SetRotate(cameraRot_);



	//ターンごとに更新を行う
	switch (turnState_)
	{
	case TurnState::NONE:
		break;
	case TurnState::PLAYER:
		for (auto& player : pPlayer_) {
			player->Update();
			// プレイヤーの位置をフィールドにセット
			pField_->SetPlayerPos(player->GetPosX(), player->GetPosY(), player->GetPosZ());
		}

		for (auto& player : pPlayer_) {

			// ターン終了
			//NOTE:今はエンターキーでターンを切り替える
			if (player->GetHasMoved())
			{
				turnState_ = TurnState::ENEMY;
				
        // エネミーのターン開始
			enemyManager_->SetEnemyTurn(true);
				player->ResetMoveFlag();
			}


		}

		break;
	case TurnState::ENEMY:
		//エネミーの更新
		enemyManager_->Update();
		// ターン終了
		if (!enemyManager_->IsEnemyTurn())
		{
			turnState_ = TurnState::PLAYER;
		}
		break;
	}



	//// プレイヤーの位置をフィールドにセット
	//if (Input::GetInstans()->PushKey(DIK_1))
	//{;
	//	pField_->SetPlayerPos(pPlayer_->GetPosX(), pPlayer_->GetPosY(), pPlayer_->GetPosZ());
	//}
	//if (Input::GetInstans()->PushKey(DIK_2))
	//{
	//	pPlayer2_->Update();
	//	pField_->SetPlayerPos(pPlayer2_->GetPosX(), pPlayer2_->GetPosY(), pPlayer2_->GetPosZ());
	//}
	//if (Input::GetInstans()->PushKey(DIK_3))
	//{
	//	pPlayer3_->Update();
	//	pField_->SetPlayerPos(pPlayer3_->GetPosX(), pPlayer3_->GetPosY(), pPlayer3_->GetPosZ());
	//}


	// ボールの更新
	ball->Update();

	// フィールドの更新
	pField_->Update();


	//プレイヤーの３Dオブジェクトを更新

	for (auto& player : pPlayer_) {
		player->UpdateTransform();
	}

	//エネミーの３Dオブジェクトを更新
	enemyManager_->UpdateTransform();

	// ゴール判定
	if (pField_->IsGoal())
	{
		SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
	}

	// ゲームオーバー判定
	if (pField_->IsGameOver())
	{
		SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	}
	if (enemyManager_->IsSandwiching() && turnState_ == TurnState::PLAYER)
	{
		SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	}

	// ------------テスト----------------
	// プレイヤーの位置テスト
	for (const auto& player : pPlayer_) {
		Field::Pos prePlayerPos = { player->GetPrePosX(), player->GetPrePosY(), player->GetPrePosZ() };
		pField_->SetBlockType(prePlayerPos.x, prePlayerPos.y, prePlayerPos.z, 0);
	}

	// フィールド毎フレーム更新するやつ
	for (int z = 0; z < DEPTH; z++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			if (pField_->GetBlockType(6, 0, z) != 1)
			{
				pField_->SetBlockType(6, 0, z, 2);
			}
		}
	}



#ifdef _DEBUG

	if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("gamePlayScene %d");
		if (ImGui::Button("GameClearScene"))
		{
			SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
		}
		if (ImGui::Button("GameOverScene"))
		{
			SceneManager::GetInstance()->ChangeScene("GAMEOVER");
		}
		ImGui::DragFloat3("mousPos", &mousePos.x, 0.1f);

		ImGui::SliderFloat3("cameraPos", &cameraPos_.x, -50.0f, 50.0f);
		ImGui::SliderFloat3("cameraRot", &cameraRot_.x, -3.0f, 3.0f);

		pField_->ImGui();

#ifdef _DEBUG
		const Vector3& cameraPos = CameraManager::GetInstans()->GetActiveCamera()->GetTransform().translate;
		const Vector3& cameraRot = CameraManager::GetInstans()->GetActiveCamera()->GetTransform().rotate;


#endif
		for (const auto& player : pPlayer_) {
			player->ImGui();
		}
		//ImGui::Text("prePlayerPos %d", &prePlayerPos_.x);


		if (ImGui::Button("Turn Player"))
		{
			turnState_ = TurnState::PLAYER;
		}
		ImGui::SameLine();
		if (ImGui::Button("Turn Enemy"))
		{
			turnState_ = TurnState::ENEMY;
		}


	}

#endif // _DEBUG
}

void GamePlayScene::Draw()
{
#pragma region 3Dオブジェクト描画

	//3dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	Object3DCommon::GetInstance()->CommonDraw();

	//3Dオブジェクトの描画
	MouseObject->Draw();
	pField_->Draw();
	for (const auto& player : pPlayer_) {
		player->Draw();
	}
	// ボールの描画
	ball->Draw();
	
	//エネミーの描画
	enemyManager_->Draw();

#pragma endregion


#pragma region スプライト描画

	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();

#pragma endregion

}

void GamePlayScene::SetclickPlayerPos()
{
	// **プレイヤーの選択処理**
	for (const auto& player : pPlayer_) {
		Vector3 playerPos = pField_->GetBlockPositionAt(player->GetPosX(), 0, player->GetPosZ());
		Vector3 playerSize = Vector3(1.0f, 1.0f, 1.0f);

		// **マウスがプレイヤーの範囲内にあるか判定**
		if (mousePos.x >= playerPos.x - playerSize.x / 2 &&
			mousePos.x <= playerPos.x + playerSize.x / 2 &&
			mousePos.z >= playerPos.z - playerSize.z / 2 &&
			mousePos.z <= playerPos.z + playerSize.z / 2) {

			// **左クリックでプレイヤーを選択**
			if (Input::GetInstans()->TriggerMouse(0)) {
				selectedPlayer_ = player.get(); // `unique_ptr` からポインタ取得
				ImGui::Text("Player Selected at: %d, %d", player->GetPosX(), player->GetPosZ());
				return;
			}
		}
	}

	// **プレイヤーが選択されている場合に移動を実行**
	if (selectedPlayer_ != nullptr) {
		if (Input::GetInstans()->TriggerMouse(0)) {
			selectedPlayer_->HandleMouseClick(mousePos, pField_.get(), selectedPlayer_);
		}
	}
}






