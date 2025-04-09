/*
 * ABC_Scheduler.h
 *
 *  Created on: Apr 8, 2025
 *      Author: tianhao
 */

#ifndef SCHEDULER_PLACEMENTPOLICY_ABC_ABC_SCHEDULER_H_
#define SCHEDULER_PLACEMENTPOLICY_ABC_ABC_SCHEDULER_H_

#include "scheduler/SchedulerInterface.h"

namespace MobiEdgeSim {

class ABC_Scheduler : public SchedulerInterface
{
public:
    std::string findBestHost(const Orchestrator::AppDescriptorInfo& appInfo, const std::vector<MecHostInfo>& hosts) override;

};

} /* namespace MobiEdgeSim */

#endif /* SCHEDULER_PLACEMENTPOLICY_ABC_ABC_SCHEDULER_H_ */
