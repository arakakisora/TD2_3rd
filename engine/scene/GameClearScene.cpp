#include "GameClearScene.h"
#include "Object3DCommon.h"
#include "SpriteCommon.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include "Input.h"
#include "SceneManager.h"

void GameClearScene::Initialize()
{
	//スプライトの生成
	clearSprite_ = new Sprite();
	clearSprite_->Initialize(SpriteCommon::GetInstance(), "Resources/title.png");

	blackSprite_ = new Sprite();
	blackSprite_->Initialize(SpriteCommon::GetInstance(), "Resources/black.png");
	blackSprite_->setColor({ 1.0f,1.0f,1.0f,0.0f });

	isFadeStart_ = false;
	isChangeScene_ = false;

}

void GameClearScene::Finalize()
{
	delete clearSprite_;
	delete blackSprite_;
}

void GameClearScene::Update()
{
	//スプライトの更新
	clearSprite_->Update();
	blackSprite_->Update();

	Fade();

	if (Input::GetInstans()->TriggerKey(DIK_SPACE))
	{
		isFadeStart_ = true;
	}

	if (isChangeScene_)
	{
		SceneManager::GetInstance()->ChangeScene("TITELE");
	}



#ifdef _DEBUG

	if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("gameClearScene");
		if (ImGui::Button("TitleScene"))
		{
			SceneManager::GetInstance()->ChangeScene("TITELE");
		}

	}

#endif // _DEBUG


}

void GameClearScene::Draw()
{
#pragma region 3Dオブジェクト描画

	//3dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	Object3DCommon::GetInstance()->CommonDraw();


#pragma endregion


#pragma region スプライト描画
	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();

	clearSprite_->Draw();
	blackSprite_->Draw();

#pragma endregion

}

void GameClearScene::Fade()
{
	blackSprite_->setColor({ 1.0f,1.0f,1.0f,alpha_ });

	if (isFadeStart_)
	{
		alpha_ += 0.01f;
	}

	if (alpha_ >= 1.0f)
	{
		isChangeScene_ = true;
	}
}
