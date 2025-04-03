#include "Matrix4x4.h"

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& matrix) {

	Matrix4x4 result;

	result.m[0][0] = m[0][0] + matrix.m[0][0];
	result.m[0][1] = m[0][1] + matrix.m[0][1];
	result.m[0][2] = m[0][2] + matrix.m[0][2];
	result.m[0][3] = m[0][3] + matrix.m[0][3];
	result.m[1][0] = m[1][0] + matrix.m[1][0];
	result.m[1][1] = m[1][1] + matrix.m[1][1];
	result.m[1][2] = m[1][2] + matrix.m[1][2];
	result.m[1][3] = m[1][3] + matrix.m[1][3];
	result.m[2][0] = m[2][0] + matrix.m[2][0];
	result.m[2][1] = m[2][1] + matrix.m[2][1];
	result.m[2][2] = m[2][2] + matrix.m[2][2];
	result.m[2][3] = m[2][3] + matrix.m[2][3];
	result.m[3][0] = m[3][0] + matrix.m[3][0];
	result.m[3][1] = m[3][1] + matrix.m[3][1];
	result.m[3][2] = m[3][2] + matrix.m[3][2];
	result.m[3][3] = m[3][3] + matrix.m[3][3];

	return result;
}

Matrix4x4 Matrix4x4::operator+=(const Matrix4x4& matrix) {

	m[0][0] += matrix.m[0][0];
	m[0][1] += matrix.m[0][1];
	m[0][2] += matrix.m[0][2];
	m[0][3] += matrix.m[0][3];
	m[1][0] += matrix.m[1][0];
	m[1][1] += matrix.m[1][1];
	m[1][2] += matrix.m[1][2];
	m[1][3] += matrix.m[1][3];
	m[2][0] += matrix.m[2][0];
	m[2][1] += matrix.m[2][1];
	m[2][2] += matrix.m[2][2];
	m[2][3] += matrix.m[2][3];
	m[3][0] += matrix.m[3][0];
	m[3][1] += matrix.m[3][1];
	m[3][2] += matrix.m[3][2];
	m[3][3] += matrix.m[3][3];

	return *this;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& matrix) {

	Matrix4x4 result;

	result.m[0][0] = m[0][0] - matrix.m[0][0];
	result.m[0][1] = m[0][1] - matrix.m[0][1];
	result.m[0][2] = m[0][2] - matrix.m[0][2];
	result.m[0][3] = m[0][3] - matrix.m[0][3];
	result.m[1][0] = m[1][0] - matrix.m[1][0];
	result.m[1][1] = m[1][1] - matrix.m[1][1];
	result.m[1][2] = m[1][2] - matrix.m[1][2];
	result.m[1][3] = m[1][3] - matrix.m[1][3];
	result.m[2][0] = m[2][0] - matrix.m[2][0];
	result.m[2][1] = m[2][1] - matrix.m[2][1];
	result.m[2][2] = m[2][2] - matrix.m[2][2];
	result.m[2][3] = m[2][3] - matrix.m[2][3];
	result.m[3][0] = m[3][0] - matrix.m[3][0];
	result.m[3][1] = m[3][1] - matrix.m[3][1];
	result.m[3][2] = m[3][2] - matrix.m[3][2];
	result.m[3][3] = m[3][3] - matrix.m[3][3];

	return result;
}

Matrix4x4 Matrix4x4::operator-=(const Matrix4x4& matrix) {

	m[0][0] -= matrix.m[0][0];
	m[0][1] -= matrix.m[0][1];
	m[0][2] -= matrix.m[0][2];
	m[0][3] -= matrix.m[0][3];
	m[1][0] -= matrix.m[1][0];
	m[1][1] -= matrix.m[1][1];
	m[1][2] -= matrix.m[1][2];
	m[1][3] -= matrix.m[1][3];
	m[2][0] -= matrix.m[2][0];
	m[2][1] -= matrix.m[2][1];
	m[2][2] -= matrix.m[2][2];
	m[2][3] -= matrix.m[2][3];
	m[3][0] -= matrix.m[3][0];
	m[3][1] -= matrix.m[3][1];
	m[3][2] -= matrix.m[3][2];
	m[3][3] -= matrix.m[3][3];

	return *this;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& matrix) {

	Matrix4x4 result;

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {

			result.m[x][y] = 0;

			for (int z = 0; z < 4; z++) {

				result.m[x][y] += m[x][z] * matrix.m[z][y];
			}
		}
	}

	return result;
}

Matrix4x4 Matrix4x4::operator*=(const Matrix4x4& matrix) {

	Matrix4x4 result;

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {

			result.m[x][y] = 0;

			for (int z = 0; z < 4; z++) {

				result.m[x][y] += m[x][z] * matrix.m[z][y];
			}
		}
	}

	*this = result;

	return *this;
}
