#pragma once
#include "Model.h"
#include "Object3D.h"
#include <memory>

class Stadium
{
public:
	Stadium();
	~Stadium();
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
	void ImGui();
private:

	//スタジアムの3Dオブジェクト
	std::unique_ptr<Object3D> object3D_;
	Transform transform;
	// スタジアムの3Dオブジェクト２
	std::unique_ptr<Object3D> object3D2_;
	Transform transform2;
	// スタジアムの3Dオブジェクト３
	std::unique_ptr<Object3D> object3D3_;
	Transform transform3;

};

