#pragma once

#include "BaseScene.h"
#include"Sprite.h"

class GameClearScene:public BaseScene
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

	Sprite* clearSprite_ = nullptr;
	Sprite* blackSprite_ = nullptr;

	bool isFadeStart_ = false;
	bool isChangeScene_ = false;

	// 透明度
	float alpha_ = 0.0f;

};

