#pragma once

#include "modeller/node.h"
#include "modeller/branch.h"
#include "modeller/leaf.h"
#include "modeller/seed.h"
#include "scene/agent.h"
#include "model/model.h"
#include "lparse.h"

#include <vector>
#include <memory>
#include <list>

namespace LRender {
	class AgentModel final {
	public:
		AgentModel(
			const Agent &agent,
			LParse::Randomizer &randomizer);
		const Model &getBranches() const;
		const Model &getLeaves() const;
		const std::vector<Vector> &getSeedPositions() const;
		const Vector &getMinimum() const;
		const Vector &getMaximum() const;
		float getArea() const;

	private:
		static const float TURTLE_STEP;
		static const float TURTLE_ANGLE;
		static const size_t TUBE_PRECISION;

		std::unique_ptr<Model> modelBranches;
		std::unique_ptr<Model> modelLeaves;
		std::vector<Vector> seedPositions;
		Vector minimum;
		Vector maximum;
		float area;

		void build(
			const Agent &agent, LParse::Randomizer &randomizer);
		Branch *traceBranch(
			Branch *const parent,
			bool leaf,
			std::list<Branch> &branches,
			std::list<Leaf> &leaves,
			std::list<Seed> &seeds,
			Node node,
			std::string::const_iterator &at,
			const std::string::const_iterator &last);
	};
}