/*
 * ABC_Scheduler.cpp
 *
 *  Created on: Apr 8, 2025
 *      Author: tianhao
 */
#include "ABC_Scheduler.h"
#include <iostream>

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

        sendEmployedBees(appInfo, hosts);

        double maxFit = bees[0].fitness;
        for (int i = 1; i < populationSize; ++i) {
            if (bees[i].fitness > maxFit) {
                maxFit = bees[i].fitness;
            }
        }
        for (int i = 0; i < populationSize; ++i) {
            probs[i] = 0.9 * (bees[i].fitness / maxFit) + 0.1;
        }

        sendOnlookerBees(appInfo, hosts);
        calculateFitnessAll(appInfo, hosts);
        elitism();

        sendScoutBees(appInfo, hosts);
    }

    Bee &bestBee = bees[0];
    if (bestBee.hostIndex < 0 || bestBee.hostIndex >= (int) hosts.size()) {
        return "mecHost_null";
    }
    if (!canAllocate(hosts[bestBee.hostIndex], appInfo)) {
        return "mecHost_null";
    }
    return hosts[bestBee.hostIndex].name;
}

/**
 *
 */
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
    for (int i = 0; i < populationSize; ++i) {
        int hIdx = bees[i].hostIndex;
        if (hIdx < 0 || hIdx >= (int) hosts.size()) {
            bees[i].fitness = -1e9;
            continue;
        }

        bees[i].fitness = computeFitness(hIdx, appInfo, hosts);
    }
}


void ABC_Scheduler::sendEmployedBees(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts)
{
    for (int i = 0; i < populationSize; ++i) {
        employedMove(i, appInfo, hosts);
    }
}


void ABC_Scheduler::sendOnlookerBees(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts)
{
    std::uniform_real_distribution<double> distReal(0.0, 1.0);
    for (int i = 0; i < populationSize; ++i) {
        if (distReal(rng) < probs[i]) {
            employedMove(i, appInfo, hosts);
        }
    }
}


void ABC_Scheduler::sendScoutBees(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts)
{
    int maxTrialIndex = 0;
    int maxTrialVal = bees[0].trial;
    for (int i = 2; i < populationSize; ++i) {
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


void ABC_Scheduler::employedMove(int beeIndex, const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts)
{

    Bee currentBee = bees[beeIndex];
    double oldFitness = currentBee.fitness;

    std::uniform_real_distribution<double> distReal(-1.0, 1.0);
    std::uniform_int_distribution<int> distBee(0, populationSize - 1);

    for (int t = 0; t < tryTime; ++t) {

        int neighborIdx = distBee(rng);
        while (neighborIdx == beeIndex) {
            neighborIdx = distBee(rng);
        }
        int neighborHostIndex = bees[neighborIdx].hostIndex;


        double phi = distReal(rng); // [-1,1]
        int oldVal = currentBee.hostIndex;
        int updated = (int) std::lround(oldVal + (oldVal - neighborHostIndex) * phi);


        if (updated < 0) {
            updated = 0;
        }
        else if (updated >= (int) hosts.size()) {
            updated = (int) hosts.size() - 1;
        }


        double newFit = computeFitness(updated, appInfo, hosts);

        if (newFit > oldFitness) {
            bees[beeIndex].hostIndex = updated;
            bees[beeIndex].fitness = newFit;
            bees[beeIndex].trial = 0;
            return;
        }
    }

    bees[beeIndex].trial++;
}


double ABC_Scheduler::computeFitness(int hostIndex, const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts) const
{
    if (hostIndex < 0 || hostIndex >= (int) hosts.size()) {
        return -1e9;
    }


    const auto &host = hosts[hostIndex];
    if (!canAllocate(host, appInfo)) {
        return -1e9;
    }


    double latency = host.latency;
    double distance = calcDistance(host.latitude, host.longitude, appInfo.latitude, appInfo.longitude);

    double cpuRate = (host.cpu > 0) ? (double) host.availableCpu / (double) host.cpu : 0.0;
    double ramRate = (host.ram > 0) ? (double) host.availableRam / (double) host.ram : 0.0;
    double diskRate = (host.disk > 0) ? (double) host.availableDisk / (double) host.disk : 0.0;
    double resourceRate = (cpuRate + ramRate + diskRate) /3.0;


    double invLatency = 1.0 / (1.0 + latency);
    double invDistance = 1.0 / (1.0 + distance);

    double wLatency = 0.6;
    double wResource = 0.2;
    double wLocation = 0.2;

    double fitnessVal = wLatency * invLatency + wResource * resourceRate + wLocation * invDistance;

    return fitnessVal;
}


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


double ABC_Scheduler::calcDistance(double lat1, double lon1, double lat2, double lon2) const
{
    double dlat = lat1 - lat2;
    double dlon = lon1 - lon2;
    return std::sqrt(dlat * dlat + dlon * dlon);
}

} // namespace MobiEdgeSim
