#include "TitleScene.h"
#include "Object3DCommon.h"
#include "SpriteCommon.h"
#include "Input.h"
#include "SceneManager.h"
#include "ImGuiManager.h"
#include <imgui.h>


void TitleScene::Initialize()
{
	
	//スプライトの生成
	titleSprite_ = new Sprite();
	titleSprite_->Initialize(SpriteCommon::GetInstance(),"Resources/title.png");

	blackSprite_ = new Sprite();
	blackSprite_->Initialize(SpriteCommon::GetInstance(),"Resources/black.png");
	blackSprite_->setColor({ 1.0f,1.0f,1.0f,0.0f });

	isFadeStart_ = false;
	isChangeScene_ = false;
}

void TitleScene::Finalize()
{
	delete titleSprite_;
	delete blackSprite_;
}

void TitleScene::Update()
{
	//スプライトの更新
	titleSprite_->Update();
	blackSprite_->Update();

	Fade();

	if (Input::GetInstans()->TriggerKey(DIK_SPACE)) 
	{	
		isFadeStart_ = true;
	}

	if (isChangeScene_)
	{
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}

#ifdef _DEBUG

	if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("titleScene %d");
		if (ImGui::Button("gamePlayScene"))
		{
			SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		}
	}


	ImGui::Text("TitleScene");

	ImGui::SliderFloat("alpha", &alpha_, 0.0f, 1.0f);

#endif // _DEBUG

}

void TitleScene::Draw()
{
	//3dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	Object3DCommon::GetInstance()->CommonDraw();

	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();

	//スプライトの描画
	titleSprite_->Draw();
	blackSprite_->Draw();

}

void TitleScene::Fade()
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
