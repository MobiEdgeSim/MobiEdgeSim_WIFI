//
// ue app for sending udp package to mechost and get a reply to record the latency

package mobiedgesim.apps;
import inet.applications.udpapp.UdpBasicApp;

module UdpUeApp extends UdpBasicApp
{
    parameters:
        @class("MobiEdgeSim::UdpUeApp");
        

        double updateDestInterval @unit(s) = default(3s);
        double rttTimeout @unit(s) = default(10s);
        
        double latitude = 0;
        double longitude = 0;
        //sendInterval = 100000s;// this is to skip the autosending un udpBasicApp
     
}
