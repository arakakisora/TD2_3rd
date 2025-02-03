#pragma once
#include "Model.h"
#include "Object3D.h"
#include "ModelManager.h"
#include "Object3DCommon.h"
#include "imgui.h"
class Ball
{
public:
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// 終了
	void Finalize();
	// ImGui
	void ImGui();

public:
	// ボールの位置を取得
	Vector3 GetPosition() { return ballData.position; }
	// ボールの位置を設定
	void SetPosition(Vector3 pos) { ballData.position = pos; }
	// ボールの回転を取得
	Vector3 GetRotate() { return ballData.rotate; }
	// ボールの回転を設定
	void SetRotate(Vector3 rot) { ballData.rotate = rot; }
	// ボールのスケールを取得
	Vector3 GetScale() { return ballData.scale; }
	// ボールのスケールを設定
	void SetScale(Vector3 scale) { ballData.scale = scale; }
private:
	// ボールデータ
	struct BallData
	{
		Vector3 position;
		Vector3 rotate;
		Vector3 scale;
	};
	// 3Dオブジェクト / スマートポインタ
	std::unique_ptr<Object3D> object3D_;
	// ボールデータ
	BallData ballData;

	int posX = 0;
	int posY = 0;
	int posZ;

};

