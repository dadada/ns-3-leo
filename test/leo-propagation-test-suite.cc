/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "math.h"
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

class LeoPropagationRxNoLosTestCase : public TestCase
{
public:
  LeoPropagationRxNoLosTestCase ()
    : TestCase ("Test RxNoLos for without LOS")
  {}
  virtual ~LeoPropagationRxNoLosTestCase ()
  {}

private:
  virtual void DoRun (void);
};

void
LeoPropagationRxNoLosTestCase::DoRun ()
{
  Ptr<ConstantPositionMobilityModel> a = CreateObject<ConstantPositionMobilityModel> ();
  a->SetPosition (Vector3D (1000000.0, 0, 0));
  Ptr<ConstantPositionMobilityModel> b = CreateObject<ConstantPositionMobilityModel> ();
  b->SetPosition (Vector3D (-1000000.0, 0, 0));

  Ptr<LeoPropagationLossModel> model = CreateObject<LeoPropagationLossModel> ();
  model->SetAttribute ("MaxDistance", DoubleValue (1000000.0));

  NS_TEST_ASSERT_MSG_EQ (model->CalcRxPower (1.0, a, b), 0.0, "0 Rx power without LOS");
}

class LeoPropagationRxLosTestCase : public TestCase
{
public:
  LeoPropagationRxLosTestCase ()
    : TestCase ("Test Rx with LOS")
  {}
  virtual ~LeoPropagationRxLosTestCase ()
  {}

private:
  virtual void DoRun (void);
};

void
LeoPropagationRxLosTestCase::DoRun ()
{
  Ptr<ConstantPositionMobilityModel> a = CreateObject<ConstantPositionMobilityModel> ();
  a->SetPosition (Vector3D (1000000.0, 0, 0));
  Ptr<ConstantPositionMobilityModel> b = CreateObject<ConstantPositionMobilityModel> ();
  b->SetPosition (Vector3D (1000001.0, 0, 0));

  Ptr<LeoPropagationLossModel> model = CreateObject<LeoPropagationLossModel> ();
  model->SetAttribute ("ElevationAngle", DoubleValue (10.0));
  model->SetAttribute ("AtmosphericLoss", DoubleValue (0.0));
  model->SetAttribute ("FreeSpacePathLoss", DoubleValue (0.0));
  model->SetAttribute ("LinkMargin", DoubleValue (0.0));

  NS_TEST_ASSERT_MSG_EQ (model->CalcRxPower (1.0, a, b), 1.0, "Tx same as Rx power without losses and LOS");
}

class LeoPropagationBadAngleTestCase : public TestCase
{
public:
  LeoPropagationBadAngleTestCase ()
    : TestCase ("Test Rx with too large elevation angle")
  {}
  virtual ~LeoPropagationBadAngleTestCase ()
  {}

private:
  virtual void DoRun (void);
};

void
LeoPropagationBadAngleTestCase::DoRun ()
{
  Ptr<ConstantPositionMobilityModel> a = CreateObject<ConstantPositionMobilityModel> ();
  a->SetPosition (Vector3D (1000000.0, 0, 0));
  Ptr<ConstantPositionMobilityModel> b = CreateObject<ConstantPositionMobilityModel> ();
  b->SetPosition (Vector3D (1000000.0, sin (M_PI / 9) * 1000000.0, 0));

  Ptr<LeoPropagationLossModel> model = CreateObject<LeoPropagationLossModel> ();
  model->SetAttribute ("ElevationAngle", DoubleValue (M_PI / 9));

  NS_TEST_ASSERT_MSG_EQ (model->CalcRxPower (1.0, a, b), 0.0, "Tx must be 0 if elevation is too large");
}

class LeoPropagationLossTestCase : public TestCase
{
public:
  LeoPropagationLossTestCase ()
    : TestCase ("Test Rx with some losses")
  {}
  virtual ~LeoPropagationLossTestCase ()
  {}

private:
  virtual void DoRun (void);
};

void
LeoPropagationLossTestCase::DoRun ()
{
  Ptr<ConstantPositionMobilityModel> a = CreateObject<ConstantPositionMobilityModel> ();
  a->SetPosition (Vector3D (1000000.0, 0, 0));
  Ptr<ConstantPositionMobilityModel> b = CreateObject<ConstantPositionMobilityModel> ();
  b->SetPosition (Vector3D (1000001.0, 0, 0));

  Ptr<LeoPropagationLossModel> model = CreateObject<LeoPropagationLossModel> ();
  model->SetAttribute ("AtmosphericLoss", DoubleValue (0.1));
  model->SetAttribute ("FreeSpacePathLoss", DoubleValue (0.1));
  model->SetAttribute ("LinkMargin", DoubleValue (0.3));

  NS_TEST_ASSERT_MSG_EQ (model->CalcRxPower (1.0, a, b), 0.5, "Tx must be smaller with some losses");
}

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
  AddTestCase (new LeoPropagationRxNoLosTestCase, TestCase::QUICK);
  AddTestCase (new LeoPropagationRxLosTestCase, TestCase::QUICK);
  AddTestCase (new LeoPropagationBadAngleTestCase, TestCase::QUICK);
  AddTestCase (new LeoPropagationLossTestCase, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static LeoPropagationTestSuite leoTestSuite;
