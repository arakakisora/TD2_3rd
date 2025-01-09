#include "GamePlayScene.h"
#include <ModelManager.h>
#include "Object3DCommon.h"
#include "SpriteCommon.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include "Input.h"
#include "TitleScene.h"

void GamePlayScene::Initialize()
{
	//カメラの生成	
	pCamera_ = new Camera();
	pCamera_->SetRotate({ 0,0,0, });
	pCamera_->SetTranslate({ 0,0,-50, });
	Object3DCommon::GetInstance()->SetDefaultCamera(pCamera_);


	// 3Dオブジェクト
	ModelManager::GetInstans()->LoadModel("cube.obj");

	// フィールド

	for (int y = 0; y < 5; y++)
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

	
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 7; x++)
		{
			if (x == 0 or x == 6)
			{
				pField_->SetBlockType(x, y, 0, 1);
			}
		}
	}
	

}

void GamePlayScene::Finalize()
{

	pField_->Finalize();
	for (auto pFieldObject_ : pFieldObject_)
	{
		delete pFieldObject_;
	}

	delete pCamera_;
}

void GamePlayScene::Update()
{
	//カメラの更新
	pCamera_->Update();

	pField_->Update();

	pCamera_->SetTranslate(cameraPos_);
	pCamera_->SetRotate(cameraRot_);
	
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

		ImGui::SliderFloat3("cameraPos", &cameraPos_.x, -70.0f, 70.0f);	
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

	pField_->Draw();

#pragma endregion


#pragma region スプライト描画
	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();

#pragma endregion




}
