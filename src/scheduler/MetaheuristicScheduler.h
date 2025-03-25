#ifndef METAHEURISTIC_SCHEDULER_H
#define METAHEURISTIC_SCHEDULER_H

#include <jni.h>
#include <vector>
#include <string>
#include "JVMManager.h"
#include "nodes/mecHost/mecHost.h"
#include "nodes/orchestrator/orchestrator.h"
namespace MobiEdgeSim{
class MetaheuristicScheduler {
public:
    MetaheuristicScheduler(const Orchestrator::AppDescriptorInfo &appInfo,
                           const  std::vector<MecHostInfo> &mecHostInfos,
                           const std::string &algorithmName);
    ~MetaheuristicScheduler();
    std::string findBestHost();
    std::string algorithmName;

private:
    JNIEnv* env;
    jobject scheduler;
    jclass schedulerClass;
    std::string bestHostName;
};
}

#endif // METAHEURISTIC_SCHEDULER_H
