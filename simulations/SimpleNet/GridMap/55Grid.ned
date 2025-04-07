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

package mobiedgesim.simulations.SimpleNet.GridMap;

import mobiedgesim.nodes.mobileNodes.mmecHost.MmecHost;
import mobiedgesim.nodes.mobileNodes.ue.Ue;
import mobiedgesim.nodes.mecHost.MecHost;
import mobiedgesim.nodes.orchestrator.Orchestrator;
import inet.visualizer.common.IntegratedMultiVisualizer;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import org.car2x.veins.subprojects.veins_inet.VeinsInetManager;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;

network TwoKmGrid
{
    parameters:
        **.mgmt.numChannels = 20;
        double playgroundSizeX @unit(m); // x size of the area the nodes are in (in meters)
        double playgroundSizeY @unit(m); // y size of the area the nodes are in (in meters)
        double playgroundSizeZ @unit(m); // z size of the area the nodes are in (in meters)
        @display("bgb=2000,2000");
    submodules:
        mmecHost[0]: MmecHost {
            @display("p=277,60");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        ue[0]: Ue {
            @display("p=368,60");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost1: MecHost {
            @display("p=0,0");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost2: MecHost {
            @display("p=0,500");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost3: MecHost {
            @display("p=0,1000");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost4: MecHost {
            @display("p=0,1500");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost5: MecHost {
            @display("p=0,2000");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost6: MecHost {
            @display("p=500,0");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }        
        mecHost7: MecHost {
            @display("p=500,500");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost8: MecHost {
            @display("p=500,1000");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost9: MecHost {
            @display("p=500,1500");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost10: MecHost {
            @display("p=500,2000");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost11: MecHost {
            @display("p=1000,0");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost12: MecHost {
            @display("p=1000,500");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost13: MecHost {
            @display("p=1000,1000");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost14: MecHost {
            @display("p=1000,1500");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost15: MecHost {
            @display("p=1000,2000");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost16: MecHost {
            @display("p=1500,0");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost17: MecHost {
            @display("p=1500,500");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost18: MecHost {
            @display("p=1500,1000");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost19: MecHost {
            @display("p=1500,1500");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost20: MecHost {
            @display("p=1500,2000");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost21: MecHost {
            @display("p=2000,0");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost22: MecHost {
            @display("p=2000,500");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost23: MecHost {
            @display("p=2000,1000");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost24: MecHost {
            @display("p=2000,1500");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        } 
        mecHost25: MecHost {
            @display("p=2000,2000");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
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

}
