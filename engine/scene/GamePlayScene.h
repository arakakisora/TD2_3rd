#pragma once

#include "Camera.h"	
#include "Model.h"
#include"Sprite.h"
#include "Object3D.h"
#include "Audio.h"
#include "BaseScene.h"

#include "SceneManager.h"

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

	//サンプル
	Camera* sampleCamera = nullptr;//カメラ
	Object3D* sampleObj3d = nullptr;//3Dオブジェクト
	Sprite* sampleSprite = nullptr;//スプライト
	SoundData sampleSoundData;//サウンドデータ
	SoundData sampleSoundData2;//サウンドデータ

	//サウンド再生中かどうか
	bool soundPlaying = false;
	float playbackSpeed = 1.0f;
};

