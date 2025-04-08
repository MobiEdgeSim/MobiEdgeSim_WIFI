//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "orchestrator.h"
#include "inet/common/ModuleAccess.h"
#include <sstream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include "veins_inet/VeinsInetMobility.h"
#include "inet/networklayer/common/NetworkInterface.h"
#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/networklayer/ipv4/Ipv4InterfaceData.h"
#include "nodes/mecHost/mecHost.h"
#include "nodes/mobileNodes/mmecHost/mmecHost.h"
#include "scheduler/MetaheuristicScheduler.h"
#include "apps/UdpUeApp.h"
#include "inet/networklayer/common/L3AddressResolver.h"
// Emulation debug
#include <iostream>

#include <random>

namespace MobiEdgeSim {

Define_Module(Orchestrator);

void Orchestrator::initialize(int stage)
{
    cSimpleModule::initialize(stage);
    // Avoid multiple initializations
    if (stage != inet::INITSTAGE_LOCAL)
        return;

    EV << "Orchestrator::initialize " << endl;
    updateInterval = par("updateInterval").doubleValue();
    spInterval = par("spInterval").doubleValue();

    // initial the update time interval
    updateMsg = new cMessage("updateMecHost");
    spMsg = new cMessage("servicePlacement");
    scheduleAt(simTime() + 2 + updateInterval, updateMsg);
    scheduleAt(simTime() + 2 + spInterval, spMsg);

    requestRam = par("ramRequest").doubleValue();
    requestDisk = par("diskRequest").doubleValue();
    requestCpu = par("cpuRequest").doubleValue();
    algorithmName = par("algorithmName").stdstringValue();

    //first update
    updateMecHost();

}

void Orchestrator::handleMessage(cMessage *msg)
{
    if (strcmp(msg->getName(), "updateMecHost") == 0) {
        updateMecHost();
        scheduleAt(simTime() + updateInterval, updateMsg);
    }
    else if (strcmp(msg->getName(), "servicePlacement") == 0) {
        servicePlacement();
        if (spMsg->isScheduled())
            cancelEvent(spMsg);
        scheduleAt(simTime() + spInterval, spMsg);
    }
}

void Orchestrator::updateMecHost()
{
    EV << "Orchestrator::updateMecHost " << endl;
    std::vector<cModule*> newMecHosts;
    // get the root module
    cModule *systemModule = getSimulation()->getSystemModule();

    for (cModule::SubmoduleIterator it(systemModule); !it.end(); ++it) {
        cModule *mod = *it;
        if (strstr(mod->getName(), "mecHost") != nullptr) {
            newMecHosts.push_back(mod);
        }
    }

    bool hasChanged = false;

    //add
    for (auto newHost : newMecHosts) {
        if (std::find(mecHosts.begin(), mecHosts.end(), newHost) == mecHosts.end()) {
            mecHosts.push_back(newHost);
            hasChanged = true;
        }
    } //delete
    for (auto it = mecHosts.begin(); it != mecHosts.end();) {
        if (std::find(newMecHosts.begin(), newMecHosts.end(), *it) == newMecHosts.end()) {
            it = mecHosts.erase(it);
            hasChanged = true;
        }
        else {
            ++it;
        }
    }
    if (hasChanged) {
        updateMecHostListParam();
    }
    EV << "Updated mecHost list: ";
    for (auto host : mecHosts) {
        EV << host->getFullName() << " ";
    }
    EV << "\n";
}

void Orchestrator::updateMecHostListParam()
{
    std::string newMecHostList;
    for (auto host : mecHosts) {
        if (!newMecHostList.empty())
            newMecHostList += ",";
        newMecHostList += host->getFullName();
    }
    par("mecHostList").setStringValue(newMecHostList.c_str());
    EV << "Updated mecHostList parameter: " << newMecHostList << endl;
}

std::vector<std::string> Orchestrator::getMechostNames(inet::Coord currentCoord)
{
    EV << "Orchestrator::getMechostNames -- scanning submodules\n";

    std::vector<std::string> hostList;
    double wIFIDistance = par("wIFIDistance").doubleValue();

    cModule *systemModule = getSimulation()->getSystemModule();

    for (cModule::SubmoduleIterator it(systemModule); !it.end(); ++it) {
        cModule *mod = *it;

        MecHost *mecHost = dynamic_cast<MecHost*>(mod);
        MmecHost *mmecHost = dynamic_cast<MmecHost*>(mod);
        if (!mecHost && !mmecHost)
            continue;

        inet::Coord pos;
        if (mecHost) {
            pos.x = mecHost->getMecHostInfo().latitude;
            pos.y = mecHost->getMecHostInfo().longitude;
        }
        else {
            pos.x = mmecHost->getMecHostInfo().latitude;
            pos.y = mmecHost->getMecHostInfo().longitude;
        }

        double dx = pos.x - currentCoord.x;
        double dy = pos.y - currentCoord.y;
        double distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= wIFIDistance)
            hostList.push_back(mod->getFullName());
    }

    return hostList;
}

// construct UE  AppDescriptorInfo
Orchestrator::AppDescriptorInfo Orchestrator::buildAppDescriptor(cModule *ue)
{
    AppDescriptorInfo appInfo;
    appInfo.name = ue->getFullName();

    // location
    inet::Coord uePos;
    cModule *mobilityModule = ue->getSubmodule("mobility");
    if (mobilityModule) {
        veins::VeinsInetMobility *mob = check_and_cast<veins::VeinsInetMobility*>(mobilityModule);
        uePos = mob->getCurrentPosition();
    }
    else {
        uePos.x = ue->par("latitude").doubleValue();
        uePos.y = ue->par("longitude").doubleValue();
        uePos.z = 0;
    }
    appInfo.latitude = std::round(uePos.x * 1000.0) / 1000.0;
    appInfo.longitude = std::round(uePos.y * 1000.0) / 1000.0;

    //request resource
    double occupancy = uniform(0.2, 0.8);
    appInfo.ram = requestRam * (1 - occupancy);
    appInfo.disk = requestDisk * (1 - occupancy);
    appInfo.cpu = requestCpu * (1 - occupancy);

    // UE IP address
    inet::L3AddressResolver resolver;
    inet::L3Address ueAddr = resolver.resolve(ue->getFullName());
    appInfo.ueIpAddress = ueAddr.str();

    EV << "Built AppDescriptor for UE " << ue->getFullName() << ": RAM=" << appInfo.ram << ", Disk=" << appInfo.disk << ", CPU=" << appInfo.cpu
            << ", Position=(" << appInfo.latitude << ", " << appInfo.longitude << ")\n";

    return appInfo;
}

// build MEC hosts informations
std::vector<MobiEdgeSim::MecHostInfo> Orchestrator::buildMecHostInfos()
{
    std::vector<MecHostInfo> infos;

    cModule *systemModule = getSimulation()->getSystemModule();

    for (cModule::SubmoduleIterator it(systemModule); !it.end(); ++it) {
        cModule *mod = *it;

        MecHost *mecHost = dynamic_cast<MecHost*>(mod);
        MmecHost *mmecHost = dynamic_cast<MmecHost*>(mod);

        if (!mecHost && !mmecHost)
            continue;

        if (mecHost) {
            infos.push_back(mecHost->getMecHostInfo());
        }
        else { // mmecHost != nullptr
            infos.push_back(mmecHost->getMecHostInfo());
        }
    }
    return infos;
}

cModule* Orchestrator::findBestMecHostForUE(cModule *ue)
{
    EV << "Orchestrator::findBestMecHostForUE invoked for UE " << ue->getFullName() << "\n";
    AppDescriptorInfo appInfo = buildAppDescriptor(ue);

    std::vector<MecHostInfo> hostInfos = buildMecHostInfos();

    //get the latency
    MobiEdgeSim::UdpUeApp *udpApp = check_and_cast<MobiEdgeSim::UdpUeApp*>(ue->getSubmodule("udpUeApp"));
    if (udpApp) {
        const std::map<std::string, omnetpp::simtime_t> &rttMap = udpApp->getRttMap();
        for (auto &hostInfo : hostInfos) {
            auto it = rttMap.find(hostInfo.name);
            if (it != rttMap.end()) {
                EV << "Orchestrator get latency!!!!" << endl;
                hostInfo.latency = it->second.dbl() * 1000;
            }
            else {
                hostInfo.latency = 1e6;
            }
        }
    }
    else {
        EV << "UdpUeApp module not found in UE " << ue->getFullName() << "\n";
    }

    MetaheuristicScheduler scheduler(appInfo, hostInfos, algorithmName);
    std::string bestHostName = scheduler.findBestHost();
    EV << "MetaheuristicScheduler selected best host: " << bestHostName << "\n";

    for (auto host : mecHosts) {
        if (host->getFullName() == bestHostName) {

            MecHost *selectedHost = check_and_cast<MecHost*>(host);
            selectedHost->updateResources(requestRam, requestDisk, requestCpu);
            return host;
        }
    }
    return nullptr;
}

void Orchestrator::servicePlacement()
{
    EV << "Orchestrator::servicePlacement invoked" << "\n";
    ues.clear();
    cModule *systemModule = getSimulation()->getSystemModule();

    for (cModule::SubmoduleIterator it(systemModule); !it.end(); ++it) {
        cModule *mod = *it;
        if (strstr(mod->getName(), "ue") != nullptr) {
            ues.push_back(mod);
        }
    }
    for (auto ue : ues) {
        cModule *bestHost = findBestMecHostForUE(ue);
        if (bestHost) {
            EV << "Service Placement: For UE [" << ue->getFullName() << "], best MEC host selected is [" << bestHost->getFullName()
                    << "] and its resources have been updated.\n";
        }
        else {
            EV << "Service Placement: For UE [" << ue->getFullName() << "], no suitable MEC host found.\n";
        }
    }
}

void Orchestrator::finish()
{
    cancelAndDelete(updateMsg);
    cancelAndDelete(spMsg);
}
}
