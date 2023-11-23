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

int main (int argc, char *argv[])
{

  CommandLine cmd;
  std::string orbitFile;
  std::string traceFile;
  LeoLatLong source;
  LeoLatLong destination;
  std::string islRate;
  std::string constellation;
  uint64_t numGws;
  double interval;
  double duration;
  std::string routingProto = "aodv";
  cmd.AddValue("orbitFile", "CSV file with orbit parameters", orbitFile);
  cmd.AddValue("traceFile", "CSV file to store mobility trace in", traceFile);
  cmd.AddValue("precision", "ns3::LeoCircularOrbitMobilityModel::Precision");
  cmd.AddValue("duration", "Duration of the simulation in seconds", duration);
  cmd.AddValue("numGws", "Number of gateways", numGws);
  cmd.AddValue("source", "Traffic source", source);
  cmd.AddValue("destination", "Traffic destination", destination);
  cmd.AddValue("islRate", "Throughput of the ISL link", islRate);
  cmd.AddValue("constellation", "LEO constellation link settings name", constellation);
  cmd.AddValue("interval", "Echo interval", interval);
  cmd.AddValue("routing", "Routing protocol", routingProto);
  cmd.AddValue("ttlThresh", "ns3::aodv::RoutingProtocol::TtlThreshold");
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
  NodeContainer stations = ground.Install (numGws);
  NodeContainer users = ground.Install (source, destination);
  stations.Add (users);

  Ptr<Node> client = users.Get (0);
  Ptr<Node> server = users.Get (1);

  NetDeviceContainer islNet, utNet;

  IslHelper islCh;
  islCh.SetDeviceAttribute ("DataRate", StringValue (islRate));
  islCh.SetChannelAttribute ("PropagationDelay", StringValue ("ns3::ConstantSpeedPropagationDelayModel"));
  islCh.SetChannelAttribute ("PropagationLoss", StringValue ("ns3::IslPropagationLossModel"));
  islNet = islCh.Install (satellites);

  LeoChannelHelper utCh;
  utCh.SetConstellation (constellation);
  utNet = utCh.Install (satellites, stations);

  InternetStackHelper stack;
  if (routingProto == "epidemic")
    {
      EpidemicHelper epidemic;
      epidemic.Set ("HopCount", UintegerValue (50));
      //epidemic.Set ("QueueLength", UintegerValue (50));
      //epidemic.Set ("QueueEntryExpireTime", TimeValue (Seconds (100)));
      //epidemic.Set ("BeaconInterval", TimeValue (Seconds (1)));

      stack.SetRoutingHelper (epidemic);
    }
  else
    {
      // Install internet stack on nodes
      AodvHelper aodv;
      aodv.Set ("EnableHello", BooleanValue (false));
      aodv.Set ("RreqRateLimit", UintegerValue (1));
      aodv.Set ("RerrRateLimit", UintegerValue (1));

      stack.SetRoutingHelper (aodv);
    }

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

  std::cout << "LOCAL =" << client->GetId () << std::endl;
  std::cout << "REMOTE=" << server->GetId () << std::endl;

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
