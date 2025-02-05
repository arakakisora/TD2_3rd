#include "GameOverScene.h"
#include "GameClearScene.h"
#include "Object3DCommon.h"
#include "SpriteCommon.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include "Input.h"
#include "SceneManager.h"
#include "CameraManager.h"

void GameOverScene::Initialize()
{
	//カメラの生成
	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({ 0.0f,0.0f,0.0f });
	camera_->SetTranslate({ 0.0f,0.0f,-10.0f });
	//カメラの登録
	CameraManager::GetInstans()->AddCamera("gameOver", camera_.get());
	CameraManager::GetInstans()->SetActiveCamera("gameOver");

	//スプライトの生成
	gameOverSprite_ = new Sprite();
	gameOverSprite_->Initialize(SpriteCommon::GetInstance(), "Resources/gameover.png");

	blackSprite_ = new Sprite();
	blackSprite_->Initialize(SpriteCommon::GetInstance(), "Resources/black.png");
	blackSprite_->SetSize({ 1280.0f,720.0f });
	blackSprite_->setColor({ 1.0f,1.0f,1.0f,1.0f });

	isSceneStart_ = true;
	isFadeStart_ = false;

	alpha_ = 1.0f;

	bgm_ = Audio::GetInstance()->SoundLoadWave("Resources/audio/gameover/bgm.wav");
}

void GameOverScene::Finalize()
{
	CameraManager::GetInstans()->RemoveCamera("gameOver");
	

	delete gameOverSprite_;
	delete blackSprite_;
}

void GameOverScene::Update()
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

	gameOverSprite_->Update();
	blackSprite_->Update();

	Fade();


	if (Input::GetInstans()->TriggerKey(DIK_SPACE))
	{
		isFadeStart_ = true;
	}


#ifdef _DEBUG

	if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("gameOverScene");
		if (ImGui::Button("TitleScene"))
		{
			SceneManager::GetInstance()->ChangeScene("TITELE");
		}

		ImGui::SliderFloat("alpha", &alpha_, 0.0f, 1.0f);
	}

#endif // _DEBUG

}

void GameOverScene::Draw()
{
#pragma region 3Dオブジェクト描画

	//3dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	Object3DCommon::GetInstance()->CommonDraw();


#pragma endregion


#pragma region スプライト描画
	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();

	gameOverSprite_->Draw();
	blackSprite_->Draw();

#pragma endregion

}

void GameOverScene::Fade()
{
	blackSprite_->setColor({ 1.0f,1.0f,1.0f,alpha_ });

	// シーンスタート
	if (isSceneStart_ && alpha_ > 0.01f)
	{
		alpha_ -= 0.01f;
		if (alpha_ <= 0.01f)
		{
			isSceneStart_ = false;
		}
	}
	// タイトルに遷移
	if (isFadeStart_)
	{
		alpha_ += 0.01f;
		if (alpha_ >= 1.0f)
		{
			SceneManager::GetInstance()->ChangeScene("TITELE");
		}
	}
}
