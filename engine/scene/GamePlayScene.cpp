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


	//Playerの初期化
	player = new Player();
	player->Initialize();
	

	//カメラの生成	
	pCamera_ = new Camera();
	pCamera_->SetRotate({ 0,0,0, });
	pCamera_->SetTranslate({ 0,0,-20 });
	CameraManager::GetInstans()->AddCamera("main", pCamera_);


	// 3Dオブジェクト
	ModelManager::GetInstans()->LoadModel("cube.obj");

	// フィールド

	for (int z = 0; z < 5; z++)
	{
		for (int x = 0; x < 7; x++)
		{
			Object3D* pFieldObject = new Object3D();
			pFieldObject->Initialize(Object3DCommon::GetInstance());
			pFieldObject->SetModel("cube.obj");
			pFieldObject_.push_back(pFieldObject);
		}
	}
	

	pField_ = std::make_unique<Field>();
	pField_->Initialize(pFieldObject_);

}

void GamePlayScene::Finalize()
{
	player->Finalize();
	delete player;


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
	player->Update();
	//カメラの更新
	CameraManager::GetInstans()->GetActiveCamera()->Update();

	pField_->Update();

	CameraManager::GetInstans()->GetActiveCamera()->SetTranslate(cameraPos_);
	CameraManager::GetInstans()->GetActiveCamera()->SetRotate(cameraRot_);
	
	// ゴール判定
	if (pField_->IsGoal())
	{
		SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
	}



	// ------------テスト----------------
	// ボールの位置テスト
	prePos_ = pField_->GetBlockPosition(2);
	
	// パス
	if (Input::GetInstans()->PushKey(DIK_P) && Input::GetInstans()->TriggerKey(DIK_RIGHT) && (int)prePos_.x <= WIDTH - 3)
	{
		pField_->SetBlockType((int)prePos_.x + 2, (int)prePos_.y, (int)prePos_.z, 2);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
	}
	else if (Input::GetInstans()->PushKey(DIK_P) && Input::GetInstans()->TriggerKey(DIK_LEFT) && (int)prePos_.x >= 2)
	{
		pField_->SetBlockType((int)prePos_.x - 2, (int)prePos_.y, (int)prePos_.z, 2);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
	}
	else if (Input::GetInstans()->PushKey(DIK_P) && Input::GetInstans()->TriggerKey(DIK_UP) && (int)prePos_.y <= HEIGHT - 3)
	{
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y + 2, (int)prePos_.z, 2);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
	}
	else if (Input::GetInstans()->PushKey(DIK_P) && Input::GetInstans()->TriggerKey(DIK_DOWN) && (int)prePos_.y >= 2)
	{
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y - 2, (int)prePos_.z, 2);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
	}



	// ------------テスト----------------
	// ボールの位置テスト
	prePos_ = pField_->GetBlockPosition(2);
	
	// パス
	if (Input::GetInstans()->PushKey(DIK_P) && Input::GetInstans()->TriggerKey(DIK_RIGHT) && (int)prePos_.x <= WIDTH - 3)
	{
		pField_->SetBlockType((int)prePos_.x + 2, (int)prePos_.y, (int)prePos_.z, 2);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
	}
	else if (Input::GetInstans()->PushKey(DIK_P) && Input::GetInstans()->TriggerKey(DIK_LEFT) && (int)prePos_.x >= 2)
	{
		pField_->SetBlockType((int)prePos_.x - 2, (int)prePos_.y, (int)prePos_.z, 2);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
	}
	else if (Input::GetInstans()->PushKey(DIK_P) && Input::GetInstans()->TriggerKey(DIK_UP) && (int)prePos_.z >= 2)
	{
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z - 2, 2);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
	}
	else if (Input::GetInstans()->PushKey(DIK_P) && Input::GetInstans()->TriggerKey(DIK_DOWN) && (int)prePos_.z <= DEPTH - 3)
	{
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z + 2, 2);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
	}

	// ドリブル
	else if (Input::GetInstans()->TriggerKey(DIK_RIGHT) && (int)prePos_.x >= 0 && (int)prePos_.x <= WIDTH - 2)
	{
		pField_->SetBlockType((int)prePos_.x + 1, (int)prePos_.y, (int)prePos_.z, 2);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
	}
	else if (Input::GetInstans()->TriggerKey(DIK_LEFT) && (int)prePos_.x >= 1 && (int)prePos_.x <= WIDTH-1)
	{
		pField_->SetBlockType((int)prePos_.x - 1, (int)prePos_.y, (int)prePos_.z, 2);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
	}
	else if (Input::GetInstans()->TriggerKey(DIK_UP) && (int)prePos_.z >= 1 && (int)prePos_.z <= DEPTH - 1)
	{
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z - 1, 2);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
	}
	else if (Input::GetInstans()->TriggerKey(DIK_DOWN) && (int)prePos_.z >= 0 && (int)prePos_.z <= DEPTH - 2)
	{
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z + 1, 2);
		pField_->SetBlockType((int)prePos_.x, (int)prePos_.y, (int)prePos_.z, 0);
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

		ImGui::SliderFloat3("cameraPos", &cameraPos_.x, -50.0f, 50.0f);	
		ImGui::SliderFloat3("cameraRot", &cameraRot_.x, -3.0f, 3.0f);

		pField_->ImGui();
	}

#endif // _DEBUG
}

void GamePlayScene::Draw()
{
#pragma region 3Dオブジェクト描画

	//3dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	Object3DCommon::GetInstance()->CommonDraw();

	//player->Draw();
	pField_->Draw();

#pragma endregion


#pragma region スプライト描画

	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();

#pragma endregion

}
