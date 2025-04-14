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

package mobiedgesim.simulations.SimpleNet.Four_Km_GridMap;


import mobiedgesim.nodes.mobileNodes.mmecHost.MmecHost;
import mobiedgesim.nodes.mobileNodes.ue.Ue;
import mobiedgesim.nodes.mecHost.MecHost;
import mobiedgesim.nodes.orchestrator.Orchestrator;
import inet.visualizer.common.IntegratedMultiVisualizer;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import org.car2x.veins.subprojects.veins_inet.VeinsInetManager;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;

network fiftyStatic
{
    parameters:
        **.mgmt.numChannels = 20;
        double playgroundSizeX @unit(m); // x size of the area the nodes are in (in meters)
        double playgroundSizeY @unit(m); // y size of the area the nodes are in (in meters)
        double playgroundSizeZ @unit(m); // z size of the area the nodes are in (in meters)
        @display("bgb=4000,4000");
    submodules:
        mmecHost[0]: MmecHost {
            @display("p=277,60");
        }
        ue[0]: Ue {
            @display("p=368,60");
        }
        
        visualizer: IntegratedMultiVisualizer {
            @display("p=31,36");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=31,105");
        }
        veinsManager: VeinsInetManager {
            @display("p=31,236;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=31,175");
            //config = xml("<config><interface hosts='*' address='145.236.x.x' netmask='255.255.0.0'/></config>");
        }
        orchestrator: Orchestrator {
            @display("p=669,49");
        }

		mecHost1: MecHost { @display("p=0,0"); }
mecHost2: MecHost { @display("p=444,0"); }
mecHost3: MecHost { @display("p=889,0"); }
mecHost4: MecHost { @display("p=1333,0"); }
mecHost5: MecHost { @display("p=1778,0"); }
mecHost6: MecHost { @display("p=2222,0"); }
mecHost7: MecHost { @display("p=2667,0"); }
mecHost8: MecHost { @display("p=3111,0"); }
mecHost9: MecHost { @display("p=3556,0"); }
mecHost10: MecHost { @display("p=4000,0"); }

mecHost11: MecHost { @display("p=0,1000"); }
mecHost12: MecHost { @display("p=444,1000"); }
mecHost13: MecHost { @display("p=889,1000"); }
mecHost14: MecHost { @display("p=1333,1000"); }
mecHost15: MecHost { @display("p=1778,1000"); }
mecHost16: MecHost { @display("p=2222,1000"); }
mecHost17: MecHost { @display("p=2667,1000"); }
mecHost18: MecHost { @display("p=3111,1000"); }
mecHost19: MecHost { @display("p=3556,1000"); }
mecHost20: MecHost { @display("p=4000,1000"); }

mecHost21: MecHost { @display("p=0,2000"); }
mecHost22: MecHost { @display("p=444,2000"); }
mecHost23: MecHost { @display("p=889,2000"); }
mecHost24: MecHost { @display("p=1333,2000"); }
mecHost25: MecHost { @display("p=1778,2000"); }
mecHost26: MecHost { @display("p=2222,2000"); }
mecHost27: MecHost { @display("p=2667,2000"); }
mecHost28: MecHost { @display("p=3111,2000"); }
mecHost29: MecHost { @display("p=3556,2000"); }
mecHost30: MecHost { @display("p=4000,2000"); }

mecHost31: MecHost { @display("p=0,3000"); }
mecHost32: MecHost { @display("p=444,3000"); }
mecHost33: MecHost { @display("p=889,3000"); }
mecHost34: MecHost { @display("p=1333,3000"); }
mecHost35: MecHost { @display("p=1778,3000"); }
mecHost36: MecHost { @display("p=2222,3000"); }
mecHost37: MecHost { @display("p=2667,3000"); }
mecHost38: MecHost { @display("p=3111,3000"); }
mecHost39: MecHost { @display("p=3556,3000"); }
mecHost40: MecHost { @display("p=4000,3000"); }

mecHost41: MecHost { @display("p=0,4000"); }
mecHost42: MecHost { @display("p=444,4000"); }
mecHost43: MecHost { @display("p=889,4000"); }
mecHost44: MecHost { @display("p=1333,4000"); }
mecHost45: MecHost { @display("p=1778,4000"); }
mecHost46: MecHost { @display("p=2222,4000"); }
mecHost47: MecHost { @display("p=2667,4000"); }
mecHost48: MecHost { @display("p=3111,4000"); }
mecHost49: MecHost { @display("p=3556,4000"); }
mecHost50: MecHost { @display("p=4000,4000"); }
		
}
