#pragma once

#include "Camera.h"	
#include "Model.h"
#include"Sprite.h"
#include "Object3D.h"
#include "Audio.h"
#include "BaseScene.h"
#include "SceneManager.h"

#include "application/turn/BaseTurnState.h"

#include "application/objects/Field.h"
#include "application/characters/Enemy.h"
#include "application/characters/Player.h"

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

	// ターンステート
	void ChangeState(std::unique_ptr<BaseTurnState> _pState);

public: // ゲッター

	// プレイヤー (ターンステートで使いたい)
	std::shared_ptr<Player> GetPlayer() { return pPlayer_; }
	
	// エネミー (ターンステートで使いたい)
	std::shared_ptr<Enemy> GetEnemy() { return enemy_; }

	// フィールド (ターンステートで使いたい)
	std::shared_ptr<Field> GetField() { return pField_; }

private:	

	// Player
	std::shared_ptr<Player> pPlayer_;

	//カメラのポインタ
	Camera* pCamera_ = nullptr;
	Vector3 cameraPos_ = Vector3(3.0f, -20.0f, 0.0f);
	Vector3 cameraRot_ = Vector3(-1.5f, 0.0f, 0.0f);


	// 3Dオブジェクト
	std::vector<Object3D*> pFieldObject_ = {};

	// Field
	std::shared_ptr<Field> pField_ = nullptr;

	//エネミー
	std::shared_ptr<Enemy> enemy_ = nullptr;
	
	// ターンステート
	std::unique_ptr<BaseTurnState> pState_ = nullptr;

	// テスト用
	Vector3 prePos_{};
	Field::Pos prePlayerPos_{};

};

