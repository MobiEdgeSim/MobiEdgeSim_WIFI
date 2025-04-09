/*
 * ClosestFit.cpp
 *
 *  Created on: Apr 8, 2025
 *      Author: tianhao
 */

#include "ClosestFit_Scheduler.h"

namespace MobiEdgeSim {

static double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    return std::sqrt((lat1 - lat2) * (lat1 - lat2) + (lon1 - lon2) * (lon1 - lon2));
}

std::string ClosestFit_Scheduler::findBestHost(
    const Orchestrator::AppDescriptorInfo& appInfo,
    const std::vector<MecHostInfo>& hosts
) {
    double minDistance = std::numeric_limits<double>::max();
    std::string bestHost = "mecHost_null";

    for (const auto& host : hosts) {
        double distance = calculateDistance(host.latitude, host.longitude, appInfo.latitude, appInfo.longitude);

        bool canAllocate = host.availableCpu >= appInfo.cpu &&
                           host.availableRam >= appInfo.ram &&
                           host.availableDisk >= appInfo.disk;

        if (canAllocate && distance < minDistance) {
            minDistance = distance;
            bestHost = host.name;
        }
    }

    return bestHost;
}

} /* namespace MobiEdgeSim */
