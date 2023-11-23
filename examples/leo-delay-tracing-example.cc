/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/leo-module.h"
#include "ns3/network-module.h"
#include "ns3/aodv-module.h"
#include "ns3/udp-server.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("LeoDelayTracingExample");

static void
EchoRx (std::string context, Ptr<const Packet> packet)
{
  SeqTsHeader seqTs;
  Ptr<Packet> p = packet->Copy ();
  p->RemoveHeader (seqTs);
  // seqnr, timestamp, delay
  std::cout << context << "," << seqTs.GetSeq () << "," << seqTs.GetTs () << "," << Simulator::Now () - seqTs.GetTs () << std::endl;
}

class Orbit {
public:
  Orbit (double a, double i, double p, double s) : alt (a), inc (i), planes (p), sats (s) {}
  double alt;
  double inc;
  uint16_t planes;
  uint16_t sats;
};


// Starlink
//Orbit (1150, 53.0, 32, 50),
//Orbit (1110, 53.8, 32, 50),
//Orbit (1130, 74.0,  8, 50),
//Orbit (1275, 81, 5, 75),
//Orbit (1325, 70, 6, 75),

// Telesat
//Orbit (1000.0, 99.5, 6, 12),
//Orbit (1248.0, 37.4, 5, 9),

int main (int argc, char *argv[])
{
  std::vector<Orbit> orbits = {
  //Orbit (1000.0, 99.5, 6, 12),
  //Orbit (1248.0, 37.4, 5, 9),
Orbit (1150, 53.0, 32, 50),
Orbit (1110, 53.8, 32, 50),
//      Orbit (1150, 53.0, 32, 50),
 //     Orbit (1110, 53.8, 32, 50),
  };
  NodeContainer satellites;
  for (Orbit orb: orbits)
    {
      NodeContainer c;
      c.Create (orb.sats*orb.planes);

      MobilityHelper mobility;
      mobility.SetPositionAllocator ("ns3::LeoCircularOrbitPostionAllocator",
                                     "NumOrbits", IntegerValue (orb.planes),
                                     "NumSatellites", IntegerValue (orb.sats));
      mobility.SetMobilityModel ("ns3::LeoCircularOrbitMobilityModel",
  			     	 "Altitude", DoubleValue (orb.alt),
  			     	 "Inclination", DoubleValue (orb.inc),
  			     	 "Precision", TimeValue (Minutes (1)));
      mobility.Install (c);
      satellites.Add (c);
    }

  LeoGndNodeHelper ground;
  NodeContainer stations = ground.Install ("contrib/leo/data/ground-stations/usa-60.waypoints");

  NetDeviceContainer islNet, utNet;

  IslHelper islCh;
  islCh.SetDeviceAttribute ("DataRate", StringValue ("1Gbps"));
  islCh.SetChannelAttribute ("PropagationDelay", StringValue ("ns3::ConstantSpeedPropagationDelayModel"));
  islCh.SetChannelAttribute ("PropagationLoss", StringValue ("ns3::IslPropagationLossModel"));
  islNet = islCh.Install (satellites);

  LeoChannelHelper utCh;
  utCh.SetConstellation ("StarlinkUser");
  utNet = utCh.Install (satellites, stations);

  // Install internet stack on nodes
  AodvHelper aodv;
  // This disabled is far better for performance (huge network)
  aodv.Set ("EnableHello", BooleanValue (false));
  //aodv.Set ("HelloInterval", TimeValue (Seconds (10)));
  //aodv.Set ("TtlStart", UintegerValue (2));
  //aodv.Set ("TtlIncrement", UintegerValue (1));
  //aodv.Set ("TtlThreshold", UintegerValue (20));
  //aodv.Set ("RreqRetries", UintegerValue (1000));
  //aodv.Set ("RreqRateLimit", UintegerValue (1));
  //aodv.Set ("RerrRateLimit", UintegerValue (1));
  //aodv.Set ("ActiveRouteTimeout", TimeValue (Minutes (1)));
  //aodv.Set ("NextHopWait", TimeValue (MilliSeconds (200)));
  //aodv.Set ("NetDiameter", UintegerValue (300));
  //aodv.Set ("AllowedHelloLoss", UintegerValue (10000));
  //aodv.Set ("PathDiscoveryTime", TimeValue (Seconds (1)));

  InternetStackHelper stack;
  stack.SetRoutingHelper (aodv);
  stack.Install (satellites);
  stack.Install (stations);

  // Make all networks addressable for legacy protocol
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.0.0", "255.255.0.0");
  Ipv4InterfaceContainer islIp = ipv4.Assign (islNet);
  ipv4.SetBase ("10.3.0.0", "255.255.0.0");
  Ipv4InterfaceContainer utIp = ipv4.Assign (utNet);

  Ptr<Node> client = stations.Get (0);
  Ptr<Node> server = stations.Get (1);

  // we want to ping terminals
  UdpServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (server);

  // install a client on one of the terminals
  ApplicationContainer clientApps;
  Address remote = server->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();
  UdpClientHelper echoClient (remote, 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (6000));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
  clientApps.Add (echoClient.Install (client));

  Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::UdpServer/Rx",
  		   MakeCallback (&EchoRx));

  std::cout << "LOCAL =" << client->GetId () << std::endl;
  std::cout << "REMOTE=" << server->GetId () << std::endl;

  std::cout << "Context,Sequence Number,Timestamp,Delay" << std::endl;

  serverApps.Start (Seconds (1));
  clientApps.Start (Seconds (1));

  Simulator::Stop (Minutes (10));
  Simulator::Run ();
  Simulator::Destroy ();

  Ptr<UdpServer> result = StaticCast<UdpServer> (serverApps.Get (0));
  std::cout << "Received,Lost" << std::endl
    << result->GetReceived () << "," << result->GetLost () << std::endl;

  return 0;
}
