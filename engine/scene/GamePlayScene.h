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
#include "application/characters/enemy/Enemy.h"

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
	Player* pPlayer_;

	//カメラのポインタ
	Camera* pCamera_ = nullptr;
	Vector3 cameraPos_ = Vector3(3.0f, -20.0f, 0.0f);
	Vector3 cameraRot_ = Vector3(-1.5f, 0.0f, 0.0f);


	// 3Dオブジェクト
	std::vector<Object3D*> pFieldObject_ = {};

	// Field
	std::unique_ptr<Field> pField_ = nullptr;

	//エネミー
	std::unique_ptr<Enemy> enemy_ = nullptr;
	

	// テスト用
	Vector3 prePos_{};
	Field::Pos prePlayerPos_{};


	Vector3 mousePos = Vector3(0, 0, 0);
	Object3D* MouseObject = nullptr;
};

