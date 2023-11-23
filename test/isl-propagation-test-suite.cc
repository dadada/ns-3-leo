/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/test.h"

#include "ns3/leo-module.h"

using namespace ns3;

class IslPropagationAngleTestCase1 : public TestCase
{
public:
  IslPropagationAngleTestCase1 ();
  virtual ~IslPropagationAngleTestCase1 ();

private:
  virtual void DoRun (void);
};

IslPropagationAngleTestCase1::IslPropagationAngleTestCase1 ()
  : TestCase ("Test LOS computation neighboring sats")
{
}

IslPropagationAngleTestCase1::~IslPropagationAngleTestCase1 ()
{
}

void
IslPropagationAngleTestCase1::DoRun (void)
{
  Ptr<ConstantPositionMobilityModel> a = CreateObject<ConstantPositionMobilityModel> ();
  a->SetPosition (Vector3D (IslPropagationLossModel::EARTH_RAD + 1000.0, 10, 0));
  Ptr<ConstantPositionMobilityModel> b = CreateObject<ConstantPositionMobilityModel> ();
  b->SetPosition (Vector3D (IslPropagationLossModel::EARTH_RAD + 1000.0, 0, 0));

  NS_TEST_ASSERT_MSG_EQ (IslPropagationLossModel::GetLos (a, b), true, "LOS of neighboring satellites");
}

class IslPropagationAngleTestCase2 : public TestCase
{
public:
  IslPropagationAngleTestCase2 ();
  virtual ~IslPropagationAngleTestCase2 ();

private:
  virtual void DoRun (void);
};

IslPropagationAngleTestCase2::IslPropagationAngleTestCase2 ()
  : TestCase ("Test LOS of sats without LOS")
{
}

IslPropagationAngleTestCase2::~IslPropagationAngleTestCase2 ()
{
}

void
IslPropagationAngleTestCase2::DoRun (void)
{
  Ptr<ConstantPositionMobilityModel> a = CreateObject<ConstantPositionMobilityModel> ();
  a->SetPosition (Vector3D (IslPropagationLossModel::EARTH_RAD + 1000.0, 0, 0));
  Ptr<ConstantPositionMobilityModel> b = CreateObject<ConstantPositionMobilityModel> ();
  b->SetPosition (Vector3D (- (IslPropagationLossModel::EARTH_RAD + 1000.0), 0, 0));

  NS_TEST_ASSERT_MSG_EQ (IslPropagationLossModel::GetLos (a, b), false, "LOS of opposing");
}

class IslPropagationTestSuite : public TestSuite
{
public:
  IslPropagationTestSuite ();
};

IslPropagationTestSuite::IslPropagationTestSuite ()
  : TestSuite ("leo-isl-propagation", UNIT)
{
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new IslPropagationAngleTestCase1, TestCase::QUICK);
  AddTestCase (new IslPropagationAngleTestCase2, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static IslPropagationTestSuite leoTestSuite;
