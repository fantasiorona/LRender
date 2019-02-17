#include "matrix.h"

#include <algorithm>
#include <iterator>
#include <math.h>

using namespace LRender;

Matrix::Matrix() {
	identity();
}

Matrix::Matrix(const Matrix &other) {
	*this = other;
}

Matrix::Matrix(const float (&values)[4][4]) {
	std::copy(&values[0][0], &values[0][0] + 16, &m[0][0]);
}

Matrix Matrix::copy() const {
	return Matrix(*this);
}

Matrix &Matrix::identity() {
	for(size_t i = 0; i < 4; ++i)
		m[i][i] = 1;

	return *this;
}

Matrix &Matrix::translate(const Vector &translation) {
	return *this *= Matrix::makeTranslate(translation);
}

Matrix &Matrix::scale(const Vector &scale) {
	return *this *= Matrix::makeScale(scale);
}

Matrix &Matrix::rotateX(const float angle) {
	return *this *= Matrix::makeRotateX(angle);
}

Matrix &Matrix::rotateY(const float angle) {
	return *this *= Matrix::makeRotateY(angle);
}

Matrix &Matrix::rotateZ(const float angle) {
	return *this *= Matrix::makeRotateZ(angle);
}

Vector Matrix::operator*(const Vector &other) const {
	return Vector(
		m[0][0] * other.x + m[1][0] * other.y + m[2][0] * other.z + m[3][0],
		m[0][1] * other.x + m[1][1] * other.y + m[2][1] * other.z + m[3][1],
		m[0][2] * other.x + m[1][2] * other.y + m[2][2] * other.z + m[3][2]
	);
}

Matrix &Matrix::operator*=(const Matrix &other) {
	Matrix matrix;

	for(size_t j = 0; j < 4; ++j) for(size_t i = 0; i < 4; ++i) {
		matrix.m[i][j] = m[i][0] * other.m[0][j];

		for(size_t k = 1; k < 4; ++k)
			matrix.m[i][j] += m[i][k] * other.m[k][j];
	}

	return *this = matrix;
}

Matrix &Matrix::operator=(const Matrix &other) {
	std::copy(&other.m[0][0], &other.m[0][0] + 16, &m[0][0]);

	return *this;
}

float Matrix::get(const size_t x, const size_t y) const {
	return m[x][y];
}

Matrix Matrix::makeTranslate(const Vector &translation) {
	return Matrix({
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ translation.x, translation.y, translation.z, 1 }
	});
}

Matrix Matrix::makeScale(const Vector &scale) {
	return Matrix({
		{ scale.x, 0, 0, 0 },
		{ 0, scale.y, 0, 0 },
		{ 0, 0, scale.z, 0 },
		{ 0, 0, 0, 1 }
	});
}

Matrix Matrix::makeRotateX(const float angle) {
	return Matrix({
		{ 1, 0, 0, 0 },
		{ 0, cos(angle), -sin(angle), 0 },
		{ 0, sin(angle), cos(angle), 0 },
		{ 0, 0, 0, 1 }
	});
}

Matrix Matrix::makeRotateY(const float angle) {
	return Matrix({
		{ cos(angle), 0, sin(angle), 0 },
		{ 0, 1, 0, 0 },
		{ -sin(angle), 0, cos(angle), 0 },
		{ 0, 0, 0, 1 }
	});
}

Matrix Matrix::makeRotateZ(const float angle) {
	return Matrix({
		{ cos(angle), -sin(angle), 0, 0 },
		{ sin(angle), cos(angle), 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	});
}

Matrix Matrix::makeLookAt(const Vector &from, const Vector &to, const Vector &up) {
	auto f = (to - from).normalize();
	auto s = f.cross(up).normalize();
	auto t = s.cross(f);

	return Matrix({
		{ s.x, t.x, -f.x, 0 },
		{ s.y, t.y, -f.y, 0 },
		{ s.z, t.z, -f.z, 0 },
		{
			-Vector(s.x, t.x, -f.x).dot(from),
			-Vector(s.y, t.y, -f.y).dot(from),
			-Vector(s.z, t.z, -f.z).dot(from),
			1
		}
	});
}

Matrix Matrix::makePerspective(
	const float angle,
	const float aspect,
	const float znear,
	const float zfar) {
	const float a = 1.0 / tan(angle * 0.5);

	return Matrix({
		{ a / aspect, 0, 0, 0 },
		{ 0, a, 0, 0 },
		{ 0, 0, -((zfar + znear) / (zfar - znear)), -1 },
		{ 0, 0, -((2 * zfar * znear) / (zfar - znear)), 0 }
	});
}

Matrix LRender::operator*(Matrix lhs, const Matrix &rhs) {
	return lhs *= rhs;
}

std::ostream &operator<<(std::ostream &stream, const Matrix &matrix) {
	for(size_t row = 0; row < 4; ++row) {
		stream << '(';

		for(size_t column = 0; column < 4; ++column) {
			stream << matrix.get(column, row);

			if(column < 3)
				stream << ", ";
		}

		stream << ')';

		if(row < 3)
			stream << std::endl;
	}

	return stream;
}