/*
 * ABC_Scheduler.h
 *
 *  Created on: Apr 8, 2025
 *      Author: tianhao
 */

#ifndef SCHEDULER_PLACEMENTPOLICY_ABC_ABC_SCHEDULER_H_
#define SCHEDULER_PLACEMENTPOLICY_ABC_ABC_SCHEDULER_H_

#include "scheduler/SchedulerInterface.h"
#include <random>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

namespace MobiEdgeSim {

/*
 *
 */
struct Bee
{
    int hostIndex;
    double fitness;
    int trial;

    Bee() : hostIndex(-1), fitness(-1.0), trial(0){}
};

class ABC_Scheduler : public SchedulerInterface
{
public:
    std::string findBestHost(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts) override;


    ABC_Scheduler(int popSize = 100, int iterations = 200, int limit = 30, int tryTimeSet = 30) :
            populationSize(popSize), maxIterations(iterations), limitTrial(limit), tryTime(tryTimeSet)
    {
        rng.seed(std::random_device { }());
    }

private:

    int populationSize;
    int maxIterations;
    int limitTrial;
    int tryTime;


    std::vector<Bee> bees;
    std::vector<double> probs; //save probabilities in on-look stage

    std::mt19937 rng;

private:
    //core functions
    void initBees(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts);
    void calculateFitnessAll(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts);
    void sendEmployedBees(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts);
    void sendOnlookerBees(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts);
    void sendScoutBees(const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts);
    void elitism(); // save the best bee in bee[0]


    void employedMove(int beeIndex, const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts);
    double computeFitness(int hostIndex, const Orchestrator::AppDescriptorInfo &appInfo, const std::vector<MecHostInfo> &hosts) const;
    bool canAllocate(const MecHostInfo &host, const Orchestrator::AppDescriptorInfo &appInfo) const;
    double calcDistance(double lat1, double lon1, double lat2, double lon2) const;
};

} /* namespace MobiEdgeSim */

#endif /* SCHEDULER_PLACEMENTPOLICY_ABC_ABC_SCHEDULER_H_ */
