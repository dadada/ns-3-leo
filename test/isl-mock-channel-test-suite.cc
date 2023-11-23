/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/node-container.h"

#include "ns3/leo-module.h"
#include "ns3/test.h"

using namespace ns3;

class IslMockChannelTransmitUnknownTestCase : public TestCase
{
public:
  IslMockChannelTransmitUnknownTestCase ();
  virtual ~IslMockChannelTransmitUnknownTestCase ();

private:
  virtual void DoRun (void);
};

IslMockChannelTransmitUnknownTestCase::IslMockChannelTransmitUnknownTestCase ()
  : TestCase ("Test transmission to broadcast address")
{
}

IslMockChannelTransmitUnknownTestCase::~IslMockChannelTransmitUnknownTestCase ()
{
}

void
IslMockChannelTransmitUnknownTestCase::DoRun (void)
{
  Ptr<IslMockChannel> channel = CreateObject<IslMockChannel> ();
  Packet *packet = new Packet ();
  Ptr<Packet> p = Ptr<Packet>(packet);
  Ptr<MockNetDevice> dev = CreateObject<MockNetDevice> ();
  dev->SetAddress (Mac48Address::Allocate ());
  int32_t srcId = channel->Attach (dev);
  Address destAddr = Mac48Address::GetBroadcast ();
  Time txTime;
  bool result = channel->TransmitStart (p, srcId, destAddr, txTime);

  NS_TEST_ASSERT_MSG_EQ (result, true, "Broadcast address delivers to all");
}

class IslMockChannelTransmitKnownTestCase : public TestCase
{
public:
  IslMockChannelTransmitKnownTestCase ();
  virtual ~IslMockChannelTransmitKnownTestCase ();

private:
  virtual void DoRun (void);
};

IslMockChannelTransmitKnownTestCase::IslMockChannelTransmitKnownTestCase ()
  : TestCase ("Test transmission to known destination")
{
}

IslMockChannelTransmitKnownTestCase::~IslMockChannelTransmitKnownTestCase ()
{
}

void
IslMockChannelTransmitKnownTestCase::DoRun (void)
{
  Ptr<IslMockChannel> channel = CreateObject<IslMockChannel> ();
  channel->SetAttribute ("PropagationDelay", StringValue ("ns3::ConstantSpeedPropagationDelayModel"));
  channel->SetAttribute ("PropagationLoss", StringValue ("ns3::IslPropagationLossModel"));

  Packet *packet = new Packet ();
  Ptr<Packet> p = Ptr<Packet>(packet);

  Ptr<Node> srcNode = CreateObject<Node> ();
  Ptr<ConstantPositionMobilityModel> loc = CreateObject<ConstantPositionMobilityModel> ();
  srcNode->AggregateObject (loc);
  Ptr<MockNetDevice> srcDev = CreateObject<MockNetDevice> ();
  srcDev->SetNode (srcNode);
  srcDev->SetAddress (Mac48Address::Allocate ());
  int32_t srcId = channel->Attach (srcDev);

  Ptr<Node> dstNode = CreateObject<Node> ();
  loc = CreateObject<ConstantPositionMobilityModel> ();
  dstNode->AggregateObject (loc);
  Ptr<MockNetDevice> dstDev = CreateObject<MockNetDevice> ();
  dstDev->SetNode (dstNode);
  dstDev->SetAddress (Mac48Address::Allocate ());
  channel->Attach (dstDev);

  Address destAddr = dstDev->GetAddress ();
  Time txTime;
  bool result = channel->TransmitStart (p, srcId, destAddr, txTime);

  NS_TEST_ASSERT_MSG_EQ (result, true, "Known destination delivers");
}

class IslMockChannelTestSuite : public TestSuite
{
public:
  IslMockChannelTestSuite ();
};

IslMockChannelTestSuite::IslMockChannelTestSuite ()
  : TestSuite ("leo-isl-mock-channel", UNIT)
{
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new IslMockChannelTransmitUnknownTestCase, TestCase::QUICK);
  AddTestCase (new IslMockChannelTransmitKnownTestCase, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static IslMockChannelTestSuite islMockChannelTestSuite;
