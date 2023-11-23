/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/leo-module.h"
#include "ns3/test.h"

using namespace ns3;

class LeoWaypointFileEmptyTestCase : public TestCase
{
public:
  LeoWaypointFileEmptyTestCase ();
  virtual ~LeoWaypointFileEmptyTestCase ();

private:
  virtual void DoRun (void);
};

LeoWaypointFileEmptyTestCase::LeoWaypointFileEmptyTestCase ()
  : TestCase ("Test reading from empty file")
{
}

LeoWaypointFileEmptyTestCase::~LeoWaypointFileEmptyTestCase ()
{
}

void
LeoWaypointFileEmptyTestCase::DoRun (void)
{
  Ptr<LeoWaypointInputFileStreamContainer> container = CreateObject<LeoWaypointInputFileStreamContainer> ();
  container->SetAttribute("File", StringValue ("contrib/leo/data/test/empty"));
  container->SetAttribute("LastTime", TimeValue (Time (1)));
  Waypoint wp;

  uint32_t i = 0;
  while (container->GetNextSample (wp))
    {
      i ++;
    }

  NS_TEST_ASSERT_MSG_EQ ((i == 0), true, "Reading waypoints from empty");
}

class LeoWaypointSomeEntriesTestCase : public TestCase
{
public:
  LeoWaypointSomeEntriesTestCase ();
  virtual ~LeoWaypointSomeEntriesTestCase ();

private:
  virtual void DoRun (void);
};

LeoWaypointSomeEntriesTestCase::LeoWaypointSomeEntriesTestCase ()
  : TestCase ("Test reading from non-empty file")
{
}

LeoWaypointSomeEntriesTestCase::~LeoWaypointSomeEntriesTestCase ()
{
}

void
LeoWaypointSomeEntriesTestCase::DoRun (void)
{
  Ptr<LeoWaypointInputFileStreamContainer> container = CreateObject<LeoWaypointInputFileStreamContainer> ();
  container->SetAttribute("File", StringValue ("contrib/leo/data/waypoints.txt"));
  container->SetAttribute("LastTime", TimeValue (Time (1)));
  Waypoint wp;

  uint32_t i = 0;
  while (container->GetNextSample (wp))
    {
      i ++;
    }

  NS_TEST_ASSERT_MSG_EQ ((i > 0), true, "Reading from non-empty stream succeeds");
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
  AddTestCase (new LeoWaypointFileEmptyTestCase, TestCase::QUICK);
  AddTestCase (new LeoWaypointSomeEntriesTestCase, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static LeoWaypointsTestSuite leoWaypointsTestSuite;
