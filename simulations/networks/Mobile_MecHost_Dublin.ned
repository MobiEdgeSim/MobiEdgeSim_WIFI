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

package mobiedgesim.simulations.networks;

import mobiedgesim.nodes.mobileNodes.mmecHost.MmecHost;
import mobiedgesim.nodes.mobileNodes.ue.Ue;
import mobiedgesim.nodes.mecHost.MecHost;
import mobiedgesim.nodes.orchestrator.Orchestrator;
import inet.visualizer.common.IntegratedMultiVisualizer;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import org.car2x.veins.subprojects.veins_inet.VeinsInetManager;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;

network mobile_MecHost_Dublin
{
    parameters:
        **.mgmt.numChannels = 20;
        double playgroundSizeX @unit(m); // x size of the area the nodes are in (in meters)
        double playgroundSizeY @unit(m); // y size of the area the nodes are in (in meters)
        double playgroundSizeZ @unit(m); // z size of the area the nodes are in (in meters)
        @display("bgb=12191.01,9653.66");
    submodules:
        mmecHost[0]: MmecHost {
            @display("p=7720,1186");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        ue[0]: Ue {
            @display("p=4797,979");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        visualizer: IntegratedMultiVisualizer {
            @display("p=662,427");
        }
        radioMedium: Ieee80211ScalarRadioMedium {
            @display("p=662,1172");
        }
        veinsManager: VeinsInetManager {
            @display("p=662,1875;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=662,2606");
            //config = xml("<config><interface hosts='*' address='145.236.x.x' netmask='255.255.0.0'/></config>");
        }
        orchestrator: Orchestrator {
            @display("p=11690,441");
        }

}
