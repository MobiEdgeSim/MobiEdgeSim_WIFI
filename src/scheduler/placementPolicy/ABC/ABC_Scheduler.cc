/*
 * ABC_Scheduler.cpp
 *
 *  Created on: Apr 8, 2025
 *      Author: tianhao
 */

#include "ABC_Scheduler.h"

namespace MobiEdgeSim {

std::string ABC_Scheduler::findBestHost(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts)
{
    if (hosts.empty())
        return "mecHost_null";


    return hosts[0].name;
}

} /* namespace MobiEdgeSim */
