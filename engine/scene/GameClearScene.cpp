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
	clearSprite_->Initialize(SpriteCommon::GetInstance(), "Resources/clear.png");

	whiteSprite_ = new Sprite();
	whiteSprite_->Initialize(SpriteCommon::GetInstance(), "Resources/black.png");
	whiteSprite_->SetSize({ 1280.0f,720.0f });
	whiteSprite_->setColor({ 1.0f,1.0f,1.0f,1.0f });

	blackSprite_ = new Sprite();
	blackSprite_->Initialize(SpriteCommon::GetInstance(), "Resources/black.png");
	blackSprite_->SetSize({ 1280.0f,720.0f });
	blackSprite_->setColor({ 1.0f,1.0f,1.0f,0.0f });

	isSceneStart_ = true;
	isFadeStart_ = false;
	whiteAlpha_ = 1.0f;
	blackAlpha_ = 0.0f;
}

void GameClearScene::Finalize()
{
	delete clearSprite_;
	delete whiteSprite_;
	delete blackSprite_;
}

void GameClearScene::Update()
{
	//スプライトの更新
	clearSprite_->Update();
	whiteSprite_->Update();
	blackSprite_->Update();

	Fade();

	if (Input::GetInstans()->TriggerKey(DIK_SPACE))
	{
		isFadeStart_ = true;
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
	whiteSprite_->setColor({ 1.0f,1.0f,1.0f,whiteAlpha_ });
	blackSprite_->setColor({ 1.0f,1.0f,1.0f,blackAlpha_ });

	// シーンスタート
	if (isSceneStart_ && whiteAlpha_ > 0.01f)
	{
		whiteAlpha_ -= 0.01f;
		if (whiteAlpha_ <= 0.01f)
		{
			isSceneStart_ = false;
		}
	}

	// タイトルに戻る
	if (isFadeStart_)
	{
		blackAlpha_ += 0.01f;
	}
	if (blackAlpha_ >= 1.0f)
	{
		SceneManager::GetInstance()->ChangeScene("TITELE");
	}

}
