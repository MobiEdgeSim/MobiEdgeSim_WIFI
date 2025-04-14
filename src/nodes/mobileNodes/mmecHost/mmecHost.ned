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

package mobiedgesim.nodes.mobileNodes.mmecHost;


import mobiedgesim.nodes.SimpleNodeBase;
import inet.applications.udpapp.UdpEchoApp;
import mobiedgesim.apps.UdpHostSink;
import mobiedgesim.nodes.mecHost.MecHost;
//
// TODO auto-generated module
//
module MmecHost extends MecHost
{
    parameters:
        @display("i=device/server2;bgb=1000,679");
        //@class(MobiEdgeSim::MmecHost);
        mobility.typename = default("VeinsInetMobility");
        updatePositionInterval = 1s;
        maxRam  = default(0.5GB);
        maxDisk = default(50GB);
        maxCpuSpeed = default(1500); 
        requestProcessTime = 1s;
//        double maxRam @unit("B") = default(1GB);
//        double maxDisk @unit("B") = default(50GB);
//        double maxCpuSpeed = default(1500); 	 // CPU's clock speed in Million Instruction per Seconds (MIPS)
//        
//        double updatePositionInterval @unit(s) = default(1s);
//        string requiredServices = default("service1");
//        
//        double requestProcessTime @unit(s) = default(1s);
//        double resource_used_pro = default(0.8);
//        
//    submodules:
//	    udpHostSink : UdpHostSink{
//	         parameters:
//
//                @display("p=675,80");
//	    }
//	 connections:
//	     udpHostSink.socketOut --> at.in++;
//         udpHostSink.socketIn <-- at.out++;
         

}
