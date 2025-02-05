#pragma once

#include "Camera.h"	
#include "Model.h"
#include"Sprite.h"
#include "Object3D.h"
#include "Audio.h"
#include "BaseScene.h"
#include"Player.h"
#include "SceneManager.h"
#include "vector"
#include "Field.h"
#include "application/characters/enemy/Enemy.h"
#include "application/characters/enemy/EnemyManager.h"
#include "Ball.h"
#include "PlayerManager.h"  // 追加
#include "skydome.h"
#include "Stadium.h"

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

	// 切り替え演出
	void Fade();
	

private:
	//ターン
	enum class TurnState
	{
		NONE,
		PLAYER,
		ENEMY,
	};
	TurnState turnState_ = TurnState::PLAYER;

	// Player


	Player* selectedPlayer_ = nullptr;


	
	std::unique_ptr<PlayerManager> playerManager_;  // 追加


	//カメラのポインタ
	Camera* pCamera_ = nullptr;
	Vector3 cameraPos_ = Vector3(3.0f, -13.88f, 8.334f);
	Vector3 cameraRot_ = Vector3(-2.015f, 0.0f, 0.0f);
	// 3Dオブジェクト
	std::vector<Object3D*> pFieldObject_ = {};
	// Field
	std::unique_ptr<Field> pField_ = nullptr;
	//エネミーマネージャー
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;
	//天球
	std::unique_ptr<skydome> skydome_;
	// テスト用
	std::vector<Vector3> prePos_ = {};

	// プレイヤーの位置
	std::vector<Field::Pos> playerPosList_ = {};

	// 観客席
	std::unique_ptr<Stadium> stadium_ = nullptr;

	Ball* ball = nullptr;

	Vector3 mousePos = Vector3(0, 0, 0);
	Object3D* MouseObject = nullptr;

	Sprite* whiteSprite_ = nullptr;
	Sprite* blackSprite_ = nullptr;

	bool isSceneStart_ = true;
	bool isClearFadeStart_ = false;
	bool isGameOverFadeStart_ = false;
	// 透明度
	float blackAlpha_ = 0.0f;
	float whiteAlpha_ = 0.0f;
	// bgmのサウンドデータ
	SoundData bgm_;
	bool isBgmPlay_ = false;
};

