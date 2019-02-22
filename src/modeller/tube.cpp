#include "modeller.h"
#include "../math/constants.h"
#include "../math/matrix.h"

#include <math.h>

using namespace LRender;

float getRadius(size_t dist) {
	return (1.0f - (1.0f / (float(dist) / 150.0f + 1.0f))) * 0.1f;
}

void Modeller::makeTube(
	std::vector<Vertex> &vertices,
	std::vector<uint32_t> &indices,
	const size_t precision,
	const Path &path) {
	std::vector<Vector> ring;
	Vector color(0.5, 0.7, 0.2);

	for(size_t i = 0; i < precision; ++i) {
		const float radians = Constants::PI * 2 * float(i) / precision;

		ring.push_back(Vector(cos(radians), 0, sin(radians)));
	}

	for(auto node = path.getNodes().begin(); node < path.getNodes().end(); ++node) {
		std::vector<Vector> transformedRing = ring;

		path.transform(transformedRing, node - path.getNodes().begin());

		for(auto ringPoint : transformedRing)
			vertices.push_back(Vertex(
				node->position + ringPoint * getRadius(node->topDist),
				ringPoint,
				color));

		const size_t size = vertices.size();

		if(node > path.getNodes().begin()) for(size_t i = 0; i < precision; ++i) {
			indices.push_back(size - 1 - i);
			indices.push_back(size - 1 - precision - i);
			indices.push_back(size - 1 - precision - ((i + 1) % precision));
			indices.push_back(size - 1 - precision - ((i + 1) % precision));
			indices.push_back(size - 1 - ((i + 1) % precision));
			indices.push_back(size - 1 - i);
		}
	}
}