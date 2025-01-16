#pragma once
#include "Model.h"
#include "ModelManager.h"
#include "Input.h"
#include "Object3D.h"
#include "Object3DCommon.h"
#include "Camera.h"
class Player
{
public:
	Player();
	~Player();
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了
	void Finalize();

private:
	Model* model;
	Object3D* object3D_;

	struct PlayerData
	{
		Vector3 position;
		Vector3 rotate;
		Vector3 scale;
	};
	PlayerData playerData;
	//Camera* camera;
};

