#ifndef SCHEDULER_RESULTLOGGER_RESULTLOGGER_H_
#define SCHEDULER_RESULTLOGGER_RESULTLOGGER_H_

#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <memory>
#include <fstream>
#include "nodes/orchestrator/orchestrator.h"
#include "nodes/mecHost/mecHost.h"

namespace MobiEdgeSim {

class ResultLogger {
private:
    static ResultLogger* instance;
    static std::mutex initMutex;

    ResultLogger() = default;

    std::unordered_map<std::string, std::unique_ptr<std::ofstream>> fileMap;

    static int requestIndex;

public:
    static ResultLogger& getInstance();
    ~ResultLogger();


    ResultLogger(const ResultLogger&) = delete;
    ResultLogger& operator=(const ResultLogger&) = delete;


    void logPlacementResult(
        const std::string& algorithmName,
        const Orchestrator::AppDescriptorInfo& appInfo,
        const std::string& bestHostName,
        const std::vector<MecHostInfo>& hosts,
        double schedulingTimeMs
    );

    void flush(const std::string& algorithmName);
private:
    std::ofstream& getFileStream(const std::string& algorithmName);
};

} // namespace MobiEdgeSim

#endif /* SCHEDULER_RESULTLOGGER_RESULTLOGGER_H_ */
