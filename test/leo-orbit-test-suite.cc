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

class LeoOrbitTestSuite : TestSuite
{
public:
  LeoOrbitTestSuite() : TestSuite ("leo-orbit", UNIT) {
      AddTestCase (new LeoOrbitSpeedTestCase, TestCase::QUICK);
  }
};

static LeoOrbitTestSuite leoOrbitTestSuite;
