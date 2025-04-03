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

namespace MobiEdgeSim {

class UdpUeApp: public inet::UdpBasicApp {
protected:
    // 用于保存每个收到回复的 RTT 延迟
    std::map<std::string, omnetpp::simtime_t> rttMap;
    inet::cMessage *updateDestMsg = nullptr;
        // 定时更新间隔（单位 s），可以从参数中读取
    double updateDestInterval;

    virtual void initialize(int stage) override;
    virtual void handleMessage(inet::cMessage *msg) override;
    // 重载处理数据包的方法
    virtual void processPacket(inet::Packet *packet) override;

    void updateDestAddresses();
    void sendPacket();
public:
    // 供外部查询延迟数据的接口
    const std::map<std::string, omnetpp::simtime_t>& getRttMap() const { return rttMap; }
    virtual void finish() override;
};

}

#endif
