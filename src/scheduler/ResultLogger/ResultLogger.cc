#include "ResultLogger.h"
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <omnetpp.h>
#include <sys/stat.h>

namespace MobiEdgeSim {

ResultLogger *ResultLogger::instance = nullptr;
std::mutex ResultLogger::initMutex;

ResultLogger& ResultLogger::getInstance() {
    std::lock_guard<std::mutex> lock(initMutex);
    if (!instance) {
        instance = new ResultLogger();
    }
    return *instance;
}

ResultLogger::~ResultLogger() {
    for (auto &kv : fileMap) {
        if (kv.second && kv.second->is_open()) {
            kv.second->close();
        }
    }
}

std::ofstream& ResultLogger::getFileStream(const std::string &algorithmName) {
    auto it = fileMap.find(algorithmName);
    if (it != fileMap.end()) {

        return *(it->second);
    }

    std::string fileName = "placement_results_" + algorithmName + ".csv";

    auto ofsPtr = std::make_unique<std::ofstream>(fileName,
            std::ios::out | std::ios::app);
    if (!ofsPtr->is_open()) {
        throw std::runtime_error("Failed to open " + fileName);
    }

    if (ofsPtr->tellp() == 0) {
        (*ofsPtr)
                << "SimTime,Algorithm,AppName,BestHost,SchedulingTimeMs,BestLat,BestLon,AvailCpu,AvailRam,AvailDisk,AppCpu,AppRam,AppDisk,AppLat,AppLon\n";
        ofsPtr->flush();
    }

    fileMap[algorithmName] = std::move(ofsPtr);

    return *(fileMap[algorithmName]);
}

void ResultLogger::logPlacementResult(const std::string &algorithmName,
        const Orchestrator::AppDescriptorInfo &appInfo,
        const std::string &bestHostName, const std::vector<MecHostInfo> &hosts,
        double schedulingTimeMs) {

    omnetpp::simtime_t now = omnetpp::simTime();

    double bestLat = 0.0, bestLon = 0.0;
    double availCpu = 0.0, availRam = 0.0, availDisk = 0.0, latency = 0.0;
    for (auto &host : hosts) {
        std::cout<<"host name:"<<host.name<<"host lat:"<<host.latitude<<"host lon:"<<host.longitude<<"host cpu:"<<host.availableCpu<<"host ram:"<<host.availableRam<<"host disk:"<<host.availableDisk<<"host latency:"<<host.latency<<std::endl;
        if (host.name == bestHostName) {
            bestLat = host.latitude;
            bestLon = host.longitude;
            availCpu = host.availableCpu;
            availRam = host.availableRam;
            availDisk = host.availableDisk;
            latency = host.latency;
            break;
        }
    }

    std::ofstream &ofs = getFileStream(algorithmName);

    //std::cout << "simTime: " << simTime().str() << std::endl;

    std::cout <<"[Logger] " <<now << " algo: "<<algorithmName <<" ue name:"<< appInfo.name
            << " best host:" <<bestHostName << " execution time:" << schedulingTimeMs << " best host lat:" << bestLat << " "
            << "best host lon:"<< bestLon << " host avail_cpu:" << availCpu << " host avail_ram:" << availRam << " host avail_disk:" << availDisk<<" latency"<<latency
            << " request_cpu:" << appInfo.cpu << " request_ram:" << appInfo.ram << " request_disk:" << appInfo.disk
            << " ue lat:" << appInfo.latitude << " ue lon" << appInfo.longitude << std::endl;
    std::cout <<"----------------------------------------------------------------------------------------------------------------------------------------------------------"<<std::endl;
    ofs << now << "," << algorithmName << "," << appInfo.name << ","
            << bestHostName << "," << schedulingTimeMs << "," << bestLat << ","
            << bestLon << "," << availCpu << "," << availRam << "," << availDisk<<","<<latency
            << "," << appInfo.cpu << "," << appInfo.ram << "," << appInfo.disk
            << "," << appInfo.latitude << "," << appInfo.longitude << "\n";

    ofs.flush();
}

void ResultLogger::flush(const std::string &algorithmName) {
    auto it = fileMap.find(algorithmName);
    if (it != fileMap.end() && it->second->is_open()) {
        it->second->flush();
    }
}

} // namespace MobiEdgeSim
