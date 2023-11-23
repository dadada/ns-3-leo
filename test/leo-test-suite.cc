/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/node-container.h"
#include "ns3/core-module.h"
#include "ns3/aodv-module.h"
#include "ns3/test.h"

#include "ns3/leo-module.h"

using namespace ns3;

class LeoTestCase1 : public TestCase
{
public:
  LeoTestCase1 ();
  virtual ~LeoTestCase1 ();

private:
  virtual void DoRun (void);
};

LeoTestCase1::LeoTestCase1 ()
  : TestCase ("Leo test case (does nothing)")
{
}

LeoTestCase1::~LeoTestCase1 ()
{
}

void
LeoTestCase1::DoRun (void)
{
  Time::SetResolution (Time::MS);

  std::vector<std::string> satWps =
    {
      "contrib/leo/data/test/waypoints.txt",
      "contrib/leo/data/test/waypoints.txt",
      "contrib/leo/data/test/waypoints.txt",
      "contrib/leo/data/test/waypoints.txt",
    };

  LeoSatNodeHelper satHelper;
  NodeContainer satellites = satHelper.Install (satWps);
  LeoGndNodeHelper gndHelper;
  NodeContainer gateways = gndHelper.Install ("contrib/leo/data/test/ground-stations.txt");
  NodeContainer terminals = gndHelper.Install ("contrib/leo/data/test/ground-stations.txt");

  NetDeviceContainer islNet, gwNet, utNet;

  IslHelper islCh;
  islCh.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  islCh.SetDeviceAttribute ("ReceiveErrorModel", StringValue ("ns3::BurstErrorModel"));
  islCh.SetChannelAttribute ("PropagationDelay", StringValue ("ns3::ConstantSpeedPropagationDelayModel"));
  //// TODO propagation loss from mobility model
  islCh.SetChannelAttribute ("PropagationLoss", StringValue ("ns3::RangePropagationLossModel"));
  islNet = islCh.Install (satellites);

  LeoChannelHelper gwCh;
  gwCh.SetGndDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  gwCh.SetGndDeviceAttribute ("ReceiveErrorModel", StringValue ("ns3::BurstErrorModel"));
  gwCh.SetSatDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  gwCh.SetSatDeviceAttribute ("ReceiveErrorModel", StringValue ("ns3::BurstErrorModel"));
  gwCh.SetChannelAttribute ("PropagationDelay", StringValue ("ns3::ConstantSpeedPropagationDelayModel"));
  // TODO propagation loss from mobility model
  gwCh.SetChannelAttribute ("PropagationLoss", StringValue ("ns3::RangePropagationLossModel"));
  gwNet = gwCh.Install (satellites, gateways);

  LeoChannelHelper utCh;
  utCh.SetGndDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  utCh.SetGndDeviceAttribute ("ReceiveErrorModel", StringValue ("ns3::BurstErrorModel"));
  utCh.SetSatDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  utCh.SetSatDeviceAttribute ("ReceiveErrorModel", StringValue ("ns3::BurstErrorModel"));
  utCh.SetChannelAttribute ("PropagationDelay", StringValue ("ns3::ConstantSpeedPropagationDelayModel"));
  // TODO propagation loss from mobility model
  utCh.SetChannelAttribute ("PropagationLoss", StringValue ("ns3::RangePropagationLossModel"));
  utNet = utCh.Install (satellites, terminals);

  // Install internet stack on nodes
  InternetStackHelper stack;
  AodvHelper aodv;
  stack.SetRoutingHelper (aodv);
  stack.Install (satellites);
  stack.Install (gateways);
  stack.Install (terminals);

  // Make all networks addressable for legacy protocol
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.0.0", "255.255.0.0");
  Ipv4InterfaceContainer islIp = ipv4.Assign (islNet);
  ipv4.SetBase ("10.2.0.0", "255.255.0.0");
  Ipv4InterfaceContainer gwIp = ipv4.Assign (gwNet);
  ipv4.SetBase ("10.3.0.0", "255.255.0.0");
  Ipv4InterfaceContainer utIp = ipv4.Assign (utNet);

  ArpCacheHelper arpCache;
  arpCache.Install (islNet, islIp);
  arpCache.Install (gwNet, gwIp);
  arpCache.Install (utNet, utIp);

  // we want to ping terminals
  UdpEchoServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (terminals);

  // install a client on one of the terminals
  ApplicationContainer clientApps;
  Address remote = utIp.GetAddress (1, 0);
  UdpEchoClientHelper echoClient (remote, 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (3));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
  clientApps.Add (echoClient.Install (terminals.Get (0)));

  serverApps.Start (Seconds (1.0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (9.0));
  serverApps.Stop (Seconds (10));

  Simulator::Run ();
  Simulator::Destroy ();
}

// The TestSuite class names the TestSuite, identifies what type of TestSuite,
// and enables the TestCases to be run.  Typically, only the constructor for
// this class must be defined
//
class LeoTestSuite : public TestSuite
{
public:
  LeoTestSuite ();
};

LeoTestSuite::LeoTestSuite ()
  : TestSuite ("leo", EXAMPLE)
{
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new LeoTestCase1, TestCase::EXTENSIVE);
}

// Do not forget to allocate an instance of this TestSuite
static LeoTestSuite leoTestSuite;
