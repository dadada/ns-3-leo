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

class EmptySatNodeHelperTestCase : public TestCase
{
public:
  EmptySatNodeHelperTestCase ();
  virtual ~EmptySatNodeHelperTestCase ();

private:
  virtual void DoRun (void);
};

EmptySatNodeHelperTestCase::EmptySatNodeHelperTestCase ()
  : TestCase ("Empty list of waypoint files")
{
}

EmptySatNodeHelperTestCase::~EmptySatNodeHelperTestCase ()
{
}

void
EmptySatNodeHelperTestCase::DoRun (void)
{
  std::vector<std::string> satWps = {};

  LeoSatNodeHelper satHelper;
  NodeContainer satellites = satHelper.Install (satWps);

  NS_ASSERT_MSG (satellites.GetN () == 0, "Empty list of waypoint files produces non-empty node container");
}

// ------------------------------------------------------------------------- //

class SingleSatNodeHelperTestCase : public TestCase
{
public:
  SingleSatNodeHelperTestCase ();
  virtual ~SingleSatNodeHelperTestCase ();

private:
  virtual void DoRun (void);
};

SingleSatNodeHelperTestCase::SingleSatNodeHelperTestCase ()
  : TestCase ("Single waypoint file")
{
}

SingleSatNodeHelperTestCase::~SingleSatNodeHelperTestCase ()
{
}

void
SingleSatNodeHelperTestCase::DoRun (void)
{
  std::vector<std::string> satWps =
    {
      "contrib/leo/data/test/waypoints.txt"
    };

  LeoSatNodeHelper satHelper;
  NodeContainer satellites = satHelper.Install (satWps);

  NS_ASSERT_MSG (satellites.GetN () == 1, "No satellite nodes");

  Ptr<MobilityModel> mob = satellites.Get (0)->GetObject<MobilityModel> ();
  NS_ASSERT_MSG (mob != Ptr<MobilityModel> (), "Mobility model is valid");
}

class SatNodeHelperTestSuite : public TestSuite
{
public:
  SatNodeHelperTestSuite ();
};

SatNodeHelperTestSuite::SatNodeHelperTestSuite ()
  : TestSuite ("sat-node-helper", UNIT)
{
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new EmptySatNodeHelperTestCase, TestCase::QUICK);
  AddTestCase (new SingleSatNodeHelperTestCase, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static SatNodeHelperTestSuite satNodeHelperTestSuite;
