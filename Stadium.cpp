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
	transform.translate = Vector3(0.2f, -0.2f, -2.7f);
	transform.rotate = Vector3(3.0f, 0.0f, 0.0f);
	transform.scale = Vector3(0.4f, 1.0f, 1.0f);

	ModelManager::GetInstans()->LoadModel("stadium.obj");
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize(Object3DCommon::GetInstance());
	object3D_->SetModel("stadium.obj");
	object3D_->SetTransform(transform);

	// 2つ目のスタジアム
	transform2.translate = Vector3(8.7f, 0.0f, 2.1f);
	transform2.rotate = Vector3(3.14f,-1.49f,0);
	transform2.scale = Vector3(0.3f, 1.0f, 0.4f);

	ModelManager::GetInstans()->LoadModel("stadium.obj");
	object3D2_ = std::make_unique<Object3D>();
	object3D2_->Initialize(Object3DCommon::GetInstance());
	object3D2_->SetModel("stadium.obj");
	object3D2_->SetTransform(transform2);

	// 3つ目のスタジアム
	transform3.translate = Vector3(-2.4f,0.0f,2.1f);
	transform3.rotate = Vector3(3.14f, 1.49f, 0.0f);
	transform3.scale = Vector3(0.3f, 1.0f, 0.4f);

	ModelManager::GetInstans()->LoadModel("stadium.obj");
	object3D3_ = std::make_unique<Object3D>();
	object3D3_->Initialize(Object3DCommon::GetInstance());
	object3D3_->SetModel("stadium.obj");
	object3D3_->SetTransform(transform3);

	// 4つ目のスタジアム
	transform4.translate = Vector3(6.0f, -0.2f, -2.7f);
	transform4.rotate = Vector3(3.0f, 0.0f, 0.0f);
	transform4.scale = Vector3(0.4f, 1.0f, 1.0f);
	
	ModelManager::GetInstans()->LoadModel("stadium.obj");
	object3D4_ = std::make_unique<Object3D>();
	object3D4_->Initialize(Object3DCommon::GetInstance());
	object3D4_->SetModel("stadium.obj");
	object3D4_->SetTransform(transform4);




}

void Stadium::Update()
{
	ImGui();
	object3D_->Update();
	object3D2_->Update();
	object3D3_->Update();
	object3D4_->Update();
}

void Stadium::Draw()
{
	object3D_->Draw();
	object3D2_->Draw();
	object3D3_->Draw();
	object3D4_->Draw();
}

void Stadium::Finalize()
{
}

void Stadium::ImGui()
{
#ifdef _DEBUG

	ImGui::Text("Stadium");
	ImGui::DragFloat3("stadiumPos", &transform.translate.x, 0.1f);
	ImGui::DragFloat3("stadiumPos2", &transform2.translate.x, 0.1f);
	ImGui::DragFloat3("stadiumPos3", &transform3.translate.x, 0.1f);
	ImGui::DragFloat3("stadiumPos4", &transform4.translate.x, 0.1f);
	ImGui::DragFloat3("rot", &transform.rotate.x, 0.01f);
	ImGui::DragFloat3("rot2", &transform2.rotate.x, 0.01f);
	ImGui::DragFloat3("rot3", &transform3.rotate.x, 0.01f);
	ImGui::DragFloat3("rot4", &transform4.rotate.x, 0.01f);
	ImGui::DragFloat3("scale", &transform.scale.x, 0.1f);
	ImGui::DragFloat3("scale2", &transform2.scale.x, 0.1f);
	ImGui::DragFloat3("scale3", &transform3.scale.x, 0.1f);
	ImGui::DragFloat3("scale4", &transform4.scale.x, 0.1f);

	object3D_->SetTransform(transform);
	object3D2_->SetTransform(transform2);
	object3D3_->SetTransform(transform3);
	object3D4_->SetTransform(transform4);
#endif // _DEBUG



}
