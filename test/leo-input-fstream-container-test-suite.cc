/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/leo-module.h"
#include "ns3/test.h"

using namespace ns3;

class LeoFileInputTestCase : public TestCase
{
public:
  LeoFileInputTestCase ();
  virtual ~LeoFileInputTestCase ();

private:
  virtual void DoRun (void);
};

LeoFileInputTestCase::LeoFileInputTestCase ()
  : TestCase ("Test reading from empty file")
{
}

LeoFileInputTestCase::~LeoFileInputTestCase ()
{
}

void
LeoFileInputTestCase::DoRun (void)
{
  Ptr<LeoWaypointInputFileStreamContainer> container = CreateObject<LeoWaypointInputFileStreamContainer> ();
  container->SetAttribute("File", StringValue ("contrib/leo/data/empty"));
  container->SetAttribute("LastTime", TimeValue (Time (1)));
  Waypoint wp;

  bool res = container->GetNextSample(wp);

  NS_TEST_ASSERT_MSG_EQ (res, false, "Reading from empty stream fails");
}

class LeoWaypointsTestSuite : public TestSuite
{
public:
  LeoWaypointsTestSuite ();
};

LeoWaypointsTestSuite::LeoWaypointsTestSuite ()
  : TestSuite ("leo-waypoints", UNIT)
{
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new LeoFileInputTestCase, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static LeoWaypointsTestSuite leoWaypointsTestSuite;
