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
#include "UdpHostSink.h"

namespace MobiEdgeSim {

Define_Module(UdpHostSink);

//void socketDataArrived(inet::UdpSocket *socket, inet::Packet *packet)
//{
//    // (1) 构造要回的包, 附带 CreationTimeTag 或者原包复制
//    auto reply = packet->dup();
//    reply->setName("EchoReply");
//
//    // (2) 取出源地址和端口 (适合老版本: UdpControlInfo 或 L3AddressTag)
//    auto ctrl = packet->getTag<inet::UdpControlInfo>();
//    if (!ctrl) {
//        EV_WARN << "No UdpControlInfo, cannot echo" << endl;
//        emit(packetReceivedSignal, packet);
//        delete packet;
//        delete reply;
//        numReceived++;
//        return;
//    }
//    inet::L3Address srcAddr = ctrl->getSrcAddr();
//    int srcPort = ctrl->getSrcPort();
//
//    // (3) 回包
//    socket->sendTo(reply, srcAddr, srcPort);
//
//    // (4) 父类默认会 delete packet; 这里也可手动统计
//    emit(packetReceivedSignal, packet);
//    delete packet;
//    numReceived++;
//}


}
