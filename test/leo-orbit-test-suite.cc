/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/node-container.h"
#include "ns3/test.h"

#include "../model/leo-circular-orbit-mobility-model.h"

using namespace ns3;

class LeoOrbitSpeedTestCase : public TestCase
{
public:
  LeoOrbitSpeedTestCase () : TestCase ("Test speed for 0 altitude") {}
  virtual ~LeoOrbitSpeedTestCase () {}
private:
  virtual void DoRun (void)
  {
    Ptr<LeoCircularOrbitMobilityModel> mob = CreateObject<LeoCircularOrbitMobilityModel> ();
    mob->SetAttribute ("Altitude", DoubleValue (0.0));

    NS_TEST_ASSERT_MSG_EQ ((uint64_t) mob->GetSpeed (), (uint64_t) 7909.79, "Unexpected velocity at earths surface");
  }
};

class LeoOrbitPositionTestCase : public TestCase
{
public:
  LeoOrbitPositionTestCase () : TestCase ("Test position for 0 altitude and 0 inclination") {}
  virtual ~LeoOrbitPositionTestCase () {}
private:
  virtual void DoRun (void)
  {
    Ptr<LeoCircularOrbitMobilityModel> mob = CreateObject<LeoCircularOrbitMobilityModel> ();
    mob->SetAttribute ("Altitude", DoubleValue (0.0));
    mob->SetAttribute ("Inclination", DoubleValue (1.0));

    NS_TEST_ASSERT_MSG_EQ (mob->GetPosition ().GetLength(), LEO_EARTH_RAD_M, "Unexpected position on earths surface for 1 deg inclination");
  }
};

class LeoOrbitProgressTestCase : public TestCase
{
public:
  LeoOrbitProgressTestCase () : TestCase ("Test position for 0 altitude and 1.0 inclination after 360 seconds") {}
  virtual ~LeoOrbitProgressTestCase () {}
private:
  void TestLengthPosition (double expl, double expx, Ptr<LeoCircularOrbitMobilityModel> mob)
  {
    Vector pos = mob->GetPosition ();
    NS_TEST_EXPECT_MSG_NE_INTERNAL (pos.x, expx, "Position not equal", __FILE__, __LINE__);
    NS_TEST_EXPECT_MSG_EQ_TOL_INTERNAL (pos.GetLength (), expl, 0.001, "Position must be different", __FILE__, __LINE__);
  }

  virtual void DoRun (void)
  {
    Ptr<LeoCircularOrbitMobilityModel> mob = CreateObject<LeoCircularOrbitMobilityModel> ();
    mob->SetAttribute ("Altitude", DoubleValue (0.0));
    mob->SetAttribute ("Inclination", DoubleValue (1.0));

    Vector pos = mob->GetPosition ();
    Simulator::Schedule (Seconds (360.0), &LeoOrbitProgressTestCase::TestLengthPosition, this, LEO_EARTH_RAD_M, pos.x, mob);
    Simulator::Run ();
    Simulator::Destroy ();
  }
};

class LeoOrbitOffsetTestCase : public TestCase
{
public:
  LeoOrbitOffsetTestCase () : TestCase ("Test offset between neighboring satellites") {}
  virtual ~LeoOrbitOffsetTestCase () {}
private:
  virtual void DoRun (void)
  {
    Ptr<LeoCircularOrbitMobilityModel> mob = CreateObject<LeoCircularOrbitMobilityModel> ();
    mob->SetAttribute ("Altitude", DoubleValue (1000.0));
    mob->SetAttribute ("Inclination", DoubleValue (20.0));

    Vector pos1 = mob->GetPosition ();

    mob->SetAttribute ("Offset", DoubleValue (20.0));
    Vector pos2 = mob->GetPosition ();

    NS_TEST_ASSERT_MSG_NE (pos1.x, pos2.x, "Neighboring satellite should have different position");
  }
};

class LeoOrbitTestSuite : TestSuite
{
public:
  LeoOrbitTestSuite() : TestSuite ("leo-orbit", UNIT) {
      AddTestCase (new LeoOrbitSpeedTestCase, TestCase::QUICK);
      AddTestCase (new LeoOrbitPositionTestCase, TestCase::QUICK);
      AddTestCase (new LeoOrbitProgressTestCase, TestCase::QUICK);
      AddTestCase (new LeoOrbitOffsetTestCase, TestCase::QUICK);
  }
};

static LeoOrbitTestSuite leoOrbitTestSuite;
