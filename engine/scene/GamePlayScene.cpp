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
	ModelManager::GetInstans()->LoadModel("cube.obj");

	// フィールド

	for (int z = 0; z < DEPTH; z++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			Object3D* pFieldObject = new Object3D();
			pFieldObject->Initialize(Object3DCommon::GetInstance());
			pFieldObject->SetModel("cube.obj");
			pFieldObject_.push_back(pFieldObject);
		}
	}

	pField_ = std::make_unique<Field>();
	pField_->Initialize(pFieldObject_);

	MouseObject = new Object3D();
	MouseObject->Initialize(Object3DCommon::GetInstance());
	MouseObject->SetModel("cube.obj");

	//Playerの初期化
	pPlayer_ = new Player();
	pPlayer_->Initialize(0, 0);
	pPlayer2_ = new Player();
	pPlayer2_->Initialize(0, 3);
	pPlayer3_ = new Player();
	pPlayer3_->Initialize(0, 4);
	// プレイヤーの位置をフィールドにセット
	pField_->SetPlayerPos(pPlayer_->GetPosX(), pPlayer_->GetPosY(), pPlayer_->GetPosZ());
	pField_->SetPlayerPos(pPlayer2_->GetPosX(), pPlayer2_->GetPosY(), pPlayer2_->GetPosZ());
	pField_->SetPlayerPos(pPlayer3_->GetPosX(), pPlayer3_->GetPosY(), pPlayer3_->GetPosZ());
	//エネミー
	enemy_ = std::make_unique<Enemy>();
	enemy_->SetField(pField_.get());
	enemy_->Initialize(Object3DCommon::GetInstance(), "cube.obj");


	enemy_->SetPlayer(pPlayer_);


}

void GamePlayScene::Finalize()
{
	pPlayer_->Finalize();
	delete pPlayer_;
	pPlayer2_->Finalize();
	delete pPlayer2_;
	pPlayer3_->Finalize();
	delete pPlayer3_;

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
		// プレイヤーの更新
		pPlayer_->Update();
		// プレイヤーの位置をフィールドにセット
		pField_->SetPlayerPos(pPlayer_->GetPosX(), pPlayer_->GetPosY(), pPlayer_->GetPosZ());

		// ターン終了
		//NOTE:今はエンターキーでターンを切り替える
		if (Input::GetInstans()->TriggerKey(DIK_RETURN))
		{
			turnState_ = TurnState::ENEMY;
			// エネミーのターン開始
			enemy_->SetTurnEnd(false);
		}

		break;
	case TurnState::ENEMY:
		//エネミーの更新
		enemy_->Update();
		// ターン終了
		if (enemy_->IsTurnEnd())
		{
			turnState_ = TurnState::PLAYER;
		}
		break;
	}


	// プレイヤーの位置をフィールドにセット
	if (Input::GetInstans()->PushKey(DIK_1))
	{
		];
		pField_->SetPlayerPos(pPlayer_->GetPosX(), pPlayer_->GetPosY(), pPlayer_->GetPosZ());
	}
	if (Input::GetInstans()->PushKey(DIK_2))
	{
		pPlayer2_->Update();
		pField_->SetPlayerPos(pPlayer2_->GetPosX(), pPlayer2_->GetPosY(), pPlayer2_->GetPosZ());
	}
	if (Input::GetInstans()->PushKey(DIK_3))
	{
		pPlayer3_->Update();
		pField_->SetPlayerPos(pPlayer3_->GetPosX(), pPlayer3_->GetPosY(), pPlayer3_->GetPosZ());
	}
	
	
	// フィールドの更新
	pField_->Update();


	//プレイヤーの３Dオブジェクトを更新
	pPlayer_->UpdateTransform();

	//エネミーの３Dオブジェクトを更新
	enemy_->UpdateTransform();

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


	// ------------テスト----------------
	// プレイヤーの位置テスト
	prePlayerPos_ = { pPlayer_->GetPrePosX(),pPlayer_->GetPrePosY(),pPlayer_->GetPrePosZ() };
	prePlayerPos2_ = { pPlayer2_->GetPrePosX(),pPlayer2_->GetPrePosY(),pPlayer2_->GetPrePosZ() };
	prePlayerPos3_ = { pPlayer3_->GetPrePosX(),pPlayer3_->GetPrePosY(),pPlayer3_->GetPrePosZ() };
	pField_->SetBlockType(prePlayerPos_.x, prePlayerPos_.y, prePlayerPos_.z, 0);

	// ボールの位置テスト

	prePos_ = pField_->GetBlockPosition(1);


	// パス
	if (Input::GetInstans()->PushKey(DIK_P) && Input::GetInstans()->TriggerKey(DIK_RIGHT) && (int)prePos_.x <= WIDTH - 3)
	{
		pField_->SetBlockType((int)prePos_.x + 2, (int)prePos_.y, (int)prePos_.z, 1);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
	}
	else if (Input::GetInstans()->PushKey(DIK_P) && Input::GetInstans()->TriggerKey(DIK_LEFT) && (int)prePos_.x >= 2)
	{
		pField_->SetBlockType((int)prePos_.x - 2, (int)prePos_.y, (int)prePos_.z, 1);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
	}
	else if (Input::GetInstans()->PushKey(DIK_P) && Input::GetInstans()->TriggerKey(DIK_UP) && (int)prePos_.z >= 2)
	{
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z - 2, 1);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
	}
	else if (Input::GetInstans()->PushKey(DIK_P) && Input::GetInstans()->TriggerKey(DIK_DOWN) && (int)prePos_.z <= DEPTH - 3)
	{
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z + 2, 1);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
	}

	// ドリブル
	else if (Input::GetInstans()->TriggerKey(DIK_RIGHT) && (int)prePos_.x >= 0 && (int)prePos_.x <= WIDTH - 2)
	{
		pField_->SetBlockType((int)prePos_.x + 1, (int)prePos_.y, (int)prePos_.z, 1);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
	}
	else if (Input::GetInstans()->TriggerKey(DIK_LEFT) && (int)prePos_.x >= 1 && (int)prePos_.x <= WIDTH - 1)
	{
		pField_->SetBlockType((int)prePos_.x - 1, (int)prePos_.y, (int)prePos_.z, 1);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
	}
	else if (Input::GetInstans()->TriggerKey(DIK_UP) && (int)prePos_.z >= 1 && (int)prePos_.z <= DEPTH - 1)
	{
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z - 1, 1);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
	}
	else if (Input::GetInstans()->TriggerKey(DIK_DOWN) && (int)prePos_.z >= 0 && (int)prePos_.z <= DEPTH - 2)
	{
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z + 1, 1);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
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

		pPlayer_->ImGui();
		pPlayer2_->ImGui();
		pPlayer3_->ImGui();

		ImGui::Text("prePlayerPos %d", &prePlayerPos_.x);


		if(ImGui::Button("Turn Player"))
		{
			turnState_ = TurnState::PLAYER;
		}
		ImGui::SameLine();
		if (ImGui::Button("Turn Enemy"))
		{
			turnState_ = TurnState::ENEMY;
			enemy_->SetTurnEnd(false);
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
	pPlayer_->Draw();
	pPlayer2_->Draw();
	pPlayer3_->Draw();

	//エネミーの描画
	enemy_->Draw();

#pragma endregion


#pragma region スプライト描画

	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();

#pragma endregion

}

void  GamePlayScene::SetclickPlayerPos()
{
	// マウス位置とフィールドブロックの判定
	for (int z = 0; z < DEPTH; z++) {
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				// ブロックの位置とスケールを取得
				const Vector3& blockPos = pField_->GetBlockPositionAt(x, y, z);
				Vector3 blockScale = Vector3(1.0f, 1.0f, 1.0f); // 既定値

				// ブロックのタイプに応じたスケール変更
				uint32_t blockType = pField_->GetBlockType(x, y, z);
				if (blockType == 1) blockScale = Vector3(0.15f, 0.15f, 0.15f);
				else if (blockType == 2) blockScale = Vector3(0.8f, 0.8f, 0.8f);
				else if (blockType == 5) blockScale = Vector3(0.5f, 0.5f, 0.5f);

				// マウス位置との簡易的なAABB判定
				if (mousePos.x >= blockPos.x - blockScale.x / 2 &&
					mousePos.x <= blockPos.x + blockScale.x / 2 &&
					mousePos.z >= blockPos.z - blockScale.z / 2 &&
					mousePos.z <= blockPos.z + blockScale.z / 2) {
					if (Input::GetInstans()->TriggerMouse(0)) {

						pPlayer_->SetPlayerPos(x, z);

					}

					// 当たったブロックの座標をログ出力（または他の処理）
					ImGui::Text("Hit Block: %d, %d, %d", x, y, z);


				}
			}
		}
	}


}
