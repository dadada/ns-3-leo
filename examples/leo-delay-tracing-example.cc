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

Address remote;

static void
EchoRx (std::string context, Ptr<const Packet> packet)
{
  SeqTsHeader seqTs;
  Ptr<Packet> p = packet->Copy ();
  p->RemoveHeader (seqTs);
  std::cout << Simulator::Now () << ":" << context << ":" << packet->GetUid() << ":" << seqTs.GetSeq () << ":" << Simulator::Now () - seqTs.GetTs () << std::endl;
}

uint64_t countBytes = 0;
static void
TracePacket (std::string context, Ptr<const Packet> packet)
{
  Ptr<Packet> p = packet->Copy ();
  std::cout << Simulator::Now () << ":" << context << ":" << p->GetUid () << ":" << (countBytes += p->GetSerializedSize ()) << std::endl;
}

static void
TraceIpForward (std::string context, const Ipv4Header &header, Ptr<const Packet> packet, uint32_t interface)
{
  if (header.GetDestination () == Ipv4Address::ConvertFrom (remote))
    {
      std::cout << Simulator::Now () << ":" << context  << ":" << packet->GetUid () << ":" << interface << ":" << header.GetSource () << ":" << header.GetDestination () << std::endl;
    }
}

static void
TraceIpDrop (std::string context, const Ipv4Header &header, Ptr< const Packet > packet, Ipv4L3Protocol::DropReason reason, Ptr< Ipv4 > ipv4, uint32_t interface)
{
  std::string res;
  switch (reason)
    {
    case Ipv4L3Protocol::DROP_INTERFACE_DOWN:
      res = "Interface down";
      break;
    case Ipv4L3Protocol::DROP_BAD_CHECKSUM:
      res = "Checksum";
      break;
    case Ipv4L3Protocol::DROP_FRAGMENT_TIMEOUT:
      res = "Timeout";
      break;
    case Ipv4L3Protocol::DROP_NO_ROUTE:
      res = "No route";
      break;
    case Ipv4L3Protocol::DROP_TTL_EXPIRED:
      res = "TTL expired";
      break;
    case Ipv4L3Protocol::DROP_ROUTE_ERROR:
      res = "Route error";
      break;
    };
  std::cout << Simulator::Now () << ":" << context << ":" << packet->GetUid () << ":" << interface << ":" << header.GetSource () << ":" << header.GetDestination () << ":" << res << std::endl;
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
  bool traceFwd = false;
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
  cmd.AddValue("islEnabled", "Enable inter-satellite links", islEnabled);
  cmd.AddValue("traceDrops", "Enable tracing of PHY and MAC drops", traceDrops);
  cmd.AddValue("traceTxRx", "Enable tracing of PHY and MAC transmits", traceTxRx);
  cmd.AddValue("traceFwd", "Enable tracing of IP layer forwarding", traceFwd);
  cmd.AddValue("latGws", "Latitudal rows of gateways", latGws);
  cmd.AddValue("lonGws", "Longitudinal rows of gateways", lonGws);
  cmd.AddValue("destOnly", "ns3::aodv::RoutingProtocol::DestinationOnly");
  cmd.AddValue("routeTimeout", "ns3::aodv::RoutingProtocol::ActiveRouteTimeout");
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
      //aodv.Set ("HelloInterval", TimeValue (Seconds (5)));
      //aodv.Set ("RreqRetries", UintegerValue (1000));
      //aodv.Set ("RerrRateLimit", UintegerValue (1000));
      //aodv.Set ("RreqRateLimit", UintegerValue (10));
      aodv.Set ("TtlIncrement", UintegerValue (5));
      aodv.Set ("TtlThreshold", UintegerValue (25));
      aodv.Set ("NetDiameter", UintegerValue (50));
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
  remote = server->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();
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

  if (traceFwd)
    {
      Config::Connect ("/NodeList/*/$ns3::Ipv4L3Protocol/UnicastForward",
		       MakeCallback (&TraceIpForward));
      Config::Connect ("/NodeList/*/$ns3::Ipv4L3Protocol/MulticastForward",
		       MakeCallback (&TraceIpForward));
      Config::Connect ("/NodeList/*/$ns3::Ipv4L3Protocol/Drop",
		       MakeCallback (&TraceIpDrop));
    }

  std::cerr << "LOCAL =" << client->GetId () << std::endl;
  std::cerr << "REMOTE=" << server->GetId () << ",addr=" << Ipv4Address::ConvertFrom (remote) << std::endl;

  //serverApps.Start (Seconds (1));
  //clientApps.Start (Seconds (1));

  Simulator::Stop (Seconds (duration));
  Simulator::Run ();
  Simulator::Destroy ();

  out.close ();
  std::cout.rdbuf(coutbuf);

  return 0;
}
