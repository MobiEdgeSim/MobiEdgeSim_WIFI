//
// ue app for sending udp package to mechost and get a reply to record the latency

package mobiedgesim.apps;
import inet.applications.udpapp.UdpBasicApp;
//
// TODO auto-generated module
//
module UdpUeApp extends UdpBasicApp
{
    parameters:
        @class("MobiEdgeSim::UdpUeApp");
        

        double updateDestInterval @unit(s) = default(1s);
        
        double latitude = 0;
        double longitude = 0;
     
}
