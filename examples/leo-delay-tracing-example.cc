/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/leo-module.h"
#include "ns3/network-module.h"
#include "ns3/aodv-module.h"
#include "ns3/udp-server.h"
#include "ns3/epidemic-routing-module.h"

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

uint64_t countBytes = 0;
static void
TracePacket (std::string context, Ptr<const Packet> packet)
{
  Ptr<Packet> p = packet->Copy ();
  std::cout << Simulator::Now () << ":" << context << ":" << (countBytes += p->GetSerializedSize ()) << ":" << p->GetUid () << std::endl;
}

int main (int argc, char *argv[])
{

  CommandLine cmd;
  std::string orbitFile;
  std::string traceFile;
  LeoLatLong source;
  LeoLatLong destination;
  std::string islRate;
  std::string constellation;
  uint32_t latGws = 20;
  uint32_t lonGws = 20;
  double interval;
  double duration;
  bool islEnabled = false;
  bool traceDrops = false;
  bool traceTxRx = false;
  std::string routingProto = "aodv";
  cmd.AddValue("orbitFile", "CSV file with orbit parameters", orbitFile);
  cmd.AddValue("traceFile", "CSV file to store mobility trace in", traceFile);
  cmd.AddValue("precision", "ns3::LeoCircularOrbitMobilityModel::Precision");
  cmd.AddValue("duration", "Duration of the simulation in seconds", duration);
  cmd.AddValue("source", "Traffic source", source);
  cmd.AddValue("destination", "Traffic destination", destination);
  cmd.AddValue("islRate", "ns3::MockNetDevice::DataRate");
  cmd.AddValue("constellation", "LEO constellation link settings name", constellation);
  cmd.AddValue("interval", "Echo interval", interval);
  cmd.AddValue("routing", "Routing protocol", routingProto);
  cmd.AddValue("ttlThresh", "ns3::aodv::RoutingProtocol::TtlThreshold");
  cmd.AddValue("netDiameter", "ns3::aodv::RoutingProtocol::NetDiameter");
  cmd.AddValue("routeTimeout", "ns3::aodv::RoutingProtocol::ActiveRouteTimeout");
  cmd.AddValue("islEnabled", "Enable inter-satellite links", islEnabled);
  cmd.AddValue("traceDrops", "Enable tracing of PHY and MAC drops", traceDrops);
  cmd.AddValue("traceTxRx", "Enable tracing of PHY and MAC transmits", traceTxRx);
  cmd.AddValue("latGws", "Latitudal rows of gateways", latGws);
  cmd.AddValue("lonGws", "Longitudinal rows of gateways", lonGws);
  cmd.AddValue("destinationOnly", "ns3::aodv::RoutingProtocol::DestinationOnly");
  cmd.Parse (argc, argv);

  std::streambuf *coutbuf = std::cout.rdbuf();
  // redirect cout if traceFile
  std::ofstream out;
  out.open (traceFile);
  if (out.is_open ())
    {
      std::cout.rdbuf(out.rdbuf());
    }

  LeoOrbitNodeHelper orbit;
  NodeContainer satellites = orbit.Install (orbitFile);

  LeoGndNodeHelper ground;
  NodeContainer stations = ground.Install (latGws, lonGws);

  NodeContainer users = ground.Install (source, destination);
  stations.Add (users);

  LeoChannelHelper utCh;
  utCh.SetConstellation (constellation);
  NetDeviceContainer utNet = utCh.Install (satellites, stations);

  InternetStackHelper stack;
  if (routingProto == "epidemic")
    {
      EpidemicHelper epidemic;
      stack.SetRoutingHelper (epidemic);
    }
  else
    {
      AodvHelper aodv;
      aodv.Set ("EnableHello", BooleanValue (false));
      //aodv.Set ("RreqLimit", UintegerValue (2));

      stack.SetRoutingHelper (aodv);
    }

  // Install internet stack on nodes
  stack.Install (satellites);
  stack.Install (stations);

  Ipv4AddressHelper ipv4;

  ipv4.SetBase ("10.1.0.0", "255.255.0.0");
  ipv4.Assign (utNet);

  if (islEnabled)
    {
      std::cerr << "ISL enabled" << std::endl;
      IslHelper islCh;
      NetDeviceContainer islNet = islCh.Install (satellites);
      ipv4.SetBase ("10.2.0.0", "255.255.0.0");
      ipv4.Assign (islNet);
    }

  Ptr<Node> client = users.Get (0);
  Ptr<Node> server = users.Get (1);

  // we want to ping terminals
  UdpServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (server);

  // install a client on one of the terminals
  ApplicationContainer clientApps;
  Address remote = server->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();
  UdpClientHelper echoClient (remote, 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (duration / interval));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (interval)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
  clientApps.Add (echoClient.Install (client));

  Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::UdpServer/Rx",
  		   MakeCallback (&EchoRx));

  if (traceDrops)
    {
      Config::Connect ("/NodeList/*/DeviceList/*/$ns3::MockNetDevice/MacTxDrop",
  		       MakeCallback (&TracePacket));
      Config::Connect ("/NodeList/*/DeviceList/*/$ns3::MockNetDevice/PhyTxDrop",
  		       MakeCallback (&TracePacket));
      Config::Connect ("/NodeList/*/DeviceList/*/$ns3::MockNetDevice/MacRxDrop",
  		       MakeCallback (&TracePacket));
      Config::Connect ("/NodeList/*/DeviceList/*/$ns3::MockNetDevice/PhyRxDrop",
  		       MakeCallback (&TracePacket));
    }

  if (traceTxRx)
    {
      Config::Connect ("/NodeList/*/DeviceList/*/$ns3::MockNetDevice/MacTx",
  		       MakeCallback (&TracePacket));
      Config::Connect ("/NodeList/*/DeviceList/*/$ns3::MockNetDevice/MacRx",
  		       MakeCallback (&TracePacket));
      Config::Connect ("/NodeList/*/DeviceList/*/$ns3::MockNetDevice/PhyRx",
  		       MakeCallback (&TracePacket));
    }

  std::cerr << "LOCAL =" << client->GetId () << std::endl;
  std::cerr << "REMOTE=" << server->GetId () << ",addr=" << Ipv4Address::ConvertFrom (remote) << std::endl;

  std::cout << "Context,Sequence Number,Timestamp,Delay" << std::endl;

  //serverApps.Start (Seconds (1));
  //clientApps.Start (Seconds (1));

  Simulator::Stop (Seconds (duration));
  Simulator::Run ();
  Simulator::Destroy ();

  Ptr<UdpServer> result = StaticCast<UdpServer> (serverApps.Get (0));
  std::cout << "Received,Lost" << std::endl
    << result->GetReceived () << "," << result->GetLost () << std::endl;

  out.close ();
  std::cout.rdbuf(coutbuf);

  return 0;
}
