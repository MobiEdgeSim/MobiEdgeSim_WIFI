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
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/transportlayer/common/L4PortTag_m.h"


namespace MobiEdgeSim {

Define_Module(UdpHostSink);

//void UdpHostSink::socketDataArrived(inet::UdpSocket *socket, inet::Packet *pk)
//{
//    auto remoteAddress = pk->getTag<inet::L3AddressInd>()->getSrcAddress();
//    int srcPort = pk->getTag<inet::L4PortInd>()->getSrcPort();
//
//    EV << "UdpHostSink received packet: " << pk->getName() << " from "
//       << remoteAddress << ":" << srcPort << inet::endl;
//
//    auto echoPacket = pk->dup();
//
//    std::string originalName = pk->getName();
//    std::string echoPrefix = "Echo-";
////    if (originalName.find(echoPrefix) != 0) {
////        echoPacket->setName((echoPrefix + originalName).c_str());
////    } else {
////        echoPacket->setName(("EchoReply-" + std::to_string(numEchoed)).c_str());
////    }
//    //echoPacket->setName(("Echo-" + std::string(pk->getName())).c_str());
//
//    // 发回
//    socket->sendTo(echoPacket, remoteAddress, srcPort);
//    delete pk;
//
//    numEchoed++;
//}



}
