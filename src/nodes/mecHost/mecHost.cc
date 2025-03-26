//
//
// 

#include "mecHost.h"
#include "inet/mobility/static/StationaryMobility.h"
#include "veins_inet/VeinsInetMobility.h"


namespace MobiEdgeSim {

Define_Module(MecHost);

void MecHost::initialize() {
    currentInfo.name = getFullName();

    //resource
    double maxRam = par("maxRam").doubleValue();
    double maxDisk = par("maxDisk").doubleValue();
    double maxCpu = par("maxCpuSpeed").doubleValue();

    double occupancy = uniform(0.2, 0.8);
    currentInfo.availableRam = maxRam * (1 - occupancy);
    currentInfo.availableDisk = maxDisk * (1 - occupancy);
    currentInfo.availableCpu = maxCpu * (1 - occupancy);

    currentInfo.ram = maxRam;
    currentInfo.disk = maxDisk;
    currentInfo.cpu = maxCpu;

    cModule *mobilityModule = getSubmodule("mobility");
    if (mobilityModule) {
        auto mobility = check_and_cast<inet::StationaryMobilityBase*>(
                mobilityModule);
        const inet::Coord &pos = mobility->getCurrentPosition();
        currentInfo.latitude = pos.x;
        currentInfo.longitude = pos.y;
    } else {

    }
    currentInfo.latency = 1e6; //currently the latency information is meaningless
    EV << "MecHost " << getFullName() << ", RAM: " << currentInfo.availableRam
              << ", Disk: " << currentInfo.availableDisk << ", CPU: "
              << currentInfo.availableCpu << "\n";

    updatePositionInterval = par("updatePositionInterval").doubleValue();
    // 创建更新位置的自消息
    updatePositionMsg = new cMessage("updatePosition");
    scheduleAt(simTime() + updatePositionInterval, updatePositionMsg);

}

void MecHost::handleMessage(cMessage *msg) {

    if (msg == updatePositionMsg) {
        updatePosition();
        // 重新安排下一个位置更新
        scheduleAt(simTime() + updatePositionInterval, updatePositionMsg);
    } else {
        // 其他消息处理
        delete msg;
    }
}

void MecHost::updatePosition() {
    // 如果存在 mobility 子模块，则更新位置，否则保持原值
    cModule *mobilityModule = getSubmodule("mobility");
    if (mobilityModule) {
        // 如果你有两种不同的 mobility 模块（例如 StationaryMobilityBase 和 VeinsInetMobility），
        // 可以根据模块类型进行判断：
        std::string nedType = mobilityModule->getNedTypeName();
        inet::Coord pos;
        if (nedType.find("VeinsInetMobility") != std::string::npos) {
            auto mobility = check_and_cast<veins::VeinsInetMobility*>(mobilityModule);
            pos = mobility->getCurrentPosition();
        } else if (nedType.find("StationaryMobility") != std::string::npos) {
            auto mobility = check_and_cast<inet::StationaryMobility*>(mobilityModule);
            pos = mobility->getCurrentPosition();
        } else {
            // 默认使用基类接口
            auto mobility = check_and_cast<inet::StationaryMobilityBase*>(mobilityModule);
            pos = mobility->getCurrentPosition();
        }
        currentInfo.latitude = std::round(pos.x * 1000.0) / 1000.0;
        currentInfo.longitude = std::round(pos.y * 1000.0) / 1000.0;
        EV << "MecHost " << getFullName() << " updated position: ("
                  << currentInfo.latitude << ", " << currentInfo.longitude
                  << ")\n";
    } else {
        EV_WARN << "MecHost " << getFullName()
                       << " has no mobility submodule, cannot update position.\n";
    }
}

const MecHostInfo& MecHost::getMecHostInfo() const {
    return currentInfo;
}

void MecHost::updateResources(double allocatedRam, double allocatedDisk,
        double allocatedCPU) {

    currentInfo.availableRam -= allocatedRam;
    currentInfo.availableDisk -= allocatedDisk;
    currentInfo.availableCpu -= allocatedCPU;
    EV << "MecHost " << getFullName() << " updated resources: availableRam="
              << currentInfo.availableRam << ", availableDisk="
              << currentInfo.availableDisk << ", availableCpu="
              << currentInfo.availableCpu << "\n";
}

void MecHost::updateStatus(const MecHostInfo &newInfo) {
    currentInfo = newInfo;
    EV << "MecHost " << getFullName() << " status updated.\n";
}

}
