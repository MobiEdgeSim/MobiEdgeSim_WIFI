/*
 * ABC_Scheduler.cpp
 *
 *  Created on: Apr 8, 2025
 *      Author: tianhao
 */

#include "ABC_Scheduler.h"
#include <iostream>
#include <cassert>

namespace MobiEdgeSim {

std::string ABC_Scheduler::findBestHost(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts)
{
    if (hosts.empty()) {
        return "mecHost_null";
    }

    bees.resize(populationSize);
    probs.resize(populationSize, 0.0);

    initBees(appInfo, hosts);
    calculateFitnessAll(appInfo, hosts);

    for (int iter = 0; iter < maxIterations; ++iter) {

        // 1) Employed
        sendEmployedBees(appInfo, hosts);

        // 2) calculate Onlooker probilities
        double maxFit = bees[0].fitness;
        for (int i = 1; i < populationSize; ++i) {
            if (bees[i].fitness > maxFit) {
                maxFit = bees[i].fitness;
            }
        }
        //
        for (int i = 0; i < populationSize; ++i) {
            probs[i] = 0.9 * (bees[i].fitness / maxFit) + 0.1;
        }

        // 3) Onlooker
        sendOnlookerBees(appInfo, hosts);

        // 4) re-calculate the fitness
        calculateFitnessAll(appInfo, hosts);

        // 5) keep best
        elitism();

        // 6) Scout
        sendScoutBees(appInfo, hosts);
    }
    //return best bee
    Bee &bestBee = bees[0];
    if (bestBee.hostIndex < 0 || bestBee.hostIndex >= (int) hosts.size()) {
        return "mecHost_null";
    }
    if (!canAllocate(hosts[bestBee.hostIndex], appInfo)) {
        return "mecHost_null";
    }
    return hosts[bestBee.hostIndex].name;
}

void ABC_Scheduler::initBees(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts)
{
    std::uniform_int_distribution<int> dist(0, (int) hosts.size() - 1);
    for (int i = 0; i < populationSize; ++i) {
        bees[i].hostIndex = dist(rng);
        bees[i].fitness = 0.0;
        bees[i].trial = 0;
    }
}

void ABC_Scheduler::calculateFitnessAll(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts)
{
    std::vector<HostMetrics> metricsVec(hosts.size());
    for (size_t hIdx = 0; hIdx < hosts.size(); ++hIdx) {
        HostMetrics hm;
        hm.feasible = canAllocate(hosts[hIdx], appInfo);
        if (!hm.feasible) {
            hm.latency = 1e9;
            hm.distance = 1e9;
            hm.resourceRate = 0.0;
        }
        else {
            double latency = hosts[hIdx].latency;
            double distance = calcDistance(hosts[hIdx].latitude, hosts[hIdx].longitude, appInfo.latitude, appInfo.longitude);

            double leftoverCpu = (hosts[hIdx].cpu > 0) ? (double) (hosts[hIdx].availableCpu - appInfo.cpu) / hosts[hIdx].cpu : 0.0;
            double leftoverRam = (hosts[hIdx].ram > 0) ? (double) (hosts[hIdx].availableRam - appInfo.ram) / hosts[hIdx].ram : 0.0;
            double leftoverDisk = (hosts[hIdx].disk > 0) ? (double) (hosts[hIdx].availableDisk - appInfo.disk) / hosts[hIdx].disk : 0.0;

            double resourceRate = (leftoverCpu + leftoverRam + leftoverDisk) / 3.0;

            hm.latency = latency;
            hm.distance = distance;
            hm.resourceRate = resourceRate;
        }
        metricsVec[hIdx] = hm;
    }

    normalizeAll(metricsVec);

    double wLatency = 0.6;
    double wDistance = 0.2;
    double wResource = 0.2;

    for (int i = 0; i < populationSize; ++i) {
        int hIdx = bees[i].hostIndex;
        if (hIdx < 0 || hIdx >= (int) hosts.size()) {
            bees[i].fitness = -1e9;
            continue;
        }
        const auto &hm = metricsVec[hIdx];
        if (!hm.feasible) {
            bees[i].fitness = -1e9;
        }
        else {
            double finalFitness = wLatency * hm.latency + wDistance * hm.distance + wResource * hm.resourceRate;
            bees[i].fitness = finalFitness;
        }
    }
}

double ABC_Scheduler::computeSingleBeeFitness(int hostIndex, const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts)
{
    if (hostIndex < 0 || hostIndex >= (int) hosts.size()) {
        return -1e9;
    }

    std::vector<HostMetrics> metricsVec(hosts.size());
    for (size_t i = 0; i < hosts.size(); ++i) {
        HostMetrics hm;
        hm.feasible = canAllocate(hosts[i], appInfo);
        if (!hm.feasible) {

            hm.latency = 1e9;
            hm.distance = 1e9;
            hm.resourceRate = 0.0;
        }
        else {
            double latency = hosts[i].latency;
            double distance = calcDistance(hosts[i].latitude, hosts[i].longitude, appInfo.latitude, appInfo.longitude);

            double leftoverCpu = (hosts[i].cpu > 0) ? (double) (hosts[i].availableCpu - appInfo.cpu) / hosts[i].cpu : 0.0;
            double leftoverRam = (hosts[i].ram > 0) ? (double) (hosts[i].availableRam - appInfo.ram) / hosts[i].ram : 0.0;
            double leftoverDisk = (hosts[i].disk > 0) ? (double) (hosts[i].availableDisk - appInfo.disk) / hosts[i].disk : 0.0;

            double resourceRate = (leftoverCpu + leftoverRam + leftoverDisk) / 3.0;

            hm.latency = latency;
            hm.distance = distance;
            hm.resourceRate = resourceRate;
        }
        metricsVec[i] = hm;
    }
    normalizeAll(metricsVec);

    const auto &hm = metricsVec[hostIndex];
    if (!hm.feasible) {
        return -1e9;
    }

    double wLatency = 0.6;
    double wDistance = 0.2;
    double wResource = 0.2;

    double finalFitness = wLatency * hm.latency + wDistance * hm.distance + wResource * hm.resourceRate;

    return finalFitness;
}

/**
 * Employed
 */
void ABC_Scheduler::sendEmployedBees(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts)
{
    for (int i = 0; i < populationSize; ++i) {
        employedMove(i, appInfo, hosts);
    }
}

/**
 * Onlooker
 */
void ABC_Scheduler::sendOnlookerBees(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts)
{
    std::uniform_real_distribution<double> distReal(0.0, 1.0);
    for (int i = 0; i < populationSize; ++i) {
        if (distReal(rng) < probs[i]) {
            employedMove(i, appInfo, hosts);
        }
    }
}

/**
 * Scout
 */
void ABC_Scheduler::sendScoutBees(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts)
{
    int maxTrialIndex = 0;
    int maxTrialVal = bees[0].trial;
    for (int i = 1; i < populationSize; ++i) {
        if (bees[i].trial > maxTrialVal) {
            maxTrialVal = bees[i].trial;
            maxTrialIndex = i;
        }
    }

    if (maxTrialVal >= limitTrial) {
        std::uniform_int_distribution<int> dist(0, (int) hosts.size() - 1);
        bees[maxTrialIndex].hostIndex = dist(rng);
        bees[maxTrialIndex].fitness = 0.0;
        bees[maxTrialIndex].trial = 0;
    }
}

/**
 * elitism：把最优 Bee 放到 bees[0]
 */
void ABC_Scheduler::elitism()
{
    Bee bestBee = bees[0];
    for (int i = 1; i < populationSize; ++i) {
        if (bees[i].fitness > bestBee.fitness) {
            bestBee = bees[i];
        }
    }
    bees[0] = bestBee;
}

/**
 * employedMove：对单个 Bee 做邻域搜索
 *   - 选一个 neighbor Bee，计算更新 hostIndex
 *   - 用 computeSingleBeeFitness(...) 与旧解比较
 *   - 如果更好就替换并退出；否则 trial++
 */
void ABC_Scheduler::employedMove(int beeIndex, const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts)
{
    Bee currentBee = bees[beeIndex];
    double oldFitness = currentBee.fitness;

    std::uniform_real_distribution<double> distReal(-1.0, 1.0);
    std::uniform_int_distribution<int> distBee(0, populationSize - 1);

    for (int t = 0; t < tryTime; ++t) {
        // 选一个不同的 Bee 作为邻居
        int neighborIdx = distBee(rng);
        while (neighborIdx == beeIndex) {
            neighborIdx = distBee(rng);
        }

        int neighborHostIndex = bees[neighborIdx].hostIndex;
        double phi = distReal(rng); // [-1,1]

        int oldVal = currentBee.hostIndex;
        int updated = (int) std::lround(oldVal + (oldVal - neighborHostIndex) * phi);

        // 边界修正
        if (updated < 0) {
            updated = 0;
        }
        else if (updated >= (int) hosts.size()) {
            updated = (int) hosts.size() - 1;
        }

        // 计算新解的局部 fitness
        double newFit = computeSingleBeeFitness(updated, appInfo, hosts);

        // 若更优就替换
        if (newFit > oldFitness) {
            bees[beeIndex].hostIndex = updated;
            bees[beeIndex].fitness = newFit;
            bees[beeIndex].trial = 0;
            return; // 本次搜索结束
        }
    }

    // 未找到更优 => trial++
    bees[beeIndex].trial++;
}

/**
 * 保留你的归一化逻辑
 */
void ABC_Scheduler::normalizeAll(std::vector<HostMetrics> &metricsVec)
{
    double minLat = 1e9, maxLat = 0.0;
    double minDist = 1e9, maxDist = 0.0;
    double minRes = 1e9, maxRes = -1e9;

    for (auto &hm : metricsVec) {
        if (!hm.feasible)
            continue;
        if (hm.latency < minLat)
            minLat = hm.latency;
        if (hm.latency > maxLat)
            maxLat = hm.latency;
        if (hm.distance < minDist)
            minDist = hm.distance;
        if (hm.distance > maxDist)
            maxDist = hm.distance;
        if (hm.resourceRate < minRes)
            minRes = hm.resourceRate;
        if (hm.resourceRate > maxRes)
            maxRes = hm.resourceRate;
    }

    double latRange = std::max(1e-9, maxLat - minLat);
    double distRange = std::max(1e-9, maxDist - minDist);
    double resRange = std::max(1e-9, maxRes - minRes);

    for (auto &hm : metricsVec) {
        if (!hm.feasible) {
            hm.latency = 0.0;
            hm.distance = 0.0;
            hm.resourceRate = 0.0;
            continue;
        }
        double latNorm = (hm.latency - minLat) / latRange;
        latNorm = 1.0 - latNorm;

        double distNorm = (hm.distance - minDist) / distRange;
        distNorm = 1.0 - distNorm;

        double resNorm = (hm.resourceRate - minRes) / resRange;

        hm.latency = latNorm;
        hm.distance = distNorm;
        hm.resourceRate = resNorm;
    }
}

/**
 * 判定是否可分配
 */
bool ABC_Scheduler::canAllocate(const MecHostInfo &host, const Orchestrator::AppDescriptorInfo &appInfo) const
{
    if (host.availableCpu < appInfo.cpu)
        return false;
    if (host.availableRam < appInfo.ram)
        return false;
    if (host.availableDisk < appInfo.disk)
        return false;
    return true;
}

/**
 * 简易距离计算
 */
double ABC_Scheduler::calcDistance(double lat1, double lon1, double lat2, double lon2) const
{
    double dlat = lat1 - lat2;
    double dlon = lon1 - lon2;
    return std::sqrt(dlat * dlat + dlon * dlon);
}

} // namespace MobiEdgeSim
