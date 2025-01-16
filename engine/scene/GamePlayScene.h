#pragma once

#include "Camera.h"	
#include "Model.h"
#include"Sprite.h"
#include "Object3D.h"
#include "Audio.h"
#include "BaseScene.h"
#include"Player.h"
#include "SceneManager.h"

#include "Field.h"

class GamePlayScene :public BaseScene
{
public:

	/// <summary>
	/// シーンの初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// シーンの終了処理
	/// </summary>
	void Finalize()override;
	/// <summary>
	/// シーンの更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// シーンの描画
	/// </summary>
	void Draw()override;

private:	
	// Player
	Player* player;

	//カメラのポインタ
	Camera* pCamera_ = nullptr;
	Vector3 cameraPos_ = Vector3(0.0f, 0.0f, -20.0f);
	Vector3 cameraRot_ = Vector3(0.0f, 0.0f, 0.0f);


	// 3Dオブジェクト
	std::vector<Object3D*> pFieldObject_ = {};

	// Field
	std::unique_ptr<Field> pField_ = nullptr;
	
};

