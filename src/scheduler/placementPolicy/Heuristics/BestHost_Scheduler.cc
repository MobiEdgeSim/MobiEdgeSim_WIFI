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

#include "BestHost_Scheduler.h"

namespace MobiEdgeSim {

std::string BestHost_Scheduler::findBestHost(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts)
{
    if (hosts.empty()) {
        return "mecHost_null";
    }

    double bestFitness = -1e9;
    std::string bestHostName = "mecHost_null";

    for (int i = 0; i < (int) hosts.size(); i++) {
        double f = computeFitness(hosts[i], appInfo);
        if (f > bestFitness) {
            bestFitness = f;
            bestHostName = hosts[i].name;
        }
    }

    if (bestFitness < -1e8) {
        return "mecHost_null";
    }
    return bestHostName;

}

double BestHost_Scheduler::computeFitness(const MecHostInfo &host, const Orchestrator::AppDescriptorInfo &appInfo) const
{
    if (!canAllocate(host, appInfo)) {
        return -1e9;
    }

    double latency = host.latency;
    double distance = calcDistance(host.latitude, host.longitude, appInfo.latitude, appInfo.longitude);

    double cpuRate = (host.cpu > 0) ? (host.availableCpu / host.cpu) : 0.0;
    double ramRate = (host.ram > 0) ? (host.availableRam / host.ram) : 0.0;
    double diskRate = (host.disk > 0) ? (host.availableDisk / host.disk) : 0.0;
    double resourceRate = (cpuRate + ramRate + diskRate) / 3.0;

    double invLatency = 1.0 / (1.0 + latency);
    double invDistance = 1.0 / (1.0 + distance);

    double wLatency = 0.6;
    double wResource = 0.2;
    double wLocation = 0.2;

    double fitnessVal = wLatency * invLatency + wResource * resourceRate + wLocation * invDistance;
    return fitnessVal;
}

bool BestHost_Scheduler::canAllocate(const MecHostInfo &host, const Orchestrator::AppDescriptorInfo &appInfo) const
{
    if (host.availableCpu < appInfo.cpu)
        return false;
    if (host.availableRam < appInfo.ram)
        return false;
    if (host.availableDisk < appInfo.disk)
        return false;
    return true;
}

double BestHost_Scheduler::calcDistance(double lat1, double lon1, double lat2, double lon2) const
{
    double dlat = lat1 - lat2;
    double dlon = lon1 - lon2;
    return std::sqrt(dlat * dlat + dlon * dlon);
}

}
