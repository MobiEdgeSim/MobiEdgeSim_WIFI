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

#ifndef __MOBIEDGESIM_UDPRTTAPP_H_
#define __MOBIEDGESIM_UDPRTTAPP_H_

#include <omnetpp.h>
#include "inet/applications/udpapp/UdpBasicApp.h"
#include "inet/common/TimeTag_m.h"
#include "inet/common/packet/Packet.h"
#include <omnetpp.h>
using namespace omnetpp;
namespace MobiEdgeSim {

struct RttEntry {
    omnetpp::simtime_t rtt;
    omnetpp::simtime_t lastUpdate;
};

class UdpUeApp: public inet::UdpBasicApp {
protected:

    //std::map<std::string, omnetpp::simtime_t> rttMap;
    std::map<std::string, RttEntry> rttMap;
    inet::cMessage *updateDestMsg = nullptr;

    //double rttTimeout;//time to delete the old rtt
    //void scheduleRttReset(const std::string &hostName);
    //std::map<std::string, cMessage*> rttResetTimerMap;

    double updateDestInterval;

    virtual void initialize(int stage) override;
    virtual void handleMessage(inet::cMessage *msg) override;

    virtual void processPacket(inet::Packet *packet) override;

    void updateDestAddresses();
    void sendPacket() override;
public:

    //const std::map<std::string, omnetpp::simtime_t>& getRttMap() const { return rttMap; }
    const std::map<std::string, RttEntry>& getRttMap() const { return rttMap; }
    virtual void finish() override;
};

}

#endif
