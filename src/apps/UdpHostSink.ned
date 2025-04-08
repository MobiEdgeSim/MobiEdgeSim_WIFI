//
// mechost app for receving the ue udp package to get latency and reply back
// 

package mobiedgesim.apps;
import inet.applications.udpapp.UdpEchoApp;

module UdpHostSink extends UdpEchoApp
{
    parameters:
        @class("MobiEdgeSim::UdpHostSink");
}
