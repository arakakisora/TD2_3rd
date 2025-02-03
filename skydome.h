#pragma once

#include <Framework.h>

#include "Object3D.h"

class skydome
{
public:

	// 初期化
	void Initialize(Object3DCommon* objectCommon, const std::string& filename);

	// 終了
	void Finalize();

	// 更新
	void Update();

	// 描画
	void Draw();

private:

	std::unique_ptr<Object3D> object3D_;

};

