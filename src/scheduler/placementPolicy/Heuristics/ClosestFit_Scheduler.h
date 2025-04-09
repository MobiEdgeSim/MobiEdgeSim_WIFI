/*
 * ClosestFit.h
 *
 *  Created on: Apr 8, 2025
 *      Author: tianhao
 */

#ifndef SCHEDULER_PLACEMENTPOLICY_HEURISTICS_CLOSESTFIT_SCHEDULER_H_
#define SCHEDULER_PLACEMENTPOLICY_HEURISTICS_CLOSESTFIT_SCHEDULER_H_

#include "scheduler/SchedulerInterface.h"
#include <limits>
#include <cmath>
namespace MobiEdgeSim {

class ClosestFit_Scheduler : public SchedulerInterface
{
public:
    std::string findBestHost(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts) override;
};

} /* namespace MobiEdgeSim */

#endif /* SCHEDULER_PLACEMENTPOLICY_HEURISTICS_CLOSESTFIT_SCHEDULER_H_ */
