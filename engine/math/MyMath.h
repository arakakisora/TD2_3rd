#pragma once
#include "Matrix4x4.h"
#include "Matrix3x3.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Vector2.h"
#pragma once
#include <assert.h>
#include <cmath>
#include <stdio.h>
#include <vector>
#include <string>



namespace MyMath {
	//回転
	Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
	//拡大
	Matrix4x4 MakeScaleMatrix(const Vector3& scale);
	//同時座標変換
	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

	//回転X
	Matrix4x4 MakeRotateXMatrix(float radian);
	//回転Y
	Matrix4x4 MakeRotateYMatrix(float radian);
	//回転Z
	Matrix4x4 MakeRotateZMatrix(float radian);

	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	float Cot(float theta);

	//透視投影行列
	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearCilp, float farClip);

	//正射影行列
	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottm, float nearCip, float farCip);
	//ビューポート変換行列
	Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
}














