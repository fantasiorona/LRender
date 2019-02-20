#pragma once

#include "math/matrix.h"
#include "math/vector.h"

#include <math.h>

namespace LRender {
	class Orbit final {
	public:
		Orbit(
			const float angle = atan(1),
			const float pitch = atan(1),
			const float zoom = 2.5f);
		void mouseGrab();
		void mouseMove(const int x, const int y);
		void mouseRelease();
		void zoomIn();
		void zoomOut();
		const Matrix &getMatrix() const;

	private:
		static const Vector UP;
		static const float RADIANS_PER_PIXEL;
		static const float PITCH_MIN;
		static const float PITCH_MAX;
		static const float ZOOM_MIN;
		static const float ZOOM_MAX;
		static const float ZOOM_FACTOR;

		Matrix view;
		int x, y;
		bool dragging;
		float angle;
		float pitch;
		float zoom;

		void updateMatrix();
	};
};