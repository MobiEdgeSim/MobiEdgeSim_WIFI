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

network hundredStatic
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

mecHost11: MecHost { @display("p=0,444"); }
mecHost12: MecHost { @display("p=444,444"); }
mecHost13: MecHost { @display("p=889,444"); }
mecHost14: MecHost { @display("p=1333,444"); }
mecHost15: MecHost { @display("p=1778,444"); }
mecHost16: MecHost { @display("p=2222,444"); }
mecHost17: MecHost { @display("p=2667,444"); }
mecHost18: MecHost { @display("p=3111,444"); }
mecHost19: MecHost { @display("p=3556,444"); }
mecHost20: MecHost { @display("p=4000,444"); }

mecHost21: MecHost { @display("p=0,889"); }
mecHost22: MecHost { @display("p=444,889"); }
mecHost23: MecHost { @display("p=889,889"); }
mecHost24: MecHost { @display("p=1333,889"); }
mecHost25: MecHost { @display("p=1778,889"); }
mecHost26: MecHost { @display("p=2222,889"); }
mecHost27: MecHost { @display("p=2667,889"); }
mecHost28: MecHost { @display("p=3111,889"); }
mecHost29: MecHost { @display("p=3556,889"); }
mecHost30: MecHost { @display("p=4000,889"); }

mecHost31: MecHost { @display("p=0,1333"); }
mecHost32: MecHost { @display("p=444,1333"); }
mecHost33: MecHost { @display("p=889,1333"); }
mecHost34: MecHost { @display("p=1333,1333"); }
mecHost35: MecHost { @display("p=1778,1333"); }
mecHost36: MecHost { @display("p=2222,1333"); }
mecHost37: MecHost { @display("p=2667,1333"); }
mecHost38: MecHost { @display("p=3111,1333"); }
mecHost39: MecHost { @display("p=3556,1333"); }
mecHost40: MecHost { @display("p=4000,1333"); }

mecHost41: MecHost { @display("p=0,1778"); }
mecHost42: MecHost { @display("p=444,1778"); }
mecHost43: MecHost { @display("p=889,1778"); }
mecHost44: MecHost { @display("p=1333,1778"); }
mecHost45: MecHost { @display("p=1778,1778"); }
mecHost46: MecHost { @display("p=2222,1778"); }
mecHost47: MecHost { @display("p=2667,1778"); }
mecHost48: MecHost { @display("p=3111,1778"); }
mecHost49: MecHost { @display("p=3556,1778"); }
mecHost50: MecHost { @display("p=4000,1778"); }

mecHost51: MecHost { @display("p=0,2222"); }
mecHost52: MecHost { @display("p=444,2222"); }
mecHost53: MecHost { @display("p=889,2222"); }
mecHost54: MecHost { @display("p=1333,2222"); }
mecHost55: MecHost { @display("p=1778,2222"); }
mecHost56: MecHost { @display("p=2222,2222"); }
mecHost57: MecHost { @display("p=2667,2222"); }
mecHost58: MecHost { @display("p=3111,2222"); }
mecHost59: MecHost { @display("p=3556,2222"); }
mecHost60: MecHost { @display("p=4000,2222"); }

mecHost61: MecHost { @display("p=0,2667"); }
mecHost62: MecHost { @display("p=444,2667"); }
mecHost63: MecHost { @display("p=889,2667"); }
mecHost64: MecHost { @display("p=1333,2667"); }
mecHost65: MecHost { @display("p=1778,2667"); }
mecHost66: MecHost { @display("p=2222,2667"); }
mecHost67: MecHost { @display("p=2667,2667"); }
mecHost68: MecHost { @display("p=3111,2667"); }
mecHost69: MecHost { @display("p=3556,2667"); }
mecHost70: MecHost { @display("p=4000,2667"); }

mecHost71: MecHost { @display("p=0,3111"); }
mecHost72: MecHost { @display("p=444,3111"); }
mecHost73: MecHost { @display("p=889,3111"); }
mecHost74: MecHost { @display("p=1333,3111"); }
mecHost75: MecHost { @display("p=1778,3111"); }
mecHost76: MecHost { @display("p=2222,3111"); }
mecHost77: MecHost { @display("p=2667,3111"); }
mecHost78: MecHost { @display("p=3111,3111"); }
mecHost79: MecHost { @display("p=3556,3111"); }
mecHost80: MecHost { @display("p=4000,3111"); }

mecHost81: MecHost { @display("p=0,3556"); }
mecHost82: MecHost { @display("p=444,3556"); }
mecHost83: MecHost { @display("p=889,3556"); }
mecHost84: MecHost { @display("p=1333,3556"); }
mecHost85: MecHost { @display("p=1778,3556"); }
mecHost86: MecHost { @display("p=2222,3556"); }
mecHost87: MecHost { @display("p=2667,3556"); }
mecHost88: MecHost { @display("p=3111,3556"); }
mecHost89: MecHost { @display("p=3556,3556"); }
mecHost90: MecHost { @display("p=4000,3556"); }

mecHost91: MecHost { @display("p=0,4000"); }
mecHost92: MecHost { @display("p=444,4000"); }
mecHost93: MecHost { @display("p=889,4000"); }
mecHost94: MecHost { @display("p=1333,4000"); }
mecHost95: MecHost { @display("p=1778,4000"); }
mecHost96: MecHost { @display("p=2222,4000"); }
mecHost97: MecHost { @display("p=2667,4000"); }
mecHost98: MecHost { @display("p=3111,4000"); }
mecHost99: MecHost { @display("p=3556,4000"); }
mecHost100: MecHost { @display("p=4000,4000"); }

}
