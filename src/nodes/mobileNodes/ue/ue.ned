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

package mobiedgesim.nodes.mobileNodes.ue;

import inet.applications.contract.IApp;
import inet.common.MessageDispatcher;
import inet.common.packet.recorder.PcapRecorder;
import inet.linklayer.contract.IEthernetInterface;
import inet.linklayer.contract.ILoopbackInterface;
import inet.linklayer.ethernet.contract.IEthernetLayer;
import inet.mobility.contract.IMobility;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.contract.INetworkLayer;
import inet.transportlayer.contract.ISctp;
import inet.transportlayer.contract.ITcp;
import inet.transportlayer.contract.IUdp;
import inet.linklayer.contract.IWirelessInterface;
import simu5g.stack.ICellularNic;
import mobiedgesim.nodes.SimpleNodeBase;
import mobiedgesim.apps.UdpUeApp;
//
// TODO auto-generated module
//
module Ue extends SimpleNodeBase
{
    parameters:
        @display("i=device/pocketpc;bgb=858,659");

    submodules:
        udpUeApp: UdpUeApp {
            parameters:

                @display("p=675,80");
        }
    connections:
        udpUeApp.socketOut --> at.in++;
        udpUeApp.socketIn <-- at.out++;

}
