/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/node-container.h"

#include "ns3/leo-module.h"
#include "ns3/test.h"

using namespace ns3;

class LeoMockChannelTransmitUnknownTestCase : public TestCase
{
public:
  LeoMockChannelTransmitUnknownTestCase ();
  virtual ~LeoMockChannelTransmitUnknownTestCase ();

private:
  virtual void DoRun (void);
};

LeoMockChannelTransmitUnknownTestCase::LeoMockChannelTransmitUnknownTestCase ()
  : TestCase ("Test transmission to unkown destination")
{
}

LeoMockChannelTransmitUnknownTestCase::~LeoMockChannelTransmitUnknownTestCase ()
{
}

void
LeoMockChannelTransmitUnknownTestCase::DoRun (void)
{
  Ptr<LeoMockChannel> channel = CreateObject<LeoMockChannel> ();
  Packet *packet = new Packet ();
  Ptr<Packet> p = Ptr<Packet>(packet);
  Ptr<LeoMockNetDevice> dev = CreateObject<LeoMockNetDevice> ();
  dev->SetDeviceType (LeoMockNetDevice::GND);
  int32_t srcId = channel->Attach (dev);
  Address destAddr;
  Time txTime;
  channel->SetAttribute ("PropagationDelay", StringValue ("ns3::ConstantSpeedPropagationDelayModel"));
  channel->SetAttribute ("PropagationLoss", StringValue ("ns3::LeoPropagationLossModel"));
  dev->SetAttribute ("MobilityModel", StringValue ("ns3::ConstantPositionMobilityModel"));
  bool result = channel->TransmitStart (p, srcId, destAddr, txTime);

  NS_TEST_ASSERT_MSG_EQ (result, false, "Unknown destination fails to deliver");
}

class LeoMockChannelTransmitKnownTestCase : public TestCase
{
public:
  LeoMockChannelTransmitKnownTestCase ();
  virtual ~LeoMockChannelTransmitKnownTestCase ();

private:
  virtual void DoRun (void);
};

LeoMockChannelTransmitKnownTestCase::LeoMockChannelTransmitKnownTestCase ()
  : TestCase ("Test transmission to known destination")
{
}

LeoMockChannelTransmitKnownTestCase::~LeoMockChannelTransmitKnownTestCase ()
{
}

void
LeoMockChannelTransmitKnownTestCase::DoRun (void)
{
  Ptr<LeoMockChannel> channel = CreateObject<LeoMockChannel> ();
  channel->SetAttribute ("PropagationDelay", StringValue ("ns3::ConstantSpeedPropagationDelayModel"));
  channel->SetAttribute ("PropagationLoss", StringValue ("ns3::LeoPropagationLossModel"));

  Packet *packet = new Packet ();
  Ptr<Packet> p = Ptr<Packet>(packet);

  Ptr<Node> srcNode = CreateObject<Node> ();
  Ptr<ConstantPositionMobilityModel> loc = CreateObject<ConstantPositionMobilityModel> ();
  srcNode->AggregateObject (loc);
  Ptr<LeoMockNetDevice> srcDev = CreateObject<LeoMockNetDevice> ();
  srcDev->SetNode (srcNode);
  srcDev->SetAttribute ("MobilityModel", StringValue ("ns3::ConstantPositionMobilityModel"));
  srcDev->SetDeviceType (LeoMockNetDevice::GND);
  int32_t srcId = channel->Attach (srcDev);

  Ptr<Node> dstNode = CreateObject<Node> ();
  loc = CreateObject<ConstantPositionMobilityModel> ();
  dstNode->AggregateObject (loc);
  Ptr<LeoMockNetDevice> dstDev = CreateObject<LeoMockNetDevice> ();
  dstDev->SetNode (dstNode);
  dstDev->SetAttribute ("MobilityModel", StringValue ("ns3::WaypointMobilityModel"));
  dstDev->SetDeviceType (LeoMockNetDevice::SAT);
  channel->Attach (dstDev);

  Address destAddr = dstDev->GetAddress ();
  Time txTime;
  bool result = channel->TransmitStart (p, srcId, destAddr, txTime);

  NS_TEST_ASSERT_MSG_EQ (result, true, "Known destination delivers");
}

class LeoMockChannelTransmitSpaceGroundTestCase : public TestCase
{
public:
  LeoMockChannelTransmitSpaceGroundTestCase ();
  virtual ~LeoMockChannelTransmitSpaceGroundTestCase ();

private:
  virtual void DoRun (void);
};

LeoMockChannelTransmitSpaceGroundTestCase::LeoMockChannelTransmitSpaceGroundTestCase ()
  : TestCase ("Test transmission to known destination")
{
}

LeoMockChannelTransmitSpaceGroundTestCase::~LeoMockChannelTransmitSpaceGroundTestCase ()
{
}

void
LeoMockChannelTransmitSpaceGroundTestCase::DoRun (void)
{
  Ptr<LeoMockChannel> channel = CreateObject<LeoMockChannel> ();
  channel->SetAttribute ("PropagationDelay", StringValue ("ns3::ConstantSpeedPropagationDelayModel"));
  channel->SetAttribute ("PropagationLoss", StringValue ("ns3::LeoPropagationLossModel"));

  Packet *packet = new Packet ();
  Ptr<Packet> p = Ptr<Packet>(packet);

  Ptr<Node> srcNode = CreateObject<Node> ();
  Ptr<LeoMockNetDevice> srcDev = CreateObject<LeoMockNetDevice> ();
  srcDev->SetNode (srcNode);
  srcDev->SetAttribute ("MobilityModel", StringValue ("ns3::WaypointMobilityModel"));
  srcDev->SetDeviceType (LeoMockNetDevice::SAT);
  int32_t srcId = channel->Attach (srcDev);

  Ptr<Node> dstNode = CreateObject<Node> ();
  Ptr<LeoMockNetDevice> dstDev = CreateObject<LeoMockNetDevice> ();
  dstDev->SetNode (dstNode);
  dstDev->SetAttribute ("MobilityModel", StringValue ("ns3::ConstantPositionMobilityModel"));
  dstDev->SetDeviceType (LeoMockNetDevice::GND);
  channel->Attach (dstDev);

  Address destAddr = dstDev->GetAddress ();
  Time txTime;
  bool result = channel->TransmitStart (p, srcId, destAddr, txTime);

  NS_TEST_ASSERT_MSG_EQ (result, true, "Space to ground destination delivers");
}

class LeoMockChannelTransmitSpaceSpaceTestCase : public TestCase
{
public:
  LeoMockChannelTransmitSpaceSpaceTestCase ();
  virtual ~LeoMockChannelTransmitSpaceSpaceTestCase ();

private:
  virtual void DoRun (void);
};

LeoMockChannelTransmitSpaceSpaceTestCase::LeoMockChannelTransmitSpaceSpaceTestCase ()
  : TestCase ("Test transmission to known destination")
{
}

LeoMockChannelTransmitSpaceSpaceTestCase::~LeoMockChannelTransmitSpaceSpaceTestCase ()
{
}

void
LeoMockChannelTransmitSpaceSpaceTestCase::DoRun (void)
{
  Ptr<LeoMockChannel> channel = CreateObject<LeoMockChannel> ();
  channel->SetAttribute ("PropagationDelay", StringValue ("ns3::ConstantSpeedPropagationDelayModel"));
  channel->SetAttribute ("PropagationLoss", StringValue ("ns3::LeoPropagationLossModel"));

  Packet *packet = new Packet ();
  Ptr<Packet> p = Ptr<Packet>(packet);

  Ptr<Node> srcNode = CreateObject<Node> ();
  Ptr<LeoMockNetDevice> srcDev = CreateObject<LeoMockNetDevice> ();
  srcDev->SetNode (srcNode);
  srcDev->SetAttribute ("MobilityModel", StringValue ("ns3::WaypointMobilityModel"));
  srcDev->SetDeviceType (LeoMockNetDevice::SAT);
  int32_t srcId = channel->Attach (srcDev);

  Ptr<Node> dstNode = CreateObject<Node> ();
  Ptr<LeoMockNetDevice> dstDev = CreateObject<LeoMockNetDevice> ();
  dstDev->SetNode (dstNode);
  dstDev->SetAttribute ("MobilityModel", StringValue ("ns3::WaypointMobilityModel"));
  dstDev->SetDeviceType (LeoMockNetDevice::SAT);
  channel->Attach (dstDev);

  Address destAddr = dstDev->GetAddress ();
  Time txTime;
  bool result = channel->TransmitStart (p, srcId, destAddr, txTime);

  NS_TEST_ASSERT_MSG_EQ (result, false, "Space to space gets dropped");
}

class LeoMockChannelTestSuite : public TestSuite
{
public:
  LeoMockChannelTestSuite ();
};

LeoMockChannelTestSuite::LeoMockChannelTestSuite ()
  : TestSuite ("leo-mock-channel", UNIT)
{
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new LeoMockChannelTransmitUnknownTestCase, TestCase::QUICK);
  AddTestCase (new LeoMockChannelTransmitKnownTestCase, TestCase::QUICK);
  AddTestCase (new LeoMockChannelTransmitSpaceGroundTestCase, TestCase::QUICK);
  AddTestCase (new LeoMockChannelTransmitSpaceSpaceTestCase, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static LeoMockChannelTestSuite islMockChannelTestSuite;
