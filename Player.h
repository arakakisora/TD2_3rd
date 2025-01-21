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
public: //アクセッサ
	Vector3 GetPosition() { return playerData.position; }
	void SetPosition(Vector3 pos) { playerData.position = pos; }
	Vector3 GetRotate() { return playerData.rotate; }
	void SetRotate(Vector3 rot) { playerData.rotate = rot; }
	Vector3 GetScale() { return playerData.scale; }
	void SetScale(Vector3 scale) { playerData.scale = scale; }
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

