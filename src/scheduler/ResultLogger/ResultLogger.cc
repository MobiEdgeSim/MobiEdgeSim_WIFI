#include "ResultLogger.h"
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <omnetpp.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace MobiEdgeSim {

ResultLogger *ResultLogger::instance = nullptr;
std::mutex ResultLogger::initMutex;
int ResultLogger::requestIndex = 0;

ResultLogger& ResultLogger::getInstance()
{
    std::lock_guard<std::mutex> lock(initMutex);
    if (!instance) {
        instance = new ResultLogger();

#ifdef _WIN32
        _mkdir("results");
        _mkdir("results\\logs");
#else
        mkdir("results", 0777);
        mkdir("results/logs", 0777);
#endif
    }
    return *instance;
}

ResultLogger::~ResultLogger()
{
    for (auto &kv : fileMap) {
        if (kv.second && kv.second->is_open()) {
            kv.second->close();
        }
    }
}

std::ofstream& ResultLogger::getFileStream(const std::string &algorithmName)
{
    auto it = fileMap.find(algorithmName);
    if (it != fileMap.end()) {

        return *(it->second);
    }

    std::string fileName = "results/placement_results_" + algorithmName + ".csv";

    auto ofsPtr = std::make_unique<std::ofstream>(fileName, std::ios::out | std::ios::app);
    if (!ofsPtr->is_open()) {
        throw std::runtime_error("Failed to open " + fileName);
    }

    if (ofsPtr->tellp() == 0) {
        (*ofsPtr)
                << "Index,SimTime,Algorithm,UeName,BestHost,SchedulingTimeMs,BestHost_Lat,BestHost_Lon,BestHost_AvailCpu,BestHost_AvailRam,BestHost_AvailDisk,Latency,Request_Cpu,Request_Ram,Request_Disk,Request_Lat,Request_Lon,distance,TotalMecHosts,TotalMmecHosts\n";
        ofsPtr->flush();
    }

    fileMap[algorithmName] = std::move(ofsPtr);

    return *(fileMap[algorithmName]);
}

void ResultLogger::logPlacementResult(const std::string &algorithmName, const Orchestrator::AppDescriptorInfo &appInfo,
        const std::string &bestHostName, const std::vector<MecHostInfo> &hosts, double schedulingTimeMsint)
{

    omnetpp::simtime_t now = omnetpp::simTime();
    ++requestIndex;

    double bestLat = 0.0, bestLon = 0.0;
    double availCpu = 0.0, availRam = 0.0, availDisk = 0.0, latency = 0.0;
    double distance = 0.0;
    int totalMecHosts = 0;
    int totalMmecHosts = 0;

    for (auto &host : hosts) {

        if (host.name.rfind("mmecHost", 0) == 0) {
            totalMmecHosts++;
        }
        else if (host.name.rfind("mecHost", 0) == 0) {
            totalMecHosts++;
        }
        std::cout << "host name:" << host.name << "host lat:" << host.latitude << "host lon:" << host.longitude << "host cpu:" << host.availableCpu
                << "host ram:" << host.availableRam << "host disk:" << host.availableDisk << "host latency:" << host.latency << std::endl;
        if (host.name == bestHostName) {
            bestLat = host.latitude;
            bestLon = host.longitude;
            availCpu = host.availableCpu;
            availRam = host.availableRam;
            availDisk = host.availableDisk;
            latency = host.latency;

            distance = std::sqrt(std::pow(appInfo.latitude - host.latitude, 2) + std::pow(appInfo.longitude - host.longitude, 2));
            //break;// if break will stop cout the host info
        }
    }

    std::ofstream &ofs = getFileStream(algorithmName);

    std::cout << "simTime: " << simTime().str() << std::endl;

    std::cout << "[Logger] " << now << " algo: " << algorithmName << " ue name:" << appInfo.name << " best host:" << bestHostName
            << " execution time:" << schedulingTimeMsint << " best host lat:" << bestLat << " " << "best host lon:" << bestLon << " host avail_cpu:"
            << availCpu << " host avail_ram:" << availRam << " host avail_disk:" << availDisk << " latency" << latency << " request_cpu:"
            << appInfo.cpu << " request_ram:" << appInfo.ram << " request_disk:" << appInfo.disk << " ue lat:" << appInfo.latitude << " ue lon:"
            << appInfo.longitude << " distance:" << distance  << " totalMecHosts:" << totalMecHosts
            << " totalMmecHosts:" << totalMmecHosts<< std::endl;
    std::cout
            << "----------------------------------------------------------------------------------------------------------------------------------------------------------"
            << std::endl;
    ofs << requestIndex << "," << now << "," << algorithmName << "," << appInfo.name << "," << bestHostName << "," << schedulingTimeMsint << ","
            << bestLat << "," << bestLon << "," << availCpu << "," << availRam << "," << availDisk << "," << latency << "," << appInfo.cpu << ","
            << appInfo.ram << "," << appInfo.disk << "," << appInfo.latitude << "," << appInfo.longitude << "," << distance << ","<< totalMecHosts << ","
            << totalMmecHosts
            << "\n";

    ofs.flush();

    std::string txtFilename = "results/logs/" + algorithmName + "_request_" + std::to_string(requestIndex) + ".txt";
    std::ofstream txtofs(txtFilename);
    if (txtofs.is_open()) {
        txtofs << "Request Index: " << requestIndex << "\n" << "SimTime: " << now << "\n" << "Algorithm: " << algorithmName << "\n" << "UE Name: "
                << appInfo.name << "\n" << "UE Position: (" << appInfo.latitude << ", " << appInfo.longitude << ")\n" << "UE Request: CPU="
                << appInfo.cpu << ", RAM=" << appInfo.ram << ", Disk=" << appInfo.disk << "\n" << "Best Host: " << bestHostName << "\n"
                << "Best Host Position: (" << bestLat << ", " << bestLon << ")\n" << "Best Host Avail CPU=" << availCpu << ", Avail RAM=" << availRam
                << ", Avail Disk=" << availDisk << "\n" << "Best Host Latency=" << latency << "\n"<< "Distance (UE->BestHost)=" << distance << "\n"
                << "Total MecHost: " << totalMecHosts << "\n"
                << "Total MmecHost: " << totalMmecHosts << "\n\n";
        txtofs << "------ All Candidate Hosts ------\n";
        for (auto &host : hosts) {
            double hostDistance = std::sqrt(std::pow(appInfo.latitude - host.latitude, 2) + std::pow(appInfo.longitude - host.longitude, 2));
            txtofs << "* Host: " << host.name << "\n   Position: (" << host.latitude << ", " << host.longitude << ")" << "\n   Avail CPU="
                    << host.availableCpu << ", Avail RAM=" << host.availableRam << ", Avail Disk=" << host.availableDisk << "\n   Latency="
                    << host.latency << "\n   Distance to UE=" << hostDistance << "\n--------------------------------\n";
        }

        txtofs.close();
    }
    else {
        std::cerr << "[Logger] Failed to open " << txtFilename << " for writing." << std::endl;
    }

}

void ResultLogger::flush(const std::string &algorithmName)
{
    auto it = fileMap.find(algorithmName);
    if (it != fileMap.end() && it->second->is_open()) {
        it->second->flush();
    }
}

} // namespace MobiEdgeSim
