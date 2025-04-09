/*
 * ClosestFit.cpp
 *
 *  Created on: Apr 8, 2025
 *      Author: tianhao
 */

#include "ClosestFit_Scheduler.h"

namespace MobiEdgeSim {

static double calculateDistance(double lat1, double lon1, double lat2, double lon2)
{
    return std::sqrt((lat1 - lat2) * (lat1 - lat2) + (lon1 - lon2) * (lon1 - lon2));
}

std::string ClosestFit_Scheduler::findBestHost(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts)
{

    std::cout << "[ClosestFit_Scheduler] Handling request:" << "\n    AppName = " << appInfo.name << "\n    CPU     = " << appInfo.cpu
            << "\n    RAM     = " << appInfo.ram << "\n    Disk    = " << appInfo.disk << "\n    Lat/Lon = (" << appInfo.latitude << ", "
            << appInfo.longitude << ")\n" << std::endl;

    std::cout << "[ClosestFit_Scheduler] Available hosts:" << std::endl;
    for (const auto &host : hosts) {
        std::cout << "  Host " << host.name << ": CPU=" << host.availableCpu << ", RAM=" << host.availableRam << ", Disk=" << host.availableDisk
                << ", Lat/Lon=(" << host.latitude << ", " << host.longitude << ")" << std::endl;
    }
    std::cout << std::endl;

    double minDistance = std::numeric_limits<double>::max();
    std::string bestHost = "mecHost_null";

    for (const auto &host : hosts) {
        double distance = calculateDistance(host.latitude, host.longitude, appInfo.latitude, appInfo.longitude);

        bool canAllocate = host.availableCpu >= appInfo.cpu && host.availableRam >= appInfo.ram && host.availableDisk >= appInfo.disk;

        if (canAllocate && distance < minDistance) {
            minDistance = distance;
            bestHost = host.name;
        }
    }

    return bestHost;
}

} /* namespace MobiEdgeSim */
