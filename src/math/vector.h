#pragma once

#include <ostream>

namespace LRender {
	class Vector final {
	public:
		Vector();
		Vector(const float n);
		Vector(const float x, const float y, const float z);
		Vector(const Vector &other);
		float length() const;
		float dot(const Vector &other) const;
		Vector copy() const;
		Vector cross(const Vector &other) const;
		Vector &normalize();
		Vector operator-() const;
		Vector &operator*=(const float scalar);
		Vector &operator*=(const Vector &other);
		Vector &operator+=(const Vector &other);
		Vector &operator-=(const Vector &other);
		Vector &operator=(const Vector &other);
		bool operator==(const Vector &other) const;
		bool operator!=(const Vector &other) const;

		float x, y, z;
	};

	Vector operator+(Vector lhs, const Vector &rhs);
	Vector operator-(Vector lhs, const Vector &rhs);
	Vector operator*(Vector lhs, const Vector &rhs);
};

std::ostream &operator<<(std::ostream &stream, const LRender::Vector &vector);