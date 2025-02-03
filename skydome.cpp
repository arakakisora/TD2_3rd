#include "skydome.h"

void skydome::Initialize(Object3DCommon* objectCommon, const std::string& filename)
{
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize(objectCommon);
	object3D_->SetModel(filename);
	object3D_->SetScale({ 100.0f,100.0f,100.0f });
}

void skydome::Finalize()
{
}

void skydome::Update()
{
	object3D_->Update();
}

void skydome::Draw()
{
	object3D_->Draw();
}
