//
//                  Simu5G
//
// Authors: Giovanni Nardini, Giovanni Stea, Antonio Virdis (University of Pisa)
//
// This file is part of a software released under the license included in file
// "license.pdf". Please read LICENSE and README files before using it.
// The above files and the present reference are part of the software itself,
// and cannot be removed from it.
//
package mobiedgesim.simulations.MultiMecHost;

import mobiedgesim.nodes.mobileNodes.mmecHost.MmecHost;
import mobiedgesim.nodes.mobileNodes.ue.Ue;
import mobiedgesim.nodes.mecHost.MecHost;
import inet.visualizer.common.IntegratedMultiVisualizer;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import org.car2x.veins.subprojects.veins_inet.VeinsInetManager;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
// MultiMecHost
//
// This network includes two gNBs and two MEC hosts
//
network MultiMecHost
{
    parameters:
        **.mgmt.numChannels = 20;
        double playgroundSizeX @unit(m); // x size of the area the nodes are in (in meters)
        double playgroundSizeY @unit(m); // y size of the area the nodes are in (in meters)
        double playgroundSizeZ @unit(m); // z size of the area the nodes are in (in meters)
        @display("bgb=732,483");
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
            @display("p=205,356");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost2: MecHost {
            @display("p=226,356");
            wlan[*].mgmt.typename = "Ieee80211MgmtAdhoc";
            wlan[*].agent.typename = "";
        }
        mecHost3: MecHost {
            @display("p=508,356");
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
    connections:


}
