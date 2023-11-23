/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/test.h"

#include "ns3/leo-module.h"

using namespace ns3;

#define EARTH_RAD 6.3781e6

class IslPropagationAngleTestCase1 : public TestCase
{
public:
  IslPropagationAngleTestCase1 () : TestCase ("neighboring sats have line-of-sight") {}
  virtual ~IslPropagationAngleTestCase1 () {}
private:
  void DoRun (void)
  {
    Ptr<ConstantPositionMobilityModel> a = CreateObject<ConstantPositionMobilityModel> ();
    a->SetPosition (Vector3D (EARTH_RAD + 1.0e6, 10, 0));
    Ptr<ConstantPositionMobilityModel> b = CreateObject<ConstantPositionMobilityModel> ();
    b->SetPosition (Vector3D (EARTH_RAD + 1.0e6, 0, 0));

    NS_TEST_ASSERT_MSG_EQ (IslPropagationLossModel::GetLos (a, b), true, "no line-of-sight");
  }
};

class IslPropagationAngleTestCase2 : public TestCase
{
public:
  IslPropagationAngleTestCase2 () : TestCase ("satellites on opposing sites of earth have no line-of-sight") {}
  virtual ~IslPropagationAngleTestCase2 () {}
private:
  virtual void DoRun (void)
  {
    Ptr<ConstantPositionMobilityModel> a = CreateObject<ConstantPositionMobilityModel> ();
    a->SetPosition (Vector3D (EARTH_RAD + 1.0e6, 0, 0));
    Ptr<ConstantPositionMobilityModel> b = CreateObject<ConstantPositionMobilityModel> ();
    b->SetPosition (Vector3D (- (EARTH_RAD + 1.0e6), 0, 0));

    NS_TEST_ASSERT_MSG_EQ (IslPropagationLossModel::GetLos (a, b), false, "line-of-sight");
  }
};

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
