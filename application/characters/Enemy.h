#pragma once
#include "Field.h"
#include "Object3D.h"

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

private:
	//トランスフォームを３Dオブジェクトに適用する
	void UpdateTransform();

private:
	// 3Dオブジェクト
	std::unique_ptr<Object3D> object3D_;

	//トランスフォーム
	Transform transform_ = { { 0.5f,0.5f,0.5f }, { 0.0f,0.0f,0.0f }, {} };

	float moveTimer_ = 0.0f;
	const float moveInterval_ = 2.0f;
	Field* field_ = nullptr;
};

