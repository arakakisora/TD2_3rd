#pragma once
#include "Model.h"
#include "ModelManager.h"
#include "Input.h"
#include "Object3D.h"
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

	// プレイヤーの一手前の位置リスト
	std::vector<Vector2>beforePosition;
	const int ThousendDaysHand = 2;
	Vector3 preBeforePos;
	Vector3 postBeforePos;


};

