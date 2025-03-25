//
//
// 

#include "mecHost.h"
#include "inet/mobility/static/StationaryMobility.h"

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
        auto mobility = check_and_cast<inet::StationaryMobilityBase*>(mobilityModule);
        const inet::Coord &pos = mobility->getCurrentPosition();
        currentInfo.latitude = pos.x;
        currentInfo.longitude = pos.y;
    }else{

    }
    currentInfo.latency = 1e6;//currently the latency information is meaningless
    EV << "MecHost " << getFullName() << ", RAM: " << currentInfo.availableRam<< ", Disk: " << currentInfo.availableDisk << ", CPU: "<< currentInfo.availableCpu << "\n";

}

void MecHost::handleMessage(cMessage *msg) {

}
const MecHostInfo& MecHost::getMecHostInfo() const {
    return currentInfo;
}

void MecHost::updateResources(double allocatedRam, double allocatedDisk,double allocatedCPU) {

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
