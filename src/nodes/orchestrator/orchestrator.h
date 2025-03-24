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

namespace MobiEdgeSim {
using namespace omnetpp;

class Orchestrator : public inet::cSimpleModule
{
  protected:
    // 定时更新消息
    inet::cMessage *updateMsg = nullptr;
    // 更新周期（单位：秒）
    inet::simtime_t updateInterval;
    // 当前检测到的 mecHost 模块列表
    std::vector<cModule*> mecHosts;

    // 更新参数 mecHostList
    void updateMecHostListParam();
    // 查找所有 mecHost 模块，并更新 mecHosts 列表（如有变化则更新参数）
    void updateMecHost();

    // 根据当前位置信息返回所有符合条件的 mecHost 的信息字符串列表
    std::vector<std::string> getMechostNames(inet::Coord currentCoord);

  protected:
    virtual void initialize(int stage) override;
    virtual void handleMessage(inet::cMessage *msg) override;
    virtual void finish() override;
};

}
#endif
