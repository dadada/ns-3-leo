/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <math.h>

#include "ns3/log.h"
#include "ns3/mobility-model.h"
#include "ns3/double.h"

#include "leo-propagation-loss-model.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LeoPropagationLossModel");

NS_OBJECT_ENSURE_REGISTERED (LeoPropagationLossModel);

TypeId
LeoPropagationLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LeoPropagationLossModel")
    .SetParent<PropagationLossModel> ()
    .SetGroupName ("Leo")
    .AddConstructor<LeoPropagationLossModel> ()
    .AddAttribute ("MaxDistance",
                   "Cut-off distance for signal propagation",
                   DoubleValue (1000000.0),
                   MakeDoubleAccessor (&LeoPropagationLossModel::m_cutoffDistance),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxAngle",
                   "Cut-off angle for signal propagation",
                   DoubleValue (20.0),
                   MakeDoubleAccessor (&LeoPropagationLossModel::m_cutoffAngle),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

LeoPropagationLossModel::LeoPropagationLossModel ()
{
}

LeoPropagationLossModel::~LeoPropagationLossModel ()
{
}

double
LeoPropagationLossModel::GetAngle (Ptr<MobilityModel> a, Ptr<MobilityModel> b)
{
  Vector3D pa = a->GetPosition () - b->GetPosition ();
  Vector3D pb = b->GetPosition ();
  pb = Vector3D (-pb.x, -pb.y, -pb.z);

  double prod = abs ((pa.x * pb.x) + (pa.y * pb.y) + (pa.z * pb.z));
  double norm = pb.GetLength () * pa.GetLength ();

  return acos (prod / norm);
}

double
LeoPropagationLossModel::DoCalcRxPower (double txPowerDbm,
                                        Ptr<MobilityModel> a,
                                        Ptr<MobilityModel> b) const
{
  // TODO create attributes for max distance and angle
  if (a->GetDistanceFrom (b) > m_cutoffDistance && GetAngle (a, b) > m_cutoffAngle)
    {
      return 0;
    }

  double rxc = 0;//-m_variable->GetValue ();
  //NS_LOG_DEBUG ("attenuation coefficient="<<rxc<<"Db");
  return txPowerDbm + rxc;
}

int64_t
LeoPropagationLossModel::DoAssignStreams (int64_t stream)
{
  return 0;
}

};
