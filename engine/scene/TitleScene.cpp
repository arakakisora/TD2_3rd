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
	titleSprite_->Initialize(SpriteCommon::GetInstance(), "Resources/test.png");
	
	
	
}

void TitleScene::Finalize()
{
	delete titleSprite_;
}

void TitleScene::Update()
{
	//スプライトの更新
	titleSprite_->Update();

	if (Input::GetInstans()->TriggerKey(DIK_SPACE)) {

		
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		
	}

	if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("titleScene %d");
		if (ImGui::Button("gamePlayScene"))
		{
			SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		}
		


	}

}

void TitleScene::Draw()
{
	//3dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	Object3DCommon::GetInstance()->CommonDraw();

	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();

	//スプライトの描画
	titleSprite_->Draw();

}
