/*
 * SchedulerPolicy.h
 *
 *  Created on: Apr 8, 2025
 *      Author: tianhao
 */

#ifndef SCHEDULER_SCHEDULERPOLICY_H_
#define SCHEDULER_SCHEDULERPOLICY_H_

#include "SchedulerInterface.h"
#include <memory>


namespace MobiEdgeSim {

class SchedulerPolicy
{

public:
    static std::unique_ptr<SchedulerInterface> createScheduler(const std::string& algorithmName);

};

} /* namespace MobiEdgeSim */

#endif /* SCHEDULER_SCHEDULERPOLICY_H_ */
