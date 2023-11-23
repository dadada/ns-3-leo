
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

class EmptyGndNodeHelperTestCase : public TestCase
{
public:
  EmptyGndNodeHelperTestCase ();
  virtual ~EmptyGndNodeHelperTestCase ();

private:
  virtual void DoRun (void);
};

EmptyGndNodeHelperTestCase::EmptyGndNodeHelperTestCase ()
  : TestCase ("Empty list of positions")
{
}

EmptyGndNodeHelperTestCase::~EmptyGndNodeHelperTestCase ()
{
}

void
EmptyGndNodeHelperTestCase::DoRun (void)
{
  std::vector<std::string> gndWps = {};

  LeoGndNodeHelper gndHelper;
  NodeContainer nodes = gndHelper.Install ("contrib/leo/data/test/empty");

  NS_ASSERT_MSG (nodes.GetN () == 0, "Empty position file produces non-empty node container");
}

// ------------------------------------------------------------------------- //

class SomeGndNodeHelperTestCase : public TestCase
{
public:
  SomeGndNodeHelperTestCase ();
  virtual ~SomeGndNodeHelperTestCase ();

private:
  virtual void DoRun (void);
};

SomeGndNodeHelperTestCase::SomeGndNodeHelperTestCase ()
  : TestCase ("Some ground stations")
{
}

SomeGndNodeHelperTestCase::~SomeGndNodeHelperTestCase ()
{
}

void
SomeGndNodeHelperTestCase::DoRun (void)
{
  LeoGndNodeHelper gndHelper;
  NodeContainer nodes = gndHelper.Install ("contrib/leo/data/test/ground-stations.txt");

  NS_ASSERT_MSG (nodes.GetN () > 1, "No ground stations");

  Ptr<MobilityModel> mob = nodes.Get (0)->GetObject<MobilityModel> ();
  NS_ASSERT_MSG (mob != Ptr<MobilityModel> (), "Mobility model is valid");
}

class GndNodeHelperTestSuite : public TestSuite
{
public:
  GndNodeHelperTestSuite ();
};

GndNodeHelperTestSuite::GndNodeHelperTestSuite ()
  : TestSuite ("leo-gnd-node-helper", UNIT)
{
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new EmptyGndNodeHelperTestCase, TestCase::QUICK);
  AddTestCase (new SomeGndNodeHelperTestCase, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static GndNodeHelperTestSuite gndNodeHelperTestSuite;
