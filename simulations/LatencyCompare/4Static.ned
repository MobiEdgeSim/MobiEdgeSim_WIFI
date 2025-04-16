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

package mobiedgesim.simulations.LatencyCompare;

import mobiedgesim.nodes.mobileNodes.mmecHost.MmecHost;
import mobiedgesim.nodes.mobileNodes.ue.Ue;
import mobiedgesim.nodes.mecHost.MecHost;
import mobiedgesim.nodes.orchestrator.Orchestrator;
import inet.visualizer.common.IntegratedMultiVisualizer;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import org.car2x.veins.subprojects.veins_inet.VeinsInetManager;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;

network FStatic
{
    parameters:
        **.mgmt.numChannels = 20;
        double playgroundSizeX @unit(m); // x size of the area the nodes are in (in meters)
        double playgroundSizeY @unit(m); // y size of the area the nodes are in (in meters)
        double playgroundSizeZ @unit(m); // z size of the area the nodes are in (in meters)
        @display("bgb=100,100");
    submodules:

        ue0: Ue {
            @display("p=50,50");
        }

        visualizer: IntegratedMultiVisualizer {
            @display("p=4,1");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=4,2");
        }
        veinsManager: VeinsInetManager {
            @display("p=4,3;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=4,4");
            //config = xml("<config><interface hosts='*' address='145.236.x.x' netmask='255.255.0.0'/></config>");
        }
        orchestrator: Orchestrator {
            @display("p=956,44");
        }

        mecHost1: MecHost {
            @display("p=0,0");
        }
//        mecHost2: MecHost {
//            @display("p=50,0");
//        }
        mecHost3: MecHost {
            @display("p=100,0");
        }
//        mecHost4: MecHost {
//            @display("p=0,50");
//        }
//        mecHost5: MecHost {
//            @display("p=50,50");
//        }
//        mecHost6: MecHost {
//            @display("p=100,50");
//        }
        mecHost7: MecHost {
            @display("p=0,100");
        }
//        mecHost8: MecHost {
//            @display("p=50,100");
//        }
        mecHost9: MecHost {
            @display("p=100,100");
        }


}
