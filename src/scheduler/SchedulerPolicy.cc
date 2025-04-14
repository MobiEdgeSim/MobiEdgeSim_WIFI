/*
 * SchedulerPolicy.cpp
 *
 *  Created on: Apr 8, 2025
 *      Author: tianhao
 */

#include "SchedulerPolicy.h"

#include "placementPolicy/Heuristics/BestHost_Scheduler.h"
#include "scheduler/placementPolicy/ABC/ABC_Scheduler.h"
#include "placementPolicy/Heuristics/ClosestFit_Scheduler.h"
#include "scheduler/placementPolicy/Heuristics/ClosestFit_Scheduler.h"

namespace MobiEdgeSim {

std::unique_ptr<SchedulerInterface> SchedulerPolicy::createScheduler(const std::string& algorithmName) {
    //std::cout<< "[SchedulerPolicy] createScheduler called at simTime = " << simTime() << std::endl;

    if (algorithmName == "ABC") {
        return std::make_unique<ABC_Scheduler>();
    }
    // else if (algorithmName == "HHO") {
    //     return std::make_unique<HHO_Scheduler>();
    // }
     else if (algorithmName == "BestHost") {
         return std::make_unique<BestHost_Scheduler>();
     }
    else if (algorithmName == "ClosestFit") {
        return std::make_unique<ClosestFit_Scheduler>();
    }
    else {
        throw std::invalid_argument("Unknown scheduler algorithm: " + algorithmName);
    }
}

} /* namespace MobiEdgeSim */
