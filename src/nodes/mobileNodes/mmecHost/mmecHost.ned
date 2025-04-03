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
//
// TODO auto-generated module
//
module MmecHost extends SimpleNodeBase
{
    parameters:
        @display("i=device/server2;bgb=1000,679");
        @class(MobiEdgeSim::MmecHost);
        
        double maxRam @unit("B") = default(2GB);
        double maxDisk @unit("B") = default(100GB);
        double maxCpuSpeed = default(3000); 	 // CPU's clock speed in Million Instruction per Seconds (MIPS)
    submodules:
	    udpHostSink : UdpHostSink{
	         parameters:

                @display("p=675,80");
	    }
	 connections:
	     udpHostSink.socketOut --> at.in++;
         udpHostSink.socketIn <-- at.out++;
}
