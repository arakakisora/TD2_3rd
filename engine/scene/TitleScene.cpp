#include "TitleScene.h"
#include "Object3DCommon.h"
#include "SpriteCommon.h"
#include "Input.h"
#include "SceneManager.h"
#include "ImGuiManager.h"
#include <imgui.h>

#include "CameraManager.h"
#include "Easing.h"
#include "ModelManager.h"

void TitleScene::Initialize()
{
	//モデル読み込み
	ModelManager::GetInstans()->LoadModel("title_left.obj");
	ModelManager::GetInstans()->LoadModel("title_right.obj");

	//カメラの生成
	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({ 0.0f,0.0f,0.0f });
	camera_->SetTranslate({ 0.0f,0.0f,-10.0f });
	//カメラの登録
	CameraManager::GetInstans()->AddCamera("title",camera_.get());
	CameraManager::GetInstans()->SetActiveCamera("title");

	//タイトルOBJの左
	titleLeft_ = std::make_unique<Object3D>();
	titleLeft_->Initialize(Object3DCommon::GetInstance());
	titleLeft_->SetModel("title_left.obj");
	titleLeft_->SetTranslate({ -1.0f,0.0f,5.0f });

	//タイトルOBJの右
	titleRight_ = std::make_unique<Object3D>();
	titleRight_->Initialize(Object3DCommon::GetInstance());
	titleRight_->SetModel("title_right.obj");
	titleRight_->SetTranslate({ 2.2f,0.0f,5.0f });
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	//カメラのアップデート
	CameraManager::GetInstans()->GetActiveCamera()->Update();

	if (Input::GetInstans()->TriggerKey(DIK_SPACE)) 
	{
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

	//ImGuiの更新
	UpdateImGui();

	//タイトルOBJの更新
	UpdateTitleObj();

	//OBJの更新
	titleLeft_->Update();
	titleRight_->Update();

}

void TitleScene::Draw()
{
	//3dオブジェクトの描画準備。3Dオブジェクトの描画に共通のグラフィックスコマンドを積む
	Object3DCommon::GetInstance()->CommonDraw();

	titleLeft_->Draw();
	titleRight_->Draw();

	//Spriteの描画準備。spriteの描画に共通のグラフィックスコマンドを積む
	SpriteCommon::GetInstance()->CommonDraw();

}

void TitleScene::UpdateImGui()
{
	ImGui::Begin("title");
	//カメラの操作
	#pragma region カメラの操作
	Vector3 cameraPos = CameraManager::GetInstans()->GetActiveCamera()->GetTransform().translate;
	ImGui::DragFloat3("cameraPos", &cameraPos.x, 0.1f);
	CameraManager::GetInstans()->GetActiveCamera()->SetTranslate(cameraPos);
	Vector3 cameraRot = CameraManager::GetInstans()->GetActiveCamera()->GetTransform().rotate;
	ImGui::DragFloat3("cameraRot", &cameraRot.x, 0.1f);
	CameraManager::GetInstans()->GetActiveCamera()->SetRotate(cameraRot);
	#pragma endregion

	//波形
	#pragma region 波形
	ImGui::SeparatorText("wave");
	ImGui::DragFloat("amplitude", &amplitude_, 0.1f);
	ImGui::DragFloat("frequency", &frequency_, 0.1f);
	#pragma endregion

	//イージング
	#pragma region イージング
	if (ImGui::CollapsingHeader("Easing", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("duration", &duration_, 0.1f);
		ImGui::SeparatorText("left");
		ImGui::DragFloat3("startTitleLeftPos", &startTitleLeftPos.x, 0.1f);
		ImGui::DragFloat3("targetTitleLeftPos", &targetTitleLeftPos.x, 0.1f);
		ImGui::SeparatorText("right");
		ImGui::DragFloat3("startTitleRightPos", &startTitleRightPos.x, 0.1f);
		ImGui::DragFloat3("targetTitleRightPos", &targetTitleRightPos.x, 0.1f);
	}
	#pragma endregion

	//タイトルOBJの操作
	#pragma region タイトルOBJの操作
	//左
	if (ImGui::CollapsingHeader("titleLeft", ImGuiTreeNodeFlags_DefaultOpen))
	{
		Vector3 titleLeftPos = titleLeft_->GetTransform().translate;
		ImGui::DragFloat3("titleLeftPos", &titleLeftPos.x, 0.1f);
		titleLeft_->SetTranslate(titleLeftPos);
		Vector3 titleLeftRot = titleLeft_->GetTransform().rotate;
		ImGui::DragFloat3("titleLeftRot", &titleLeftRot.x, 0.1f);
		titleLeft_->SetRotate(titleLeftRot);
		Vector3 titleLeftScale = titleLeft_->GetTransform().scale;
		ImGui::DragFloat3("titleLeftScale", &titleLeftScale.x, 0.1f);
		titleLeft_->SetScale(titleLeftScale);
	}
	if (ImGui::CollapsingHeader("titleRight", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//右
		Vector3 titleRightPos = titleRight_->GetTransform().translate;
		ImGui::DragFloat3("titleRightPos", &titleRightPos.x, 0.1f);
		titleRight_->SetTranslate(titleRightPos);
		Vector3 titleRightRot = titleRight_->GetTransform().rotate;
		ImGui::DragFloat3("titleRightRot", &titleRightRot.x, 0.1f);
		titleRight_->SetRotate(titleRightRot);
		Vector3 titleRightScale = titleRight_->GetTransform().scale;
		ImGui::DragFloat3("titleRightScale", &titleRightScale.x, 0.1f);
		titleRight_->SetScale(titleRightScale);
	}
	#pragma endregion

	ImGui::End();
}

void TitleScene::UpdateTitleObj()
{
	const float deltaTime = 1.0f / 60.0f;
	elapsedTime_ += deltaTime;
	if (elapsedTime_ < duration_)
	{
		//進行具合
		float progress = elapsedTime_ / duration_;
		titleLeft_->SetTranslate(Vector3(
			EasingToEnd(startTitleLeftPos.x,targetTitleLeftPos.x,EaseOutBounce<float>,progress),
			EasingToEnd(startTitleLeftPos.y, targetTitleLeftPos.y, EaseOutBounce<float>, progress),
			EasingToEnd(startTitleLeftPos.z, targetTitleLeftPos.z, EaseOutBounce<float>, progress))
		);
		titleRight_->SetTranslate(Vector3(
			EasingToEnd(startTitleRightPos.x, targetTitleRightPos.x, EaseOutBounce<float>, progress),
			EasingToEnd(startTitleRightPos.y, targetTitleRightPos.y, EaseOutBounce<float>, progress),
			EasingToEnd(startTitleRightPos.z, targetTitleRightPos.z, EaseOutBounce<float>, progress))
		);
	}
	else
	{
		waitTime_ -= deltaTime;
		if (waitTime_ <= 0.0f)
		{
			elapsedTime_ = 0.0f;
			waitTime_ = kWaitTime;
		}
	}
}
