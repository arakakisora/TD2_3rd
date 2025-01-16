#pragma once
#include "Object3D.h"

class Enemy
{
public:
	// 初期化
	void Initialize(Object3DCommon* objectCommon, const std::string& filename);

	//更新
	void Update();

	//描画
	void Draw();

private:
	// 3Dオブジェクト
	std::unique_ptr<Object3D> object3D_;

};

