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
  : TestCase ("Test channel transmission")
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
  int32_t srcId = channel->Attach (dev);
  Address destAddr;
  Time txTime;
  bool result = channel->TransmitStart (p, srcId, destAddr, txTime);

  NS_TEST_ASSERT_MSG_EQ (result, false, "Unknown destination fails to deliver");
}

class IslMockChannelTestSuite : public TestSuite
{
public:
  IslMockChannelTestSuite ();
};

IslMockChannelTestSuite::IslMockChannelTestSuite ()
  : TestSuite ("isl-mock-channel", UNIT)
{
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new IslMockChannelTransmitUnknownTestCase, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static IslMockChannelTestSuite islMockChannelTestSuite;
