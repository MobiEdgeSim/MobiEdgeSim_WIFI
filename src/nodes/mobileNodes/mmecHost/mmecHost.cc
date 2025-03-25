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

#include "mmecHost.h"
#include "veins_inet/VeinsInetMobility.h"

namespace MobiEdgeSim {


Define_Module(MmecHost);

void MmecHost::initialize() {
    currentInfo.name = getFullName();

    //resource
    double maxRam = par("maxRam").doubleValue();
    double maxDisk = par("maxDisk").doubleValue();
    double maxCpu = par("maxCpuSpeed").doubleValue();

    double occupancy = uniform(0.2, 0.8);
    currentInfo.availableRam = maxRam * (1 - occupancy);
    currentInfo.availableDisk = maxDisk * (1 - occupancy);
    currentInfo.availableCpu = maxCpu * (1 - occupancy);

    currentInfo.ram = maxRam;
    currentInfo.disk = maxDisk;
    currentInfo.cpu = maxCpu;

    cModule *mobilityModule = getSubmodule("mobility");
    if (mobilityModule) {
        auto mobility = check_and_cast<veins::VeinsInetMobility*>(mobilityModule);
        const inet::Coord &pos = mobility->getCurrentPosition();
        currentInfo.latitude = pos.x;
        currentInfo.longitude = pos.y;
    }else{

    }

    EV << "Mobile mecHost " << getFullName() << ", RAM: " << currentInfo.availableRam<< ", Disk: " << currentInfo.availableDisk << ", CPU: "<< currentInfo.availableCpu << "\n";

}


}
