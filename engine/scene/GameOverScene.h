#pragma once

#include "BaseScene.h"
#include"Sprite.h"
#include "Camera.h"	
#include <memory>

class GameOverScene :public BaseScene
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
	//カメラ
	std::unique_ptr<Camera> camera_;

	Sprite* gameOverSprite_ = nullptr;
	Sprite* blackSprite_ = nullptr;

	bool isSceneStart_ = true;
	bool isFadeStart_ = false;

	// 透明度
	float alpha_ = 0.0f;


};

