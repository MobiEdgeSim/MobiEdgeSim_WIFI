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

package mobiedgesim.nodes.orchestrator;

//
// TODO auto-generated module
//
simple Orchestrator
{
    parameters:
        @display("i=device/mainframe;bgb=1006.50995,692.76");
        @class(MobiEdgeSim::Orchestrator); 
        
        double updateInterval @unit(s) = default(1s);    // time slot to update the mechosts list
        
        double wIFIDistance = default(100);//the distance limitations of wifi signal
        
        string mecHostList = default("")@mutable;
        
        
        double spInterval @unit(s)= default(1s);
        
        //ue request service
//        int minRamRequest = 2;
//        int maxRamRequest = 16;
//        int minDiskRequest = 20;
//        int maxDiskRequest = 160;
//        int minCpuRequest = 10;
//        int maxCpuRequest = 160;
        
        string requiredServices = default("service1");
        string algorithmName = default("RandomAlgorithm");
        
        
        
    	double ramRequest @unit("B") = default(2GB);
    	double diskRequest @unit("B") = default(100GB);
    	double cpuRequest = default(3000);// CPU's clock speed in Million Instruction per Seconds (MIPS)
}
