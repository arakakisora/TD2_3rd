#pragma once
#include "Field.h"
#include "Object3D.h"
#include <numbers>
#include <cmath>

#include "Player.h"

inline float EaseInSine(float x) { return 1.0f - std::cos((x * std::numbers::pi_v<float>) / 2.0f); }

class Enemy
{
public:
	// 初期化
	void Initialize(Object3DCommon* objectCommon, const std::string& filename);

	//更新
	void Update();

	//描画
	void Draw();

	//移動
	void Move(Vector3 distance);

public: //アクセッサ
	Vector3 GetPosition() const { return transform_.translate; }
	Vector3 GetRotation() const { return transform_.rotate; }
	Vector3 GetScale() const { return transform_.scale; }

	void SetPosition(const Vector3& pos) { transform_.translate = pos; }
	void SetRotation(const Vector3& rot) { transform_.rotate = rot; }
	void SetScale(const Vector3& scale) { transform_.scale = scale; }

	void SetField(Field* field) { field_ = field; }

	void SetPlayer(Player* player) { player_ = player; }

private:
	// 入力処理
	void HandleInput();

	// AI処理
	void HandleAI();

	// イージングによる移動更新
	void UpdateEasingMovement();

	//トランスフォームを３Dオブジェクトに適用する
	void UpdateTransform();

private:
	//フィールド
	Field* field_ = nullptr;

	//プレイヤー
	Player* player_ = nullptr;

	// 3Dオブジェクト
	std::unique_ptr<Object3D> object3D_;

	//トランスフォーム
	Transform transform_ = { { 0.5f,0.5f,0.5f }, { 0.0f,0.0f,0.0f }, {} };

	//移動経過時間
	float moveTimer_ = 0.0f;
	//移動間隔
	const float moveInterval_ = 2.0f;

	// 移動開始位置と目標位置
	Vector3 moveStartPosition_;
	Vector3 moveTargetPosition_;
	// 移動の進行度
	float moveProgress_ = 1.0f;
	//イージングの開始フラグ
	bool isEaseStart_ = false;
};

