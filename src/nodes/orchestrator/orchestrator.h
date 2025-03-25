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

#ifndef __MOBIEDGESIM_ORCHESTRATOR_H_
#define __MOBIEDGESIM_ORCHESTRATOR_H_

#include <omnetpp.h>
#include <vector>
#include <string>
#include "inet/common/geometry/common/Coord.h"
#include "nodes/mecHost/mecHost.h"

namespace MobiEdgeSim {
using namespace omnetpp;

class Orchestrator : public inet::cSimpleModule
{
public:
    struct AppDescriptorInfo {//ue service require
        std::string name;
        std::string ueIpAddress;
        double ram;
        double disk;
        double cpu;
        double latitude;
        double longitude;
    };


    std::vector<MecHostInfo> mecHostInfos;

  protected:

    inet::cMessage *updateMsg = nullptr;
    inet::simtime_t updateInterval;//Update period (unit: seconds)
    std::vector<cModule*> mecHosts;

    // update mecHostList
    void updateMecHostListParam();
    void updateMecHost();
  public:
    // Returns a list of all mecHost information that meet the conditions based on the current location information
    std::vector<std::string> getMechostNames(inet::Coord currentCoord);


  protected:
    //service placement
    inet::cMessage *spMsg = nullptr;
    std::vector<cModule*> ues;
    inet::simtime_t spInterval;
    void servicePlacement();
    cModule* findBestMecHostForUE(cModule* ue);


    // build service request
    AppDescriptorInfo buildAppDescriptor(cModule* ue);
    // build mecHost infor
    std::vector<MecHostInfo> buildMecHostInfos();

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(inet::cMessage *msg) override;
    virtual void finish() override;

    double requestRam;
    double requestDisk;
    double requestCpu;
    std::string algorithmName;

};

}
#endif
