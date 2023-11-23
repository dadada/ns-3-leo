/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/node-container.h"
#include "ns3/core-module.h"

#include "ns3/leo-module.h"

#include "ns3/test.h"

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
  Simulator::Destroy ();
}

void
LeoTestCase1::DoRun (void)
{
  Time::SetResolution (Time::NS);

  NodeContainer satellites;
  satellites.Create (100);
  NodeContainer gateways;
  gateways.Create (10);
  NodeContainer terminals;
  terminals.Create (20);

  LeoHelper leo;
  leo.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  leo.SetChannelAttribute ("PropagationDelay", StringValue ("ns3::ConstantSpeedPropagationDelayModel"));
  leo.SetChannelAttribute ("PropagationLoss", StringValue ("ns3::IslPropagationLossModel"));
  leo.SetDeviceAttribute ("MobilityModel", StringValue ("ns3::LeoMobilityModel"));

  NetDeviceContainer allDevices = leo.Install (satellites, gateways, terminals);

  // we want to ping terminals
  UdpEchoServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (terminals);

  // TODO routing

  // install a client on each of the terminals
  ApplicationContainer clientApps;
  for (uint32_t i = 1; i < terminals.GetN (); i++)
    {
      Address remote = terminals.Get (i)->GetObject<Ipv6> ()->GetAddress (1, 0).GetAddress ();
      UdpEchoClientHelper echoClient (remote, 9);
      echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
      echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
      echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

      clientApps.Add (echoClient.Install (terminals.Get (i-1)));
    }

  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  Simulator::Run ();
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
