#include "MyMath.h"
#include <stdexcept>



Matrix4x4 MyMath::MakeTranslateMatrix(const Vector3& translate)

{
	Matrix4x4 ans;

	ans.m[0][0] = 1;
	ans.m[0][1] = 0;
	ans.m[0][2] = 0;
	ans.m[0][3] = 0;

	ans.m[1][0] = 0;
	ans.m[1][1] = 1;
	ans.m[1][2] = 0;
	ans.m[1][3] = 0;

	ans.m[2][0] = 0;
	ans.m[2][1] = 0;
	ans.m[2][2] = 1;
	ans.m[2][3] = 0;

	ans.m[3][0] = translate.x;
	ans.m[3][1] = translate.y;
	ans.m[3][2] = translate.z;
	ans.m[3][3] = 1;

	return ans;

}


Matrix4x4 MyMath::MakeScaleMatrix(const Vector3& scale)

{
	Matrix4x4 ans;

	ans.m[0][0] = scale.x;
	ans.m[0][1] = 0;
	ans.m[0][2] = 0;
	ans.m[0][3] = 0;

	ans.m[1][0] = 0;
	ans.m[1][1] = scale.y;
	ans.m[1][2] = 0;
	ans.m[1][3] = 0;

	ans.m[2][0] = 0;
	ans.m[2][1] = 0;
	ans.m[2][2] = scale.z;
	ans.m[2][3] = 0;

	ans.m[3][0] = 0;
	ans.m[3][1] = 0;
	ans.m[3][2] = 0;
	ans.m[3][3] = 1;
	return ans;

}


Vector3 MyMath::Transform(const Vector3& vector, const Matrix4x4& matrix)

{

	Vector3 ans;

	ans.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	ans.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	ans.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	assert(w != 0.0f);
	ans.x /= w;
	ans.y /= w;
	ans.z /= w;
	return ans;

}


Matrix4x4 MyMath::MakeRotateXMatrix(float radian)

{

	Matrix4x4 ans;
	ans.m[0][0] = 1;
	ans.m[0][1] = 0;
	ans.m[0][2] = 0;
	ans.m[0][3] = 0;

	ans.m[1][0] = 0;
	ans.m[1][1] = std::cos(radian);;
	ans.m[1][2] = std::sin(radian);;
	ans.m[1][3] = 0;

	ans.m[2][0] = 0;
	ans.m[2][1] = -std::sin(radian);;
	ans.m[2][2] = std::cos(radian);;
	ans.m[2][3] = 0;

	ans.m[3][0] = 0;
	ans.m[3][1] = 0;
	ans.m[3][2] = 0;
	ans.m[3][3] = 1;

	return ans;

}


Matrix4x4 MyMath::MakeRotateYMatrix(float radian)

{

	Matrix4x4 ans;
	ans.m[0][0] = std::cos(radian);
	ans.m[0][1] = 0;
	ans.m[0][2] = -std::sin(radian);
	ans.m[0][3] = 0;

	ans.m[1][0] = 0;
	ans.m[1][1] = 1;
	ans.m[1][2] = 0;
	ans.m[1][3] = 0;

	ans.m[2][0] = std::sin(radian);;
	ans.m[2][1] = 0;
	ans.m[2][2] = std::cos(radian);;
	ans.m[2][3] = 0;

	ans.m[3][0] = 0;
	ans.m[3][1] = 0;
	ans.m[3][2] = 0;
	ans.m[3][3] = 1;

	return ans;

}


Matrix4x4 MyMath::MakeRotateZMatrix(float radian)

{
	Matrix4x4 ans;
	ans.m[0][0] = std::cos(radian);
	ans.m[0][1] = std::sin(radian);
	ans.m[0][2] = 0;
	ans.m[0][3] = 0;

	ans.m[1][0] = -std::sin(radian);
	ans.m[1][1] = std::cos(radian);
	ans.m[1][2] = 0;
	ans.m[1][3] = 0;

	ans.m[2][0] = 0;
	ans.m[2][1] = 0;
	ans.m[2][2] = 1;
	ans.m[2][3] = 0;

	ans.m[3][0] = 0;
	ans.m[3][1] = 0;
	ans.m[3][2] = 0;
	ans.m[3][3] = 1;


	return ans;

}

Matrix4x4 MyMath::MakeRotateMatrix(const Vector3& rotate)
{
	// 各軸の回転行列を計算
	Matrix4x4 rotateX = MakeRotateXMatrix(rotate.x); // X軸回転
	Matrix4x4 rotateY = MakeRotateYMatrix(rotate.y); // Y軸回転
	Matrix4x4 rotateZ = MakeRotateZMatrix(rotate.z); // Z軸回転

	// 合成回転行列 (順序: Z → Y → X)
	return rotateZ * rotateY * rotateX;
}

Matrix4x4 MyMath::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{

	Matrix4x4 rotateXYZ = MakeRotateXMatrix(rotate.x)* MakeRotateYMatrix(rotate.y)* MakeRotateZMatrix(rotate.z);
	return MakeScaleMatrix(scale)* rotateXYZ* MakeTranslateMatrix(translate);
	
}

float MyMath::Cot(float theta)
{
	return 1 / std::tan(theta);
}

Matrix4x4 MyMath::MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearCilp, float farClip)
{

	Matrix4x4 ans;

	ans.m[0][0] = Cot(fovY / 2) / aspectRatio;
	ans.m[0][1] = 0;
	ans.m[0][2] = 0;
	ans.m[0][3] = 0;

	ans.m[1][0] = 0;
	ans.m[1][1] = Cot(fovY / 2);
	ans.m[1][2] = 0;
	ans.m[1][3] = 0;

	ans.m[2][0] = 0;
	ans.m[2][1] = 0;
	ans.m[2][2] = nearCilp / (farClip - nearCilp);
	ans.m[2][3] = 1;

	ans.m[3][0] = 0;
	ans.m[3][1] = 0;
	ans.m[3][2] = -(nearCilp + nearCilp) / (farClip - nearCilp);
	ans.m[3][3] = 0;

	return ans;

}
Matrix4x4 MyMath::MakeOrthographicMatrix(float left, float top, float right, float bottm, float nearCip, float farCip)
{

	Matrix4x4 ans;

	ans.m[0][0] = 2 / (right - left);
	ans.m[0][1] = 0;
	ans.m[0][2] = 0;
	ans.m[0][3] = 0;

	ans.m[1][0] = 0;
	ans.m[1][1] = 2 / (top - bottm);
	ans.m[1][2] = 0;
	ans.m[1][3] = 0;

	ans.m[2][0] = 0;
	ans.m[2][1] = 0;
	ans.m[2][2] = 1 / (farCip - nearCip);
	ans.m[2][3] = 0;

	ans.m[3][0] = (left + right) / (left - right);
	ans.m[3][1] = (top + bottm) / (bottm - top);
	ans.m[3][2] = nearCip / (nearCip - farCip);
	ans.m[3][3] = 1;

	return ans;

}

Matrix4x4 MyMath::MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	Matrix4x4 ans;



	ans.m[0][0] = width / 2;
	ans.m[0][1] = 0;
	ans.m[0][2] = 0;
	ans.m[0][3] = 0;

	ans.m[1][0] = 0;
	ans.m[1][1] = -(height / 2);
	ans.m[1][2] = 0;
	ans.m[1][3] = 0;

	ans.m[2][0] = 0;
	ans.m[2][1] = 0;
	ans.m[2][2] = maxDepth - minDepth;
	ans.m[2][3] = 0;

	ans.m[3][0] = left + (width / 2);
	ans.m[3][1] = top + (height / 2);
	ans.m[3][2] = minDepth;
	ans.m[3][3] = 1;




	return ans;

}

Matrix4x4 MyMath::Inverse(const Matrix4x4& matrix)
{
	Matrix4x4 result;
	float det = 0;

	for (int i = 0; i < 4; i++)
		det += (matrix.m[0][i] * (matrix.m[1][(i + 1) % 4] * matrix.m[2][(i + 2) % 4] * matrix.m[3][(i + 3) % 4]
			- matrix.m[1][(i + 3) % 4] * matrix.m[2][(i + 2) % 4] * matrix.m[3][(i + 1) % 4]));

	if (det == 0)
		throw std::runtime_error("Matrix is not invertible");

	float invDet = 1.0f / det;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[j][i] = ((matrix.m[(i + 1) % 4][(j + 1) % 4] * matrix.m[(i + 2) % 4][(j + 2) % 4] * matrix.m[(i + 3) % 4][(j + 3) % 4])
				- (matrix.m[(i + 1) % 4][(j + 3) % 4] * matrix.m[(i + 2) % 4][(j + 2) % 4] * matrix.m[(i + 3) % 4][(j + 1) % 4])) * invDet;
		}
	}

	return result;
}











