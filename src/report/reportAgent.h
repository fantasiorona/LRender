#pragma once

#include "reportLimits.h"
#include "reportExposure.h"
#include "reportSeed.h"
#include "reportSize.h"
#include "reportLeaf.h"
#include "math/vector.h"

#include <vector>

namespace LRender {
	class ReportAgent final {
	public:
		ReportAgent(
			const Vector &position,
			std::vector<ReportSeed> seeds,
			std::vector<ReportLeaf> leaves,
			ReportLimits limits,
			ReportSize size);
		void setExposure(ReportExposure &exposure);
		const Vector &getPosition() const;
		const std::vector<ReportSeed> &getSeeds() const;
		const std::vector<ReportLeaf> &getLeaves() const;
		const ReportLimits &getLimits() const;
		const ReportSize &getSize() const;
		const ReportExposure &getExposure() const;

	private:
		const Vector position;
		const std::vector<ReportSeed> seeds;
		const std::vector<ReportLeaf> leaves;
		const ReportLimits limits;
		const ReportSize size;
		ReportExposure exposure;
	};
}