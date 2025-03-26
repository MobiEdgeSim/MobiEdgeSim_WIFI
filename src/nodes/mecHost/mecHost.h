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

#ifndef __MOBIEDGESIM_MECHOST_H_
#define __MOBIEDGESIM_MECHOST_H_

#include <omnetpp.h>
#include <vector>
#include <string>

namespace MobiEdgeSim {

using namespace omnetpp;

struct MecHostInfo {//mechost Infrastructure
    std::string name;
    double availableRam;
    double availableDisk;
    double availableCpu;
    double latitude;
    double longitude;
    double ram;
    double disk;
    double cpu;
    double latency;
};

class MecHost : public cSimpleModule
{
  protected:
    MecHostInfo currentInfo;
    cMessage *updatePositionMsg = nullptr; // 用于定时更新位置的自消息
    double updatePositionInterval; // 更新位置的时间间隔

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void updatePosition(); // 更新当前位置的函数

    double maxRam;
    double maxDisk;
    double maxCPU;
    //allocated resources
    double allocatedRam;
    double allocatedDisk;
    double allocatedCPU;

  public:
      const MecHostInfo& getMecHostInfo() const;
      void updateResources(double allocatedRam, double allocatedDisk, double allocatedCPU);
      void updateStatus(const MecHostInfo &newInfo);
};


}
#endif
