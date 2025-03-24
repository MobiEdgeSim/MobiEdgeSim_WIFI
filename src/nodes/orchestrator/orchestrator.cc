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
// Emulation debug
#include <iostream>

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

    // initial the update time interval
    updateMsg = new cMessage("updateMecHost");
    scheduleAt(simTime() + updateInterval, updateMsg);

    //first update
    updateMecHost();

}

void Orchestrator::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage() && strcmp(msg->getName(), "updateMecHost") == 0) {
        updateMecHost();
        scheduleAt(simTime() + updateInterval, updateMsg);
    }
    else {
        // to do for further message
        delete msg;
    }
}

void Orchestrator::updateMecHost()
{
    EV << "Orchestrator::updateMecHost " << endl;
    std::vector<cModule*> newMecHosts;
    // get the root module
    cModule *systemModule = getSimulation()->getSystemModule();

    // 遍历系统模块下的所有子模块，查找名称中包含 "mecHost" 的模块
    for (cModule::SubmoduleIterator it(systemModule); !it.end(); ++it) {
        cModule *mod = *it;
        if (strstr(mod->getName(), "mecHost") != nullptr) {
            newMecHosts.push_back(mod);
        }
    }

    bool hasChanged = false;
    // 添加新发现的 mecHost 模块
    for (auto newHost : newMecHosts) {
        if (std::find(mecHosts.begin(), mecHosts.end(), newHost) == mecHosts.end()) {
            mecHosts.push_back(newHost);
            hasChanged = true;
        }
    }
    // 删除已不再存在的 mecHost 模块
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

    // 调试信息：输出更新后的 mecHost 列表
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
    EV << "Updated mecHostList parameter: " << newMecHostList << "\n";
}

std::vector<std::string> Orchestrator::getMechostNames(inet::Coord currentCoord)
{
    std::vector<std::string> hostList;
    // 获取 WiFi 信号范围参数（单位与坐标单位相同，例如米）
    double wIFIDistance = par("wIFIDistance").doubleValue();

    // 获取系统根模块
    cModule *systemModule = getSimulation()->getSystemModule();
    for (cModule::SubmoduleIterator it(systemModule); !it.end(); ++it) {
        cModule *mod = *it;
        bool matches = false;
        // 匹配模块基名称为 "mecHost"
        if (strcmp(mod->getName(), "mecHost") == 0)
            matches = true;
        // 或者全名后缀为 "mechost"
        std::string fullName = mod->getFullName();
        if (fullName.size() >= 8 && fullName.substr(fullName.size() - 8) == "mechost")
            matches = true;

        if (matches) {
            inet::Coord pos;
            if (mod->isVector()) {
                // 对于向量模块，从子模块 "mobility" 获取位置
                cModule *mobilityModule = mod->getSubmodule("mobility");
                if (mobilityModule) {
                    veins::VeinsInetMobility *mobility = check_and_cast<veins::VeinsInetMobility*>(mobilityModule);
                    inet::Coord lastPosition = mobility->getCurrentPosition();
                    pos.x = std::round(lastPosition.x * 1000.0) / 1000.0;
                    pos.y = std::round(lastPosition.y * 1000.0) / 1000.0;
                    pos.z = std::round(lastPosition.z * 1000.0) / 1000.0;
                }
                else {
                    pos.x = pos.y = pos.z = 0;
                }
            }
            else {
                // 对于非向量模块，从参数中读取 latitude 和 longitude
                pos.x = mod->par("latitude").doubleValue();
                pos.y = mod->par("longitude").doubleValue();
                pos.z = 0;
            }

            // 计算当前 mecHost 与 UE 之间的欧氏距离
            double dx = pos.x - currentCoord.x;
            double dy = pos.y - currentCoord.y;
            double dz = pos.z - currentCoord.z;
            double distance = std::sqrt(dx * dx + dy * dy + dz * dz);

            // 仅当距离在 WiFi 范围内时，将该 mecHost 信息加入列表中
            if (distance <= wIFIDistance) {

                EV << mod->getFullName() << ": (" << pos.x << ", " << pos.y << ", " << pos.z << ")"
                   << ", distance: " << distance << "\n";
                hostList.push_back(mod->getFullName());

            }
        }
    }
    return hostList;
}

void Orchestrator::finish()
{
    cancelAndDelete(updateMsg);
}

}
