/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/leo-module.h"
#include "ns3/test.h"

using namespace ns3;

class LeoMobilityWaypointTestCase : public TestCase
{
public:
  LeoMobilityWaypointTestCase ();
  virtual ~LeoMobilityWaypointTestCase () {}

private:
  virtual void DoRun (void);
};

LeoMobilityWaypointTestCase::LeoMobilityWaypointTestCase ()
  : TestCase ("Feed waypoints to mobility model")
{
}

void
LeoMobilityWaypointTestCase::DoRun (void)
{
  Ptr<LeoWaypointInputFileStreamContainer> container = CreateObject<LeoWaypointInputFileStreamContainer> ();
  container->SetAttribute("File", StringValue ("contrib/leo/data/waypoints.txt"));
  container->SetAttribute("LastTime", TimeValue (Time (1)));

  Ptr<WaypointMobilityModel> mobility = CreateObject<WaypointMobilityModel> ();
  Waypoint wp;
  while (container->GetNextSample (wp))
    {
      mobility->AddWaypoint (wp);
    }

  NS_TEST_ASSERT_MSG_EQ ((mobility->WaypointsLeft () > 0), true, "Reading waypoints from empty");
}

class LeoMobilityTestSuite : public TestSuite
{
public:
  LeoMobilityTestSuite ();
};

LeoMobilityTestSuite::LeoMobilityTestSuite ()
  : TestSuite ("leo-mobility", UNIT)
{
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new LeoMobilityWaypointTestCase, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static LeoMobilityTestSuite leoMobilityTestSuite;
