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
	pCamera_->SetRotate({ -2.015f,0.0f,0.0f, });
	pCamera_->SetTranslate({ 3.0f,-14.285f,8.334f });
	CameraManager::GetInstans()->AddCamera("main", pCamera_);


	// 3Dオブジェクト

	ModelManager::GetInstans()->LoadModel("cube.obj");
	//ModelManager::GetInstans()->LoadModel("cube2.obj");

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

	playerManager_ = std::make_unique<PlayerManager>();  // 追加
	playerManager_->Initialize(ball);  // 追加
	


	

	//敵のモデル読み込み
	ModelManager::GetInstans()->LoadModel("Enemy.obj");

	//エネミーマネージャー
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->SetField(pField_.get());
	enemyManager_->SetPlayer(playerManager_->GetPlayers());  // プレイヤーをエネミーマネージャーにセット
	enemyManager_->Initialize("Enemy.obj", 3);

	//スプライトの生成
	whiteSprite_ = new Sprite();
	whiteSprite_->Initialize(SpriteCommon::GetInstance(), "Resources/white.png");
	whiteSprite_->SetSize({ 1280.0f,720.0f });
	whiteSprite_->setColor({ 1.0f,1.0f,1.0f,0.0f });

	blackSprite_ = new Sprite();
	blackSprite_->Initialize(SpriteCommon::GetInstance(), "Resources/black.png");
	blackSprite_->SetSize({ 1280.0f,720.0f });
	blackSprite_->setColor({ 1.0f,1.0f,1.0f,1.0f });

	// フェード
	isSceneStart_ = true;
	isClearFadeStart_ = false;
	isGameOverFadeStart_ = false;
	whiteAlpha_ = 0.0f;
	blackAlpha_ = 1.0f;

	// BGMの読み込みと再生
	bgm_ = Audio::GetInstance()->SoundLoadWave("Resources/audio/gameplay/bgm.wav");
}

void GamePlayScene::Finalize()
{
	// ボールの終了処理
	ball->Finalize();
	delete ball;
	// プレイヤーの終了処理
	playerManager_->Finalize();


	delete MouseObject;


	pField_->Finalize();
	for (auto pFieldObject_ : pFieldObject_)
	{
		delete pFieldObject_;
	}

	CameraManager::GetInstans()->RemoveCamera("main");
	

	delete whiteSprite_;
	delete blackSprite_;
}

void GamePlayScene::Update()
{
	// BGMの再生
	if (!isBgmPlay_)
	{
		Audio::GetInstance()->SoundPlayWave(bgm_);
		isBgmPlay_ = true;
	}
	if (isBgmPlay_ && !Audio::GetInstance()->IsSoundPlaying())
	{
		isBgmPlay_ = false;
	}

	Audio::GetInstance()->SetPlaybackSpeed(3.0f);
	
	//カメラの更新
	CameraManager::GetInstans()->GetActiveCamera()->Update();
	mousePos = Input::GetInstans()->GetMouseWorldPosition(CameraManager::GetInstans()->GetActiveCamera()->GetTransform().translate.y);

	//スプライトの更新
	whiteSprite_->Update();
	blackSprite_->Update();

	Fade();

	// 現在のカメラの位置を基準にしたマウス位置取得


	MouseObject->SetTranslate(mousePos);
	MouseObject->Update();
	
	CameraManager::GetInstans()->GetActiveCamera()->SetTranslate(cameraPos_);
	CameraManager::GetInstans()->GetActiveCamera()->SetRotate(cameraRot_);
	



	//ターンごとに更新を行う
	switch (turnState_)
	{
	case TurnState::NONE:
		break;
	case TurnState::PLAYER:

	
		//playerManager_->UpdateTransform();
		if (playerManager_->HasAnyPlayerMovedOrPassed()) {
			turnState_ = TurnState::ENEMY;
			enemyManager_->SetEnemyTurn(true);
			playerManager_->ResetAllMoveAndPassFlags();
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
	playerManager_->Update(pField_.get(), enemyManager_.get());


	//エネミーの３Dオブジェクトを更新
	enemyManager_->UpdateTransform();

	// ゴール判定
	if (pField_->IsGoal())
	{
		isClearFadeStart_ = true;
	}

	// ゲームオーバー判定
	if (pField_->IsGameOver())
	{
		isGameOverFadeStart_ = true;
		
	}
	if (enemyManager_->IsSandwiching() && turnState_ == TurnState::PLAYER)
	{
		isGameOverFadeStart_ = true;
	}

	////// ------------テスト----------------
	////// プレイヤーの位置テスト
	//for (const auto& player : pPlayer_) {
	//	Field::Pos prePlayerPos = { player->GetPrePosX(), player->GetPrePosY(), player->GetPrePosZ() };
	//	pField_->SetBlockType(prePlayerPos.x, prePlayerPos.y, prePlayerPos.z, 0);
	//}

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
		if (ImGui::Button("CCC"))
		{
			isClearFadeStart_ = true;
		}
		if (ImGui::Button("GGG"))
		{
			isGameOverFadeStart_ = true;
		}

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
		playerManager_->ImGui();

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
	playerManager_->Draw();

	// ボールの描画
	ball->Draw();
	
	//エネミーの描画
	enemyManager_->Draw();

#pragma endregion


#pragma region スプライト描画

	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();

	whiteSprite_->Draw();
	blackSprite_->Draw();

#pragma endregion

}

void GamePlayScene::Fade()
{
	blackSprite_->setColor({ 1.0f,1.0f,1.0f,blackAlpha_ });
	whiteSprite_->setColor({ 1.0f,1.0f,1.0f,whiteAlpha_ });

	// シーンスタート
	if (isSceneStart_ && blackAlpha_ > 0.01f)
	{
		blackAlpha_ -= 0.01f;

		if (blackAlpha_ <= 0.01f)
		{
			isSceneStart_ = false;
		}
	}

	// ゲームオーバー切り替え
	if (isGameOverFadeStart_ && !isSceneStart_)
	{
		blackAlpha_ += 0.01f;
	}
	if (blackAlpha_ >= 1.0f)
	{
		SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	}

	// クリア切り替え
	if (isClearFadeStart_ && !isSceneStart_)
	{
		whiteAlpha_ += 0.01f;
	}
	if (whiteAlpha_ >= 1.0f)
	{
		SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
	}
}

//void GamePlayScene::SetclickPlayerPos()
//{
//	// **プレイヤーの選択処理**
//	for (const auto& player : pPlayer_) {
//		Vector3 playerPos = pField_->GetBlockPositionAt(player->GetPosX(), 0, player->GetPosZ());
//		Vector3 playerSize = Vector3(1.0f, 1.0f, 1.0f);
//
//		// **マウスがプレイヤーの範囲内にあるか判定**
//		if (mousePos.x >= playerPos.x - playerSize.x / 2 &&
//			mousePos.x <= playerPos.x + playerSize.x / 2 &&
//			mousePos.z >= playerPos.z - playerSize.z / 2 &&
//			mousePos.z <= playerPos.z + playerSize.z / 2) {
//
//			// **左クリックでプレイヤーを選択**
//			if (Input::GetInstans()->TriggerMouse(0)) {
//				selectedPlayer_ = player.get(); // `unique_ptr` からポインタ取得
//				ImGui::Text("Player Selected at: %d, %d", player->GetPosX(), player->GetPosZ());
//				return;
//			}
//		}
//	}
//
//	// **プレイヤーが選択されている場合に移動を実行**
//	if (selectedPlayer_ != nullptr) {
//		if (Input::GetInstans()->TriggerMouse(0)) {
//
//			selectedPlayer_->HandleMouseClick(mousePos, pField_.get(), selectedPlayer_);
//		}
//	}
//}






