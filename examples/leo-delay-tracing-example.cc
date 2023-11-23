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

int main (int argc, char *argv[])
{
  std::vector<Orbit> orbits = {
      Orbit (1.150, 53.0, 32, 50),
      Orbit (1.110, 53.8, 32, 50),
      Orbit (1.130, 74.0,  8, 50),
      Orbit (1.275, 81, 5, 75),
      Orbit (1.325, 70, 6, 75),

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
  NodeContainer stations = ground.Install ("contrib/leo/data/ground-stations/usa.waypoints");

  NetDeviceContainer islNet, utNet;

  IslHelper islCh;
  islCh.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  islCh.SetDeviceAttribute ("ReceiveErrorModel", StringValue ("ns3::BurstErrorModel"));
  islCh.SetChannelAttribute ("PropagationDelay", StringValue ("ns3::ConstantSpeedPropagationDelayModel"));
  islCh.SetChannelAttribute ("PropagationLoss", StringValue ("ns3::IslPropagationLossModel"));
  islNet = islCh.Install (satellites);

  LeoChannelHelper utCh;
  utCh.SetConstellation ("TelesatUser");
  utNet = utCh.Install (satellites, stations);

  // Install internet stack on nodes
  AodvHelper aodv;
  aodv.Set ("HelloInterval", TimeValue (Seconds (10)));
  aodv.Set ("TtlStart", UintegerValue (10));
  aodv.Set ("TtlIncrement", UintegerValue (10));
  aodv.Set ("TtlThreshold", UintegerValue (1000));
  aodv.Set ("RreqRetries", UintegerValue (100));
  aodv.Set ("RreqRateLimit", UintegerValue (100));
  aodv.Set ("RerrRateLimit", UintegerValue (100));
  aodv.Set ("ActiveRouteTimeout", TimeValue (Seconds (10)));
  aodv.Set ("NextHopWait", TimeValue (MilliSeconds (100)));
  aodv.Set ("NetDiameter", UintegerValue (1000));
  aodv.Set ("PathDiscoveryTime", TimeValue (Seconds (1)));

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

  // we want to ping terminals
  UdpServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (stations.Get (1));

  // install a client on one of the terminals
  ApplicationContainer clientApps;
  Address remote = stations.Get (1)->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();//utIp.GetAddress (1, 0);
  UdpClientHelper echoClient (remote, 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (360));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
  clientApps.Add (echoClient.Install (stations.Get (3)));

  Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::UdpServer/Rx",
  		   MakeCallback (&EchoRx));

  std::cout << "Context,Sequence Number,Timestamp,Delay" << std::endl;

  serverApps.Start (Seconds (1));
  clientApps.Start (Seconds (2));

  Simulator::Stop (Minutes (60));
  Simulator::Run ();
  Simulator::Destroy ();

  Ptr<UdpServer> server = StaticCast<UdpServer> (serverApps.Get (0));
  std::cout << "Received,Lost" << std::endl
    << server->GetReceived () << "," << server->GetLost () << std::endl;

  return 0;
}
