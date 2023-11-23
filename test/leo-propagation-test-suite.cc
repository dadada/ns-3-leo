/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "math.h"
#include "ns3/core-module.h"
#include "ns3/test.h"

#include "ns3/leo-module.h"

using namespace ns3;

class LeoPropagationRxNoLosTestCase : public TestCase
{
public:
  LeoPropagationRxNoLosTestCase () : TestCase ("no reception without line-of-sight") {}
  virtual ~LeoPropagationRxNoLosTestCase () {}
private:
  virtual void DoRun (void)
  {
    Ptr<ConstantPositionMobilityModel> a = CreateObject<ConstantPositionMobilityModel> ();
    a->SetPosition (Vector3D (6.7e6, 0, 0));
    Ptr<ConstantPositionMobilityModel> b = CreateObject<ConstantPositionMobilityModel> ();
    b->SetPosition (Vector3D (-7.0e6, 0, 0));

    Ptr<LeoPropagationLossModel> model = CreateObject<LeoPropagationLossModel> ();
    model->SetAttribute ("ElevationAngle", DoubleValue (10.0));

    NS_TEST_ASSERT_MSG_LT (model->CalcRxPower (1.0, a, b), -500.0, "received transmission on destination without line-of-sight");
  }
};

class LeoPropagationRxLosTestCase : public TestCase
{
public:
  LeoPropagationRxLosTestCase () : TestCase ("reception of neighboring nodes") {}
  virtual ~LeoPropagationRxLosTestCase () {}
private:
  virtual void DoRun (void)
  {
    Ptr<ConstantPositionMobilityModel> a = CreateObject<ConstantPositionMobilityModel> ();
    a->SetPosition (Vector3D (7e6, 0, 0));
    Ptr<ConstantPositionMobilityModel> b = CreateObject<ConstantPositionMobilityModel> ();
    b->SetPosition (Vector3D (7.1e6, 0, 0));

    Ptr<LeoPropagationLossModel> model = CreateObject<LeoPropagationLossModel> ();
    model->SetAttribute ("ElevationAngle", DoubleValue (10.0));
    model->SetAttribute ("AtmosphericLoss", DoubleValue (0.0));
    model->SetAttribute ("FreeSpacePathLoss", DoubleValue (0.0));
    model->SetAttribute ("LinkMargin", DoubleValue (0.0));

    NS_TEST_ASSERT_MSG_EQ (model->CalcRxPower (1.0, a, b), 1.0, "Tx not same as Rx power");
  }
};

class LeoPropagationBadAngleTestCase : public TestCase
{
public:
  LeoPropagationBadAngleTestCase () : TestCase ("no reception outside of beam") {}
  virtual ~LeoPropagationBadAngleTestCase () {}
private:
  void DoRun ()
  {
    Ptr<ConstantPositionMobilityModel> a = CreateObject<ConstantPositionMobilityModel> ();
    a->SetPosition (Vector3D (1000000.0, 0, 0));
    Ptr<ConstantPositionMobilityModel> b = CreateObject<ConstantPositionMobilityModel> ();
    b->SetPosition (Vector3D (1000000.0, sin (M_PI / 9) * 1000000.0, 0));

    Ptr<LeoPropagationLossModel> model = CreateObject<LeoPropagationLossModel> ();
    model->SetAttribute ("ElevationAngle", DoubleValue (90));

    NS_TEST_ASSERT_MSG_LT (model->CalcRxPower (1.0, a, b), -500.0, "reception outside of beam");
  }
};

class LeoPropagationLossTestCase : public TestCase
{
public:
  LeoPropagationLossTestCase () : TestCase ("path losses are applied") {}
  virtual ~LeoPropagationLossTestCase () {}
private:
  void DoRun ()
  {
    Ptr<ConstantPositionMobilityModel> a = CreateObject<ConstantPositionMobilityModel> ();
    a->SetPosition (Vector3D (7e6, 0, 0));
    Ptr<ConstantPositionMobilityModel> b = CreateObject<ConstantPositionMobilityModel> ();
    b->SetPosition (Vector3D (7.1e6, 0, 0));

    Ptr<LeoPropagationLossModel> model = CreateObject<LeoPropagationLossModel> ();
    model->SetAttribute ("AtmosphericLoss", DoubleValue (0.1));
    model->SetAttribute ("FreeSpacePathLoss", DoubleValue (0.1));
    model->SetAttribute ("LinkMargin", DoubleValue (0.3));

    NS_TEST_ASSERT_MSG_EQ (model->CalcRxPower (1.0, a, b), 0.5, "Rx power is incorrect");
  }
};

class LeoPropagationTestSuite : public TestSuite
{
public:
  LeoPropagationTestSuite ();
};

LeoPropagationTestSuite::LeoPropagationTestSuite ()
  : TestSuite ("leo-propagation", UNIT)
{
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new LeoPropagationRxNoLosTestCase, TestCase::QUICK);
  AddTestCase (new LeoPropagationRxLosTestCase, TestCase::QUICK);
  AddTestCase (new LeoPropagationBadAngleTestCase, TestCase::QUICK);
  AddTestCase (new LeoPropagationLossTestCase, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static LeoPropagationTestSuite leoTestSuite;
