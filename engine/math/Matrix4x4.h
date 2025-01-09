#pragma once

/// <summary>
/// 4x4行列
/// </summary>

struct Matrix4x4 {
	float m[4][4];

	Matrix4x4 operator+(const Matrix4x4& other) const {
		return Matrix4x4{
			m[0][0] + other.m[0][0],
			m[0][1] + other.m[0][1],
			m[0][2] + other.m[0][2],
			m[0][3] + other.m[0][3],
			m[1][0] + other.m[1][0],
			m[1][1] + other.m[1][1],
			m[1][2] + other.m[1][2],
			m[1][3] + other.m[1][3],
			m[2][0] + other.m[2][0],
			m[2][1] + other.m[2][1],
			m[2][2] + other.m[2][2],
			m[2][3] + other.m[2][3],
			m[3][0] + other.m[3][0],
			m[3][1] + other.m[3][1],
			m[3][2] + other.m[3][2],
			m[3][3] + other.m[3][3]
		};
	}
	Matrix4x4 operator-(const Matrix4x4& other) const {
		return Matrix4x4{
			m[0][0] - other.m[0][0],
			m[0][1] - other.m[0][1],
			m[0][2] - other.m[0][2],
			m[0][3] - other.m[0][3],
			m[1][0] - other.m[1][0],
			m[1][1] - other.m[1][1],
			m[1][2] - other.m[1][2],
			m[1][3] - other.m[1][3],
			m[2][0] - other.m[2][0],
			m[2][1] - other.m[2][1],
			m[2][2] - other.m[2][2],
			m[2][3] - other.m[2][3],
			m[3][0] - other.m[3][0],
			m[3][1] - other.m[3][1],
			m[3][2] - other.m[3][2],
			m[3][3] - other.m[3][3]
		};
	}
	Matrix4x4 operator*(const float& other) const {
		return Matrix4x4{
			m[0][0] * other,
			m[0][1] * other,
			m[0][2] * other,
			m[0][3] * other,
			m[1][0] * other,
			m[1][1] * other,
			m[1][2] * other,
			m[1][3] * other,
			m[2][0] * other,
			m[2][1] * other,
			m[2][2] * other,
			m[2][3] * other,
			m[3][0] * other,
			m[3][1] * other,
			m[3][2] * other,
			m[3][3] * other
		};
	}
	Matrix4x4 operator*(const Matrix4x4& other) const {
		Matrix4x4 matrix{};
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				for (int k = 0; k < 4; ++k) {
					matrix.m[i][j] += m[i][k] * other.m[k][j];
				}
			}
		}
		return matrix;
	}
	Matrix4x4 operator/(const float& other) const {
		return Matrix4x4{
			m[0][0] / other,
			m[0][1] / other,
			m[0][2] / other,
			m[0][3] / other,
			m[1][0] / other,
			m[1][1] / other,
			m[1][2] / other,
			m[1][3] / other,
			m[2][0] / other,
			m[2][1] / other,
			m[2][2] / other,
			m[2][3] / other,
			m[3][0] / other,
			m[3][1] / other,
			m[3][2] / other,
			m[3][3] / other
		};
	}

	Matrix4x4 Inverse() const {

		float determinant =
			+m[0][0] * m[1][1] * m[2][2] * m[3][3]
			+ m[0][0] * m[1][2] * m[2][3] * m[3][1]
			+ m[0][0] * m[1][3] * m[2][1] * m[3][2]

			- m[0][0] * m[1][3] * m[2][2] * m[3][1]
			- m[0][0] * m[1][2] * m[2][1] * m[3][3]
			- m[0][0] * m[1][1] * m[2][3] * m[3][2]

			- m[0][1] * m[1][0] * m[2][2] * m[3][3]
			- m[0][2] * m[1][0] * m[2][3] * m[3][1]
			- m[0][3] * m[1][0] * m[2][1] * m[3][2]

			+ m[0][3] * m[1][0] * m[2][2] * m[3][1]
			+ m[0][2] * m[1][0] * m[2][1] * m[3][3]
			+ m[0][1] * m[1][0] * m[2][3] * m[3][2]

			+ m[0][1] * m[1][2] * m[2][0] * m[3][3]
			+ m[0][2] * m[1][3] * m[2][0] * m[3][1]
			+ m[0][3] * m[1][1] * m[2][0] * m[3][2]

			- m[0][3] * m[1][2] * m[2][0] * m[3][1]
			- m[0][2] * m[1][1] * m[2][0] * m[3][3]
			- m[0][1] * m[1][3] * m[2][0] * m[3][2]

			- m[0][1] * m[1][2] * m[2][3] * m[3][0]
			- m[0][2] * m[1][3] * m[2][1] * m[3][0]
			- m[0][3] * m[1][1] * m[2][2] * m[3][0]

			+ m[0][3] * m[1][2] * m[2][1] * m[3][0]
			+ m[0][2] * m[1][1] * m[2][3] * m[3][0]
			+ m[0][1] * m[1][3] * m[2][2] * m[3][0];

		Matrix4x4 result = {};
		float recpDeterminant = 1.0f / determinant;
		result.m[0][0] = (m[1][1] * m[2][2] * m[3][3] + m[1][2] * m[2][3] * m[3][1] +
			m[1][3] * m[2][1] * m[3][2] - m[1][3] * m[2][2] * m[3][1] -
			m[1][2] * m[2][1] * m[3][3] - m[1][1] * m[2][3] * m[3][2]) * recpDeterminant;
		result.m[0][1] = (-m[0][1] * m[2][2] * m[3][3] - m[0][2] * m[2][3] * m[3][1] -
			m[0][3] * m[2][1] * m[3][2] + m[0][3] * m[2][2] * m[3][1] +
			m[0][2] * m[2][1] * m[3][3] + m[0][1] * m[2][3] * m[3][2]) * recpDeterminant;
		result.m[0][2] = (m[0][1] * m[1][2] * m[3][3] + m[0][2] * m[1][3] * m[3][1] +
			m[0][3] * m[1][1] * m[3][2] - m[0][3] * m[1][2] * m[3][1] -
			m[0][2] * m[1][1] * m[3][3] - m[0][1] * m[1][3] * m[3][2]) * recpDeterminant;
		result.m[0][3] = (-m[0][1] * m[1][2] * m[2][3] - m[0][2] * m[1][3] * m[2][1] -
			m[0][3] * m[1][1] * m[2][2] + m[0][3] * m[1][2] * m[2][1] +
			m[0][2] * m[1][1] * m[2][3] + m[0][1] * m[1][3] * m[2][2]) * recpDeterminant;

		result.m[1][0] = (-m[1][0] * m[2][2] * m[3][3] - m[1][2] * m[2][3] * m[3][0] -
			m[1][3] * m[2][0] * m[3][2] + m[1][3] * m[2][2] * m[3][0] +
			m[1][2] * m[2][0] * m[3][3] + m[1][0] * m[2][3] * m[3][2]) * recpDeterminant;
		result.m[1][1] = (m[0][0] * m[2][2] * m[3][3] + m[0][2] * m[2][3] * m[3][0] +
			m[0][3] * m[2][0] * m[3][2] - m[0][3] * m[2][2] * m[3][0] -
			m[0][2] * m[2][0] * m[3][3] - m[0][0] * m[2][3] * m[3][2]) * recpDeterminant;
		result.m[1][2] = (-m[0][0] * m[1][2] * m[3][3] - m[0][2] * m[1][3] * m[3][0] -
			m[0][3] * m[1][0] * m[3][2] + m[0][3] * m[1][2] * m[3][0] +
			m[0][2] * m[1][0] * m[3][3] + m[0][0] * m[1][3] * m[3][2]) * recpDeterminant;
		result.m[1][3] = (m[0][0] * m[1][2] * m[2][3] + m[0][2] * m[1][3] * m[2][0] +
			m[0][3] * m[1][0] * m[2][2] - m[0][3] * m[1][2] * m[2][0] -
			m[0][2] * m[1][0] * m[2][3] - m[0][0] * m[1][3] * m[2][2]) * recpDeterminant;

		result.m[2][0] = (m[1][0] * m[2][1] * m[3][3] + m[1][1] * m[2][3] * m[3][0] +
			m[1][3] * m[2][0] * m[3][1] - m[1][3] * m[2][1] * m[3][0] -
			m[1][1] * m[2][0] * m[3][3] - m[1][0] * m[2][3] * m[3][1]) * recpDeterminant;
		result.m[2][1] = (-m[0][0] * m[2][1] * m[3][3] - m[0][1] * m[2][3] * m[3][0] -
			m[0][3] * m[2][0] * m[3][1] + m[0][3] * m[2][1] * m[3][0] +
			m[0][1] * m[2][0] * m[3][3] + m[0][0] * m[2][3] * m[3][1]) * recpDeterminant;
		result.m[2][2] = (m[0][0] * m[1][1] * m[3][3] + m[0][1] * m[1][3] * m[3][0] +
			m[0][3] * m[1][0] * m[3][1] - m[0][3] * m[1][1] * m[3][0] -
			m[0][1] * m[1][0] * m[3][3] - m[0][0] * m[1][3] * m[3][1]) * recpDeterminant;
		result.m[2][3] = (-m[0][0] * m[1][1] * m[2][3] - m[0][1] * m[1][3] * m[2][0] -
			m[0][3] * m[1][0] * m[2][1] + m[0][3] * m[1][1] * m[2][0] +
			m[0][1] * m[1][0] * m[2][3] + m[0][0] * m[1][3] * m[2][1]) * recpDeterminant;

		result.m[3][0] = (-m[1][0] * m[2][1] * m[3][2] - m[1][1] * m[2][2] * m[3][0] -
			m[1][2] * m[2][0] * m[3][1] + m[1][2] * m[2][1] * m[3][0] +
			m[1][1] * m[2][0] * m[3][2] + m[1][0] * m[2][2] * m[3][1]) * recpDeterminant;
		result.m[3][1] = (m[0][0] * m[2][1] * m[3][2] + m[0][1] * m[2][2] * m[3][0] +
			m[0][2] * m[2][0] * m[3][1] - m[0][2] * m[2][1] * m[3][0] -
			m[0][1] * m[2][0] * m[3][2] - m[0][0] * m[2][2] * m[3][1]) * recpDeterminant;
		result.m[3][2] = (-m[0][0] * m[1][1] * m[3][2] - m[0][1] * m[1][2] * m[3][0] -
			m[0][2] * m[1][0] * m[3][1] + m[0][2] * m[1][1] * m[3][0] +
			m[0][1] * m[1][0] * m[3][2] + m[0][0] * m[1][2] * m[3][1]) * recpDeterminant;
		result.m[3][3] = (m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] +
			m[0][2] * m[1][0] * m[2][1] - m[0][2] * m[1][1] * m[2][0] -
			m[0][1] * m[1][0] * m[2][2] - m[0][0] * m[1][2] * m[2][1]) * recpDeterminant;

		return result;

	}

	Matrix4x4 Transpose()
	{

		Matrix4x4 ans;


		ans.m[0][0] = m[0][0];
		ans.m[0][1] = m[1][0];
		ans.m[0][2] = m[2][0];
		ans.m[0][3] = m[3][0];

		ans.m[1][0] = m[0][1];
		ans.m[1][1] = m[1][1];
		ans.m[1][2] = m[2][1];
		ans.m[1][3] = m[3][1];

		ans.m[2][0] = m[0][2];
		ans.m[2][1] = m[1][2];
		ans.m[2][2] = m[2][2];
		ans.m[2][3] = m[3][2];

		ans.m[3][0] = m[0][3];
		ans.m[3][1] = m[1][3];
		ans.m[3][2] = m[2][3];
		ans.m[3][3] = m[3][3];


		return ans;

	}

	//0行列
	Matrix4x4 MakeIdentity4x4()
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

		ans.m[3][0] = 0;
		ans.m[3][1] = 0;
		ans.m[3][2] = 0;
		ans.m[3][3] = 1;

		return ans;

	}


};




