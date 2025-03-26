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
    scheduleAt(simTime() + updateInterval, updateMsg);
    scheduleAt(simTime() +2+ spInterval, spMsg);

    requestRam = par("ramRequest").doubleValue();
    requestDisk = par("diskRequest").doubleValue();;
    requestCpu = par("cpuRequest").doubleValue();;
    algorithmName = par("algorithmName").stdstringValue();

    //first update
    updateMecHost();

}

void Orchestrator::handleMessage(cMessage *msg)
{
    if (strcmp(msg->getName(), "updateMecHost") == 0) {
        updateMecHost();
        scheduleAt(simTime() + updateInterval, updateMsg);
    } else if (strcmp(msg->getName(), "servicePlacement") == 0) {
        servicePlacement();
        // 先取消之前调度的 spMsg（如果仍在队列中）
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
    }//delete
    for (auto it = mecHosts.begin(); it != mecHosts.end(); ) {
        if (std::find(newMecHosts.begin(), newMecHosts.end(), *it) == newMecHosts.end()) {
            it = mecHosts.erase(it);
            hasChanged = true;
        } else {
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
    EV << "Orchestrator::getMechostNames -- Calculating distance limitations" << endl;
    std::vector<std::string> hostList;
    double wIFIDistance = par("wIFIDistance").doubleValue();

    // 遍历 orchestrator 内部维护的 mecHosts
       for (auto mod : mecHosts) {
           // 直接获取 MEC host 模块中保存的 currentInfo
           MecHost *mecHost = check_and_cast<MecHost*>(mod);
           const MecHostInfo &info = mecHost->getMecHostInfo();

           // 使用 currentInfo 中保存的 latitude 和 longitude 作为位置
           inet::Coord pos;
           pos.x = info.latitude;
           pos.y = info.longitude;
           pos.z = 0;  // 假设 z 坐标为 0

           // 计算与当前坐标的欧氏距离
           double dx = pos.x - currentCoord.x;
           double dy = pos.y - currentCoord.y;
           double dz = pos.z - currentCoord.z;
           double distance = std::sqrt(dx * dx + dy * dy + dz * dz);

           EV << "Distance from " << mod->getFullName() << " = " << distance << endl;
           if (distance <= wIFIDistance) {
               EV << mod->getFullName() << ": (" << pos.x << ", " << pos.y << ", " << pos.z
                  << "), distance: " << distance << "\n";
               hostList.push_back(mod->getFullName());
           }
       }
       return hostList;
}

// construct UE  AppDescriptorInfo
Orchestrator::AppDescriptorInfo Orchestrator::buildAppDescriptor(cModule* ue)
{
    AppDescriptorInfo appInfo;
    appInfo.name = ue->getFullName();

    // location
    inet::Coord uePos;
    cModule* mobilityModule = ue->getSubmodule("mobility");
    if (mobilityModule) {
        veins::VeinsInetMobility* mob = check_and_cast<veins::VeinsInetMobility*>(mobilityModule);
        uePos = mob->getCurrentPosition();
    } else {
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


    EV << "Built AppDescriptor for UE " << ue->getFullName() << ": RAM=" << appInfo.ram
       << ", Disk=" << appInfo.disk << ", CPU=" << appInfo.cpu << ", Position=(" << appInfo.latitude << ", " << appInfo.longitude << ")\n";

    return appInfo;
}

// build MEC hosts informations
std::vector<MobiEdgeSim::MecHostInfo> Orchestrator::buildMecHostInfos()
{
    std::vector<MecHostInfo> infos;
    for (auto host : mecHosts) {
        // 将 cModule* 转换为 MecHost* 并调用接口
        MecHost *mecHost = check_and_cast<MecHost*>(host);
        const MecHostInfo &info = mecHost->getMecHostInfo();
        infos.push_back(info);
    }
    return infos;
}

cModule* Orchestrator::findBestMecHostForUE(cModule* ue)
{
    EV << "Orchestrator::findBestMecHostForUE invoked for UE " << ue->getFullName() << "\n";
    // 构造该 UE 的应用描述
    AppDescriptorInfo appInfo = buildAppDescriptor(ue);
    // 从当前的 MEC 主机列表中获取最新状态
    std::vector<MecHostInfo> hostInfos = buildMecHostInfos();

    //get the latency
    MobiEdgeSim::UdpUeApp *udpApp = check_and_cast<MobiEdgeSim::UdpUeApp*>(ue->getSubmodule("udpUeApp"));
    if (udpApp) {
        const std::map<std::string, omnetpp::simtime_t> &rttMap = udpApp->getRttMap();
        for (auto &hostInfo : hostInfos) {
            auto it = rttMap.find(hostInfo.name);
            if (it != rttMap.end()) {
                hostInfo.latency = it->second.dbl(); // 将 simtime_t 转为 double
            } else {
                // 如果没有找到对应的延迟，设置一个默认较高的延迟（例如 1e6）
                hostInfo.latency = 1e6;
            }
        }
    } else {
        EV << "UdpUeApp module not found in UE " << ue->getFullName() << "\n";
    }

    // 调用 MetaheuristicScheduler 进行调度
    MetaheuristicScheduler scheduler(appInfo, hostInfos, algorithmName);
    std::string bestHostName = scheduler.findBestHost();
    EV << "MetaheuristicScheduler selected best host: " << bestHostName << "\n";

    // 在 mecHosts 列表中找到对应的 MEC 主机模块
    for (auto host : mecHosts) {
        if (host->getFullName() == bestHostName) {
            // 更新该 MEC 主机的资源：假设资源需求由 requestRam, requestDisk, requestCpu 指定
            // 注意：updateResources 需要在 MecHost 中实现，通常是减少可用资源
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
    cModule* systemModule = getSimulation()->getSystemModule();
    // 搜索所有名称中包含 "ue" 的模块作为 UE 列表
    for (cModule::SubmoduleIterator it(systemModule); !it.end(); ++it) {
        cModule* mod = *it;
        if (strstr(mod->getName(), "ue") != nullptr) {
            ues.push_back(mod);
        }
    }
    // 对每个 UE 进行服务放置
    for (auto ue : ues) {
        cModule* bestHost = findBestMecHostForUE(ue);
        if (bestHost) {
            EV << "Service Placement: For UE [" << ue->getFullName() << "], best MEC host selected is ["
               << bestHost->getFullName() << "] and its resources have been updated.\n";
        } else {
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
