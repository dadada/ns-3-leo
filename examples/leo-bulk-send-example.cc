/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/leo-module.h"
#include "ns3/network-module.h"
#include "ns3/aodv-module.h"
#include "ns3/udp-server.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("LeoBulkSendTracingExample");

uint64_t maxBytes = 1000000000;
uint16_t port = 9;
bool tracing = false;

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
      Orbit (1150, 53.0, 32, 50),
      Orbit (1110, 53.8, 32, 50),
      //Orbit (1130, 74.0,  8, 50),
      //Orbit (1275, 81, 5, 75),
      //Orbit (1325, 70, 6, 75),
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
  //aodv.Set ("HelloInterval", TimeValue (Minutes (1)));
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

  //
  // Create a PacketSinkApplication and install it on node 1
  //
  PacketSinkHelper sink ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), port));
  ApplicationContainer sinkApps = sink.Install (stations.Get (1));

  // install a client on one of the terminals
  ApplicationContainer clientApps;
  Address remote = stations.Get (1)->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();//utIp.GetAddress (1, 0);
  BulkSendHelper source ("ns3::TcpSocketFactory",
  			 remote);
  // Set the amount of data to send in bytes.  Zero is unlimited.
  source.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  ApplicationContainer sourceApps = source.Install (stations.Get (0));
  sourceApps.Start (Seconds (0.0));

  //Config::Connect ("/NodeList/[i]/$ns3::TcpL4Protocol/SocketList/[i]"
  //		   MakeCallback (&EchoRx));

  //
  // Set up tracing if enabled
  //
  if (tracing)
    {
      AsciiTraceHelper ascii;
      utCh.EnableAsciiAll (ascii.CreateFileStream ("tcp-bulk-send.tr"));
      utCh.EnablePcapAll ("tcp-bulk-send", false);
    }

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop (Minutes (1));
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");

  Ptr<PacketSink> sink1 = DynamicCast<PacketSink> (sinkApps.Get (0));
  std::cout << "Total Bytes Received: " << sink1->GetTotalRx () << std::endl;

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
