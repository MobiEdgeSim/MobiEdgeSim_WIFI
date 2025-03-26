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

package mobiedgesim.nodes.mecHost;
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
import inet.applications.udpapp.UdpEchoApp;
//
// TODO auto-generated module
//
module MecHost extends SimpleNodeBase
{
    
    parameters:
		@class(MobiEdgeSim::MecHost); 
		@display("i=device/server2;bgb=1000,679");
        //settings for mobile 
        mobility.typename = default("StationaryMobility");//# Mobility
        
        
        double maxRam @unit("B") = default(2GB);
        double maxDisk @unit("B") = default(100GB);
        double maxCpuSpeed = default(3000); 	 // CPU's clock speed in Million Instruction per Seconds (MIPS)
        
        double updatePositionInterval @unit(s) = default(1s);
        string requiredServices = default("service1");
        
	submodules:
	    udpEchoApp : UdpEchoApp{
	         parameters:

                @display("p=675,80");
	    }
	 connections:
	     udpEchoApp.socketOut --> at.in++;
         udpEchoApp.socketIn <-- at.out++;
            
    
}
