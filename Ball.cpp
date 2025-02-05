#include "Ball.h"

void Ball::Initialize()
{
	// ボールの初期化
	ballData.position = Vector3(float(posX), 0.0f, 10.0f);
	ballData.rotate = Vector3(0.0f, 0.0f, 0.0f);
	ballData.scale = Vector3(0.5f, 0.5f, 0.5f);
	ModelManager::GetInstans()->LoadModel("ball.obj");
	// モデルの生成
	object3D_ = new Object3D;
	object3D_->SetModel("ball.obj");
	object3D_->Initialize(Object3DCommon::GetInstance());
	object3D_->SetTranslate(ballData.position);
	object3D_->SetRotate(ballData.rotate);
	object3D_->SetScale(ballData.scale);
	object3D_->SetDirectionalLightDirection({ 3.0f,13.0f,-8.80f });
	object3D_->SetLighting(true);

}

void Ball::Update()
{
	// ImGui
	ImGui();
	// モデル座標の更新
	object3D_->SetTranslate(ballData.position);
	object3D_->SetRotate(ballData.rotate);
	object3D_->SetScale(ballData.scale);
	object3D_->Update();
}

void Ball::Draw()
{
	object3D_->Draw();
}

void Ball::Finalize()
{
	delete object3D_;
	delete model;
}

void Ball::ImGui()
{
	ImGui::Begin("ball");
	ImGui::DragFloat3("position", &ballData.position.x, 0.1f);
	ImGui::DragFloat3("scale", &ballData.scale.x, 0.1f);
	ImGui::DragFloat3("rotate", &ballData.rotate.x, 0.1f);
	// ライトの調整
	static Vector3 lightDirection = object3D_->GetDirectionalLight().direction;
	static Vector4 lightColor = object3D_->GetDirectionalLight().color;
	static float lightIntensity = object3D_->GetDirectionalLight().intensity;

	if (ImGui::DragFloat3("Light Direction", &lightDirection.x, 0.1f)) {
		object3D_->SetDirectionalLightDirection(lightDirection);
	}
	if (ImGui::ColorEdit4("Light Color", &lightColor.x)) {
		object3D_->SetDirectionalLightColor(lightColor);
	}
	if (ImGui::DragFloat("Light Intensity", &lightIntensity, 0.1f, 0.0f, 10.0f)) {
		object3D_->SetDirectionalLightIntensity(lightIntensity);
	}

	

	ImGui::End();
}
