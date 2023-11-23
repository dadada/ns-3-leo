/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/test.h"

#include "ns3/leo-module.h"

using namespace ns3;

class LeoPropagationAngleTestCase1 : public TestCase
{
public:
  LeoPropagationAngleTestCase1 ();
  virtual ~LeoPropagationAngleTestCase1 ();

private:
  virtual void DoRun (void);
};

LeoPropagationAngleTestCase1::LeoPropagationAngleTestCase1 ()
  : TestCase ("Test angle computation 90 deg")
{
}

LeoPropagationAngleTestCase1::~LeoPropagationAngleTestCase1 ()
{
}

void
LeoPropagationAngleTestCase1::DoRun (void)
{
  Ptr<ConstantPositionMobilityModel> a = CreateObject<ConstantPositionMobilityModel> ();
  a->SetPosition (Vector3D (10, 0, 0));
  Ptr<ConstantPositionMobilityModel> b = CreateObject<ConstantPositionMobilityModel> ();
  b->SetPosition (Vector3D (0, 10, 0));

  double angle = LeoPropagationLossModel::GetAngle (a, b);

  NS_TEST_ASSERT_MSG_EQ ((M_PI/4 - 0.1 < angle && angle < M_PI/4 + 0.1), true, "Angle should be 90 deg.");
}

class LeoPropagationAngleTestCase2 : public TestCase
{
public:
  LeoPropagationAngleTestCase2 ();
  virtual ~LeoPropagationAngleTestCase2 ();

private:
  virtual void DoRun (void);
};

LeoPropagationAngleTestCase2::LeoPropagationAngleTestCase2 ()
  : TestCase ("Test angle computation 0 deg")
{
}

LeoPropagationAngleTestCase2::~LeoPropagationAngleTestCase2 ()
{
}

void
LeoPropagationAngleTestCase2::DoRun (void)
{
  Ptr<ConstantPositionMobilityModel> a = CreateObject<ConstantPositionMobilityModel> ();
  a->SetPosition (Vector3D (10, 0, 0));
  Ptr<ConstantPositionMobilityModel> b = CreateObject<ConstantPositionMobilityModel> ();
  b->SetPosition (Vector3D (10, 0, 0));

  double angle = LeoPropagationLossModel::GetAngle (a, b);

  NS_TEST_ASSERT_MSG_EQ (isnan(angle), true, "Angle should be 0 deg.");
}

class LeoPropagationAngleTestCase3 : public TestCase
{
public:
  LeoPropagationAngleTestCase3 ();
  virtual ~LeoPropagationAngleTestCase3 ();

private:
  virtual void DoRun (void);
};

LeoPropagationAngleTestCase3::LeoPropagationAngleTestCase3 ()
  : TestCase ("Test angle computation 0 vector")
{
}

LeoPropagationAngleTestCase3::~LeoPropagationAngleTestCase3 ()
{
}

void
LeoPropagationAngleTestCase3::DoRun (void)
{
  Ptr<ConstantPositionMobilityModel> a = CreateObject<ConstantPositionMobilityModel> ();
  a->SetPosition (Vector3D (0, 0, 0));
  Ptr<ConstantPositionMobilityModel> b = CreateObject<ConstantPositionMobilityModel> ();
  b->SetPosition (Vector3D (0, 0, 0));

  double angle = LeoPropagationLossModel::GetAngle (a, b);

  NS_TEST_ASSERT_MSG_EQ (isnan(angle), true, "Angle should be NaN");
}

// The TestSuite class names the TestSuite, identifies what type of TestSuite,
// and enables the TestCases to be run.  Typically, only the constructor for
// this class must be defined
//
class LeoPropagationTestSuite : public TestSuite
{
public:
  LeoPropagationTestSuite ();
};

LeoPropagationTestSuite::LeoPropagationTestSuite ()
  : TestSuite ("leo-propagation", UNIT)
{
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new LeoPropagationAngleTestCase1, TestCase::QUICK);
  AddTestCase (new LeoPropagationAngleTestCase2, TestCase::QUICK);
  AddTestCase (new LeoPropagationAngleTestCase3, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static LeoPropagationTestSuite leoTestSuite;
