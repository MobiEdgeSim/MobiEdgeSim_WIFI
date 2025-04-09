#include "ResultLogger.h"
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <omnetpp.h>
#include <sys/stat.h>  // 用于检测文件大小/存在

namespace MobiEdgeSim {

// 静态成员初始化
ResultLogger* ResultLogger::instance = nullptr;
std::mutex ResultLogger::initMutex;

ResultLogger& ResultLogger::getInstance() {
    std::lock_guard<std::mutex> lock(initMutex);
    if (!instance) {
        instance = new ResultLogger();
    }
    return *instance;
}

ResultLogger::~ResultLogger() {
    // 释放 map 中的文件流
    for (auto& kv : fileMap) {
        if (kv.second && kv.second->is_open()) {
            kv.second->close();
        }
    }
}

// 获取或创建某个算法的输出文件流
std::ofstream& ResultLogger::getFileStream(const std::string& algorithmName) {
    auto it = fileMap.find(algorithmName);
    if (it != fileMap.end()) {
        // 已经存在，直接返回
        return *(it->second);
    }

    // 如果还没打开，则新建一个文件
    // 命名：placement_results_<algorithmName>.csv
    std::string fileName = "placement_results_" + algorithmName + ".csv";

    // 用 unique_ptr 管理，追加写
    auto ofsPtr = std::make_unique<std::ofstream>(fileName, std::ios::out | std::ios::app);
    if (!ofsPtr->is_open()) {
        throw std::runtime_error("Failed to open " + fileName);
    }

    // 检查文件是否是新创建（或空）
    // 这里简单用 tellp() == 0 判断，也可以用 stat() 检查文件大小
    if (ofsPtr->tellp() == 0) {
        // 写表头
        // 你可以把 algorithmName 也放在表头，或者后续再加
        (*ofsPtr) << "SimTime,Algorithm,AppName,BestHost,SchedulingTimeMs,BestLat,BestLon,AvailCpu,AvailRam,AvailDisk,AppCpu,AppRam,AppDisk,AppLat,AppLon\n";
        ofsPtr->flush();
    }

    // 把智能指针放进 map
    fileMap[algorithmName] = std::move(ofsPtr);

    // 再次查找并返回引用
    return *(fileMap[algorithmName]);
}

void ResultLogger::logPlacementResult(
    const std::string& algorithmName,
    const Orchestrator::AppDescriptorInfo& appInfo,
    const std::string& bestHostName,
    const std::vector<MecHostInfo>& hosts,
    double schedulingTimeMs
) {
    // 获取仿真当前时间
    omnetpp::simtime_t now = omnetpp::simTime();

    // 找到 bestHost 的一些信息
    double bestLat = 0.0, bestLon = 0.0;
    double availCpu = 0.0, availRam = 0.0, availDisk = 0.0;
    for (auto &host : hosts) {
        if (host.name == bestHostName) {
            bestLat    = host.latitude;
            bestLon    = host.longitude;
            availCpu   = host.availableCpu;
            availRam   = host.availableRam;
            availDisk  = host.availableDisk;
            break;
        }
    }

    // 获取对应的文件流
    std::ofstream& ofs = getFileStream(algorithmName);

    // 写一行CSV
    ofs << now                 << ","
        << algorithmName       << ","
        << appInfo.name        << ","
        << bestHostName        << ","
        << schedulingTimeMs    << ","
        << bestLat             << ","
        << bestLon             << ","
        << availCpu            << ","
        << availRam            << ","
        << availDisk           << ","
        << appInfo.cpu         << ","
        << appInfo.ram         << ","
        << appInfo.disk        << ","
        << appInfo.latitude    << ","
        << appInfo.longitude
        << "\n";

    ofs.flush();
}

void ResultLogger::flush(const std::string& algorithmName) {
    auto it = fileMap.find(algorithmName);
    if (it != fileMap.end() && it->second->is_open()) {
        it->second->flush();
    }
}

} // namespace MobiEdgeSim
