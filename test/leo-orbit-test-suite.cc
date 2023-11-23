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

class LeoOrbitTestSuite : TestSuite
{
public:
  LeoOrbitTestSuite() : TestSuite ("leo-orbit", UNIT) {
      AddTestCase (new LeoOrbitSpeedTestCase, TestCase::QUICK);
      AddTestCase (new LeoOrbitPositionTestCase, TestCase::QUICK);
  }
};

static LeoOrbitTestSuite leoOrbitTestSuite;
