#include "Stadium.h"
#include "Object3DCommon.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include <imgui.h>
Stadium::Stadium()
{
}

Stadium::~Stadium()
{
}

void Stadium::Initialize()
{
	// 1つ目のスタジアム
	transform.scale = Vector3(0.7f, 1.0f, 1.0f);
	transform.rotate = Vector3(3.0f, 0.0f, 0.0f);
	transform.translate = Vector3(3.0f, -0.3f, -2.4f);

	ModelManager::GetInstans()->LoadModel("stadium.obj");
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize(Object3DCommon::GetInstance());
	object3D_->SetModel("stadium.obj");
	object3D_->SetTransform(transform);

	// 2つ目のスタジアム
	transform2.translate = Vector3(7.7f, 0.3f, 2.1f);
	transform2.rotate = Vector3(3.0f,-1.53f,0);
	transform2.scale = Vector3(0.3f, 1.0f, 0.4f);

	ModelManager::GetInstans()->LoadModel("stadium.obj");
	object3D2_ = std::make_unique<Object3D>();
	object3D2_->Initialize(Object3DCommon::GetInstance());
	object3D2_->SetModel("stadium.obj");
	object3D2_->SetTransform(transform2);

	// 3つ目のスタジアム
	transform3.translate = Vector3(-1.4f,-1.0f,2.5f);
	transform3.rotate = Vector3(3.0f, 1.54f, 0.0f);
	transform3.scale = Vector3(0.3f, 1.0f, 0.4f);

	ModelManager::GetInstans()->LoadModel("stadium.obj");
	object3D3_ = std::make_unique<Object3D>();
	object3D3_->Initialize(Object3DCommon::GetInstance());
	object3D3_->SetModel("stadium.obj");
	object3D3_->SetTransform(transform3);



}

void Stadium::Update()
{
	ImGui();
	object3D_->Update();
	object3D2_->Update();
	object3D3_->Update();
}

void Stadium::Draw()
{
	object3D_->Draw();
	object3D2_->Draw();
	object3D3_->Draw();
}

void Stadium::Finalize()
{
}

void Stadium::ImGui()
{
	ImGui::Text("Stadium");
	ImGui::DragFloat3("stadiumPos", &transform.translate.x, 0.1f);
	ImGui::DragFloat3("rot", &transform.rotate.x, 0.01f);
	ImGui::DragFloat3("scale", &transform.scale.x, 0.1f);
	object3D_->SetTransform(transform);

	ImGui::DragFloat3("stadiumPos2", &transform2.translate.x, 0.1f);
	ImGui::DragFloat3("rot2", &transform2.rotate.x, 0.01f);
	ImGui::DragFloat3("scale2", &transform2.scale.x, 0.1f);
	object3D2_->SetTransform(transform2);

	ImGui::DragFloat3("stadiumPos3", &transform3.translate.x, 0.1f);
	ImGui::DragFloat3("rot3", &transform3.rotate.x, 0.01f);
	ImGui::DragFloat3("scale3", &transform3.scale.x, 0.1f);
	object3D3_->SetTransform(transform3);
}
