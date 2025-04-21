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
#include "inet/applications/base/ApplicationPacket_m.h"
#include "TimestampedPacket_m.h"
#include <omnetpp.h>
using namespace omnetpp;

namespace MobiEdgeSim {

Define_Module(UdpUeApp);

void UdpUeApp::initialize(int stage)
{
    //std::cout<< "UdpUeApp::initialize!!!" << std::endl;
    inet::UdpBasicApp::initialize(stage);
    if (stage == inet::INITSTAGE_LOCAL) {
        updateDestInterval = par("updateDestInterval").doubleValue();
        updateDestMsg = new cMessage("updateDestAddresses");
        scheduleAt(simTime() + 1, updateDestMsg);

        //rttTimeout = par("rttTimeout");
    }
}
void UdpUeApp::handleMessage(cMessage *msg)
{
    if (strcmp(msg->getName(), "updateDestAddresses") == 0) {
        updateDestAddresses();
        scheduleAt(simTime() + updateDestInterval, updateDestMsg);
        //std::cout << "UdpUeApp::handleMessage -- (scheduleAt) updateDestAddresses"<<updateDestInterval << std::endl;
        return;
    }//reset the old rtt
//    else if (strstr(msg->getName(), "resetRtt-") == msg->getName()) {
//
//        std::string hostName = msg->par("hostName").stringValue();
//
//        rttMap[hostName] = 1e6;
//        EV << "UdpUeApp::handleMessage => set rttMap[" << hostName << "] to 1e6.\n";
//
//        auto it = rttResetTimerMap.find(hostName);
//        if (it != rttResetTimerMap.end()) {
//            if (it->second == msg) {
//                rttResetTimerMap.erase(it);
//            }
//        }
//        delete msg;
//        return;
//    }
    inet::UdpBasicApp::handleMessage(msg);
}
void UdpUeApp::updateDestAddresses()
{
    EV << "UdpUeApp::updateDestAddresses invoked" << "\n";

    MobiEdgeSim::Orchestrator *orch = check_and_cast<MobiEdgeSim::Orchestrator*>(getSimulation()->getSystemModule()->getSubmodule("orchestrator"));

    if (!orch) {
        EV_WARN << "Orchestrator module not found, cannot update destination addresses.\n";
        return;
    }
    inet::Coord currentCoord;
    cModule *mobilityModule = getParentModule()->getSubmodule("mobility");
    if (mobilityModule) {
        auto mobility = check_and_cast<veins::VeinsInetMobility*>(mobilityModule);
        currentCoord = mobility->getCurrentPosition();
    }
    else {
        currentCoord.x = par("latitude").doubleValue();
        currentCoord.y = par("longitude").doubleValue();
        currentCoord.z = 0;
    }
    EV << "current latitude" << currentCoord.x << "longitude" << currentCoord.y << endl;

    std::vector<std::string> hostNames = orch->getMechostNames(currentCoord);

    destAddresses.clear();
    for (const auto &hostName : hostNames) {

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
        EV << "Triggering send after updating destination addresses." << endl;

        if (selfMsg->isScheduled())
            cancelEvent(selfMsg);
        selfMsg->setKind(SEND);
        processSend();
    }

}
void UdpUeApp::processPacket(inet::Packet *packet)
{
    //std::cout<< "UdpUeApp::processPacket received packet:" << packet->getName()<< std::endl;
    EV << "UdpUeApp::processPacket received packet: " << packet->getName() << endl;
    const auto &appPkt = packet->peekAtFront<TimestampedPacket>();

    simtime_t delay = simTime() - appPkt->getSendTime();
    EV << "RTT delay = " << delay << "s" << endl;

    auto srcAddrTag = packet->findTag<inet::L3AddressInd>();
    if (srcAddrTag) {
        auto addr = srcAddrTag->getSrcAddress();
        auto *host = inet::L3AddressResolver().findHostWithAddress(addr);
        std::string hostName = host ? host->getFullName() : addr.str();

        RttEntry &e = rttMap[hostName];
        e.rtt        = delay;
        e.lastUpdate = simTime();         // <‑‑ NEW
        EV << "Saved RTT = " << delay << "s at t=" << e.lastUpdate << endl;
//
//
//        rttMap[hostName] = delay;
//        EV << "Saved RTT to rttMap[" << hostName << "] = " << delay << "s" << endl;
        //scheduleRttReset(hostName);
    }

    UdpBasicApp::processPacket(packet);
}

//void UdpUeApp::scheduleRttReset(const std::string &hostName)
//{
//    auto it = rttResetTimerMap.find(hostName);
//    if (it != rttResetTimerMap.end()) {
//        cMessage *oldTimer = it->second;
//        if (oldTimer->isScheduled())
//            cancelEvent(oldTimer);
//        delete oldTimer;
//        rttResetTimerMap.erase(it);
//    }
//    //cMessage *timerMsg = new cMessage(("resetRtt-" + hostName).c_str());
//    timerMsg->addPar("hostName") = hostName.c_str();
//
//    rttResetTimerMap[hostName] = timerMsg;
//
//    EV << "Scheduled RTT reset for host [" << hostName << "] at t=" << simTime() + rttTimeout << "\n";
//    //std::cout << "Scheduled RTT reset for host [" << hostName << "] at t=" << simTime() + rttTimeout << "\n";
//    scheduleAt(simTime() + rttTimeout, timerMsg);
//}

void UdpUeApp::finish()
{
    cancelAndDelete(updateDestMsg);

//    for (auto &kv : rttResetTimerMap) {
//        cMessage *timer = kv.second;
//        if (timer->isScheduled())
//            cancelEvent(timer);
//        delete timer;
//    }
//    rttResetTimerMap.clear();

    inet::UdpBasicApp::finish();
}

void UdpUeApp::sendPacket()
{
    for (auto &addr : destAddresses) {
        auto packet = new inet::Packet("UdpBasicAppData");
        const auto &appPkt = inet::makeShared<TimestampedPacket>();
        appPkt->setSendTime(simTime());
        appPkt->setSequenceNumber(numSent);
        appPkt->setPayloadSize(par("messageLength").intValue());
        appPkt->setChunkLength(inet::B(par("messageLength")));

        packet->insertAtBack(appPkt);
        emit(inet::packetSentSignal, packet);
        socket.sendTo(packet, addr, destPort);
        numSent++;
    }

}

} // namespace MobiEdgeSim

