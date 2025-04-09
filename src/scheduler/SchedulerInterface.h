#ifndef SCHEDULER_INTERFACE_H
#define SCHEDULER_INTERFACE_H

#include "nodes/orchestrator/orchestrator.h"
#include "nodes/mecHost/mecHost.h"
#include <string>
#include <vector>

namespace MobiEdgeSim {

class SchedulerInterface {
public:
    virtual ~SchedulerInterface() {}

    virtual std::string findBestHost(const Orchestrator::AppDescriptorInfo& appInfo, const std::vector<MecHostInfo>& hosts) = 0;
};

} // namespace MobiEdgeSim

#endif
