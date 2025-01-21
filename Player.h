#pragma once
#include "Model.h"
#include "ModelManager.h"
#include "Input.h"
#include "Object3D.h"
#include "Object3DCommon.h"
#include "Camera.h"
#include "input.h"

class Player
{
public:
	Player();
	~Player();
	// 初期化
	void Initialize(int posZ);
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了
	void Finalize();
	// 移動
	void Move(int WIDTH, int DEPTH);
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
	int posX = -3;
	int posY = 0;
	int posZ ;
};

