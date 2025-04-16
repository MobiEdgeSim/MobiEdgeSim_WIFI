//
//
// 

#include "mecHost.h"
#include "inet/mobility/static/StationaryMobility.h"
#include "veins_inet/VeinsInetMobility.h"

namespace MobiEdgeSim {

Define_Module(MecHost);

void MecHost::initialize()
{
    currentInfo.name = getFullName();

    //resource
    double maxRam = par("maxRam").doubleValue();
    double maxDisk = par("maxDisk").doubleValue();
    double maxCpu = par("maxCpuSpeed").doubleValue();

    resource_used_pro_up = par("resource_used_pro_up").doubleValue();
    resource_used_pro_low = par("resource_used_pro_low").doubleValue();
    double occupancy = uniform(resource_used_pro_low, resource_used_pro_up);
    //double occupancy = 0;
    currentInfo.availableRam = maxRam * (1 - occupancy);
    currentInfo.availableDisk = maxDisk * (1 - occupancy);
    currentInfo.availableCpu = maxCpu * (1 - occupancy);

    currentInfo.ram = maxRam;
    currentInfo.disk = maxDisk;
    currentInfo.cpu = maxCpu;

    cModule *mobilityModule = getSubmodule("mobility");
    if (mobilityModule) {
        std::string nedType = mobilityModule->getNedTypeName();
        inet::Coord pos;
        if (nedType.find("VeinsInetMobility") != std::string::npos) {
            auto mobility = check_and_cast<veins::VeinsInetMobility*>(mobilityModule);
            pos = mobility->getCurrentPosition();
        }
        else if (nedType.find("StationaryMobility") != std::string::npos) {
            auto mobility = check_and_cast<inet::StationaryMobility*>(mobilityModule);
            pos = mobility->getCurrentPosition();
        }
        else {
            auto mobility = check_and_cast<inet::StationaryMobilityBase*>(mobilityModule);
            pos = mobility->getCurrentPosition();
        }
        currentInfo.latitude = pos.x;
        currentInfo.longitude = pos.y;
    }
    else {
        // no mobility module
    }

    // set default values
    currentInfo.latency = 1e6; //currently the latency information is meaningless
    EV << "MecHost " << getFullName() << ", RAM: " << currentInfo.availableRam << ", Disk: " << currentInfo.availableDisk << ", CPU: "
            << currentInfo.availableCpu << "\n";

    // read requestProcessTime from NED/ini file
    requestProcessTime = par("requestProcessTime").doubleValue();

    updatePositionInterval = par("updatePositionInterval").doubleValue();
    updatePosition();
    updatePositionMsg = new cMessage("updatePosition");
    scheduleAt(simTime() + updatePositionInterval, updatePositionMsg);

}

void MecHost::handleMessage(cMessage *msg)
{

    // 1) check if it's the position update timer
    if (msg == updatePositionMsg) {
        updatePosition();
        scheduleAt(simTime() + updatePositionInterval, updatePositionMsg);
        return;
    }else if (strcmp(msg->getName(), "releaseTimer")==0) {// 2)it's a releaseTimer
        // retrieve the resource usage from cMessage
        double relRam = msg->par("releaseRam").doubleValue();
        double relDisk = msg->par("releaseDisk").doubleValue();
        double relCpu = msg->par("releaseCpu").doubleValue();

        releaseResources(relRam, relDisk, relCpu);

        EV << "MecHost " << getFullName() << ": auto-release from timer. Freed CPU=" << relCpu << ", RAM=" << relRam << ", Disk=" << relDisk << "\n";
    }
    delete msg;
}

void MecHost::updatePosition()
{
    cModule *mobilityModule = getSubmodule("mobility");
    if (mobilityModule) {
        std::string nedType = mobilityModule->getNedTypeName();
        inet::Coord pos;
        if (nedType.find("VeinsInetMobility") != std::string::npos) {
            auto mobility = check_and_cast<veins::VeinsInetMobility*>(mobilityModule);
            pos = mobility->getCurrentPosition();
        }
        else if (nedType.find("StationaryMobility") != std::string::npos) {
            auto mobility = check_and_cast<inet::StationaryMobility*>(mobilityModule);
            pos = mobility->getCurrentPosition();
        }
        else {
            auto mobility = check_and_cast<inet::StationaryMobilityBase*>(mobilityModule);
            pos = mobility->getCurrentPosition();
        }
        currentInfo.latitude = std::round(pos.x * 1000.0) / 1000.0;
        currentInfo.longitude = std::round(pos.y * 1000.0) / 1000.0;
        EV << "MecHost " << getFullName() << " updated position: (" << currentInfo.latitude << ", " << currentInfo.longitude << ")\n";
    }
    else {
        EV_WARN << "MecHost " << getFullName() << " has no mobility submodule, cannot update position.\n";
    }
}

const MecHostInfo& MecHost::getMecHostInfo() const
{
    return currentInfo;
}

void MecHost::updateResources(double allocatedRam, double allocatedDisk, double allocatedCPU)
{

    currentInfo.availableRam -= allocatedRam;
    currentInfo.availableDisk -= allocatedDisk;
    currentInfo.availableCpu -= allocatedCPU;
    EV << "MecHost " << getFullName() << " updated resources: availableRam=" << currentInfo.availableRam << ", availableDisk="
            << currentInfo.availableDisk << ", availableCpu=" << currentInfo.availableCpu << "\n";

    //std::cout<<"requestProcessTime = "<<requestProcessTime<<std::endl;
    // schedule auto-release
    double half = requestProcessTime * 0.5;
    double range = requestProcessTime; // so total = [0.5T, 1.5T]
    //std::cout << "MecHost " << getFullName() << " scheduled auto-release in [" << half << ", " << half + range << "] seconds.\n";
    double actualTime = uniform(half,  half + range);//TODO

    cMessage *releaseMsg = new cMessage("releaseTimer");
    // store the resource usage in msg parameters
    releaseMsg->addPar("releaseRam") = allocatedRam;
    releaseMsg->addPar("releaseDisk") = allocatedDisk;
    releaseMsg->addPar("releaseCpu") = allocatedCPU;

    //std::cout << "MecHost " << getFullName() << " scheduled release timer in " << actualTime << " seconds.\n";
    take(releaseMsg);
    scheduleAt(simTime() + actualTime, releaseMsg);
}

void MecHost::releaseResources(double releasedRam, double releasedDisk, double releasedCPU)
{

    currentInfo.availableRam += releasedRam;
    currentInfo.availableDisk += releasedDisk;
    currentInfo.availableCpu += releasedCPU;
    EV << "MecHost " << getFullName() << " released resources: availableRam=" << currentInfo.availableRam << ", availableDisk="
            << currentInfo.availableDisk << ", availableCpu=" << currentInfo.availableCpu << "\n";
}

void MecHost::updateStatus(const MecHostInfo &newInfo)
{
    currentInfo = newInfo;
    EV << "MecHost " << getFullName() << " status updated.\n";
}

}
