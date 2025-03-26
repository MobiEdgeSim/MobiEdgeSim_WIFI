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

#include "inet/common/TimeTag_m.h"
#include "inet/common/packet/Packet.h"
#include "UdpUeApp.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "nodes/orchestrator/orchestrator.h"
#include "veins_inet/VeinsInetMobility.h"

namespace MobiEdgeSim {

Define_Module(UdpUeApp);
void UdpUeApp::initialize(int stage)
{
    inet::UdpBasicApp::initialize(stage);
    if (stage == inet::INITSTAGE_LOCAL) {
        // 创建更新目的地址的自消息，间隔从参数读取
        updateDestInterval = par("updateDestInterval").doubleValue();
        updateDestMsg = new cMessage("updateDestAddresses");
        scheduleAt(simTime() +1+ updateDestInterval, updateDestMsg);//add 1s as start time
    }
}
void UdpUeApp::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {
        if (strcmp(msg->getName(), "updateDestAddresses") == 0) {
            updateDestAddresses();
            // 重新安排下一个更新
            scheduleAt(simTime() + updateDestInterval, updateDestMsg);
            return;
        }
    }
    // 非自消息则调用父类处理
    inet::UdpBasicApp::handleMessage(msg);
}
void UdpUeApp::updateDestAddresses()
{
    EV << "UdpUeApp::updateDestAddresses invoked" << "\n";
    // 获取 orchestrator 模块
    // 假设 orchestrator 的 NED 模块名称为 "orchestrator" 位于网络层的某处，例如 "network.orchestrator"
    // 你可以根据实际情况修改路径
    MobiEdgeSim::Orchestrator *orch = check_and_cast<MobiEdgeSim::Orchestrator*>(getSimulation()->getSystemModule()->getSubmodule("orchestrator"));

    if (!orch) {
        EV_WARN << "Orchestrator module not found, cannot update destination addresses.\n";
        return;
    }
    // 这里构造一个当前位置，可以从自身的 mobility 模块获取
    inet::Coord currentCoord;
    cModule *mobilityModule = getParentModule()->getSubmodule("mobility");
    if (mobilityModule) {
        auto mobility = check_and_cast<veins::VeinsInetMobility*>(mobilityModule);
        currentCoord = mobility->getCurrentPosition();
    } else {
        currentCoord.x = par("latitude").doubleValue();
        currentCoord.y = par("longitude").doubleValue();
        currentCoord.z = 0;
    }
    EV<<"current latitude"<<currentCoord.x<<"longitude"<<currentCoord.y<<endl;
    // 调用 orchestrator 的接口获取范围内的 MEC host 名称
    std::vector<std::string> hostNames = orch->getMechostNames(currentCoord);

    // 更新本模块的 destAddresses
    // 清空原来的目标地址数组
    destAddresses.clear();
    for (const auto &hostName : hostNames) {
        // 尝试解析 hostName 为 L3Address
        inet::L3Address addr;
        if (inet::L3AddressResolver().tryResolve(hostName.c_str(), addr))
            destAddresses.push_back(addr);
        else
            EV << "Cannot resolve host name: " << hostName << "\n";
    }
    EV << "Updated destAddresses: ";
    for (auto &addr : destAddresses) {
        EV << addr << " ";
    }
    EV << "\n";

    if (!destAddresses.empty()) {
        // 如果更新后存在目的地址，则触发一次发送操作
        EV << "Triggering send after updating destination addresses." << endl;
        // 可以调用 processSend() 或者安排 selfMsg

        if (selfMsg->isScheduled())
            cancelEvent(selfMsg);
        selfMsg->setKind(SEND);
        processSend();
    }

}
void UdpUeApp::processPacket(inet::Packet *packet)
{
    // 提取创建时间标签以计算 RTT
    auto creationTimeTag = packet->findTag<inet::CreationTimeTag>();
    if (creationTimeTag) {
        omnetpp::simtime_t rtt = omnetpp::simTime() - creationTimeTag->getCreationTime();

        // 提取来源地址标签获得远程 IP
        auto addressTag = packet->findTag<inet::L3AddressInd>();
        std::string remoteIp;
        if (addressTag)
            remoteIp = addressTag->getSrcAddress().str();
        else
            remoteIp = "unknown";
        inet::L3Address addr(remoteIp.c_str());
        cModule *hostModule = inet::L3AddressResolver().findHostWithAddress(addr);
        std::string senderFullName = hostModule ? hostModule->getFullName() : "unknown";

        EV << "RTT from remote " << senderFullName << " = " << rtt << " s" << omnetpp::endl;
        // 保存 RTT 信息到 map 中（更新最新值）
        rttMap[senderFullName] = rtt;
    }
    // 调用父类逻辑（统计、删除包等）
    inet::UdpBasicApp::processPacket(packet);
}
void UdpUeApp::finish()
{
    cancelAndDelete(updateDestMsg);
    inet::UdpBasicApp::finish();
}

} // namespace MobiEdgeSim

