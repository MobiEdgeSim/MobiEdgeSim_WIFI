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
#include <limits>


namespace MobiEdgeSim {


std::string BestHost_Scheduler::findBestHost(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts)
{
    if (hosts.empty()) {
        return "mecHost_null";
    }

    double bestFitness = -std::numeric_limits<double>::infinity();
    std::string bestHostName = "mecHost_null";

    double maxLatency = -std::numeric_limits<double>::infinity(), minLatency = std::numeric_limits<double>::infinity();
    double maxDistance = -std::numeric_limits<double>::infinity(), minDistance = std::numeric_limits<double>::infinity();
    double maxCpu = -std::numeric_limits<double>::infinity(), minCpu = std::numeric_limits<double>::infinity();
    double maxRam = -std::numeric_limits<double>::infinity(), minRam = std::numeric_limits<double>::infinity();
    double maxDisk = -std::numeric_limits<double>::infinity(), minDisk = std::numeric_limits<double>::infinity();

    std::vector<double> distances(hosts.size());

    for (size_t i = 0; i < hosts.size(); i++) {
        distances[i] = calcDistance(hosts[i].latitude, hosts[i].longitude, appInfo.latitude, appInfo.longitude);

        maxLatency = std::max(maxLatency, hosts[i].latency);
        minLatency = std::min(minLatency, hosts[i].latency);

        maxDistance = std::max(maxDistance, distances[i]);
        minDistance = std::min(minDistance, distances[i]);

        double cpuRate = hosts[i].cpu > 0 ? hosts[i].availableCpu / hosts[i].cpu : 0.0;
        double ramRate = hosts[i].ram > 0 ? hosts[i].availableRam / hosts[i].ram : 0.0;
        double diskRate = hosts[i].disk > 0 ? hosts[i].availableDisk / hosts[i].disk : 0.0;

        maxCpu = std::max(maxCpu, cpuRate);
        minCpu = std::min(minCpu, cpuRate);
        maxRam = std::max(maxRam, ramRate);
        minRam = std::min(minRam, ramRate);
        maxDisk = std::max(maxDisk, diskRate);
        minDisk = std::min(minDisk, diskRate);
    }

    for (size_t i = 0; i < hosts.size(); i++) {
        double f = computeFitness(hosts[i], appInfo, maxLatency, minLatency, maxDistance, minDistance, maxCpu, minCpu, maxRam, minRam, maxDisk,
                minDisk);
        if (f > bestFitness) {
            bestFitness = f;
            bestHostName = hosts[i].name;
        }
    }

    if (bestFitness <= 0) {
        return "mecHost_null";
    }
    return bestHostName;
}

double BestHost_Scheduler::computeFitness(const MecHostInfo &host, const Orchestrator::AppDescriptorInfo &appInfo, double maxLatency,
        double minLatency, double maxDistance, double minDistance, double maxCpu, double minCpu, double maxRam, double minRam, double maxDisk,
        double minDisk) const
{
    if (!canAllocate(host, appInfo)) {
        return -std::numeric_limits<double>::infinity();
    }

    double cpuRate = host.cpu > 0 ? host.availableCpu / host.cpu : 0.0;
    double ramRate = host.ram > 0 ? host.availableRam / host.ram : 0.0;
    double diskRate = host.disk > 0 ? host.availableDisk / host.disk : 0.0;
    double resourceRate = ((cpuRate - minCpu) / (maxCpu - minCpu + 1e-6) + (ramRate - minRam) / (maxRam - minRam + 1e-6)
            + (diskRate - minDisk) / (maxDisk - minDisk + 1e-6)) / 3.0;

    double latencyNorm = (maxLatency - host.latency) / (maxLatency - minLatency + 1e-6);
    double distance = calcDistance(host.latitude, host.longitude, appInfo.latitude, appInfo.longitude);
    double distanceNorm = (maxDistance - distance) / (maxDistance - minDistance + 1e-6);

    double wLatency = 0.8;
    double wResource = 0.1;
    double wDistance = 0.1;

    return wLatency * latencyNorm + wResource * resourceRate + wDistance * distanceNorm;
}

bool BestHost_Scheduler::canAllocate(const MecHostInfo &host, const Orchestrator::AppDescriptorInfo &appInfo) const
{
    return host.availableCpu >= appInfo.cpu && host.availableRam >= appInfo.ram && host.availableDisk >= appInfo.disk;
}

double BestHost_Scheduler::calcDistance(double lat1, double lon1, double lat2, double lon2) const
{
    double dlat = lat1 - lat2;
    double dlon = lon1 - lon2;
    return std::sqrt(dlat * dlat + dlon * dlon);
}



}
