//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef SCHEDULER_PLACEMENTPOLICY_HEURISTICS_BESTHOST_SCHEDULER_H_
#define SCHEDULER_PLACEMENTPOLICY_HEURISTICS_BESTHOST_SCHEDULER_H_
#include "scheduler/SchedulerInterface.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>


namespace MobiEdgeSim {

class BestHost_Scheduler : public SchedulerInterface
{
public:
    std::string findBestHost(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts) override;

protected:
    double computeFitness(const MecHostInfo &host, const Orchestrator::AppDescriptorInfo &appInfo,
                          double maxLatency, double minLatency,
                          double maxDistance, double minDistance,
                          double maxCpu, double minCpu,
                          double maxRam, double minRam,
                          double maxDisk, double minDisk) const;
    bool canAllocate(const MecHostInfo &host, const Orchestrator::AppDescriptorInfo &appInfo) const;
    double calcDistance(double lat1, double lon1, double lat2, double lon2) const;
};


} /* namespace MobiEdgeSim */
#endif
