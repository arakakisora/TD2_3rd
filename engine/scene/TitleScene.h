#pragma once
#include "Camera.h"	
#include "Model.h"
#include"Sprite.h"
#include "Object3D.h"
#include "Audio.h"
#include "BaseScene.h"
#include "Player.h"
#include "skydome.h"

class TitleScene :public BaseScene
{

public:

	/// <summary>
	/// シーンの初期化
	/// </summary>
	void Initialize()override;
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
	//ImGuiの更新
	void UpdateImGui();
	//タイトルOBJをイージングで動かす
	void UpdateTitleObj();

public:
	//カメラ
	std::unique_ptr<Camera> camera_;
	//タイトルOBJの左
	std::unique_ptr<Object3D> titleLeft_;
	//タイトルOBJの右
	std::unique_ptr<Object3D> titleRight_;
	//天球
	std::unique_ptr<skydome> skydome_;
	/*-----------------[ イージング用変数 ]------------------*/
	//経過時間
	float elapsedTime_ = 0.0f;
	//継続時間
	float duration_ = 1.0f;
	//待機時間
	float waitTime_ = 4.0f;
	const float kWaitTime = waitTime_;
	//開始位置
	Vector3 startTitleLeftPos = { -8.0f,0.0f,5.0f };
	Vector3 startTitleRightPos = { 8.0f,0.0f,5.0f };

	//目標位置
	Vector3 targetTitleLeftPos = { -1.7f,0.0f,0.0f };
	Vector3 targetTitleRightPos = { 1.5f,0.0f,0.0f };
	/*-----------------[ 波形用変数 ]------------------*/
	//振幅
	float amplitude_ = 1.0f;
	//周波数
	float frequency_ = 1.0f;

};

