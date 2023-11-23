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
                   DoubleValue (100000.0),
                   MakeDoubleAccessor (&LeoPropagationLossModel::m_cutoffDistance),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("ElevationAngle",
                   "Cut-off angle for signal propagation",
                   DoubleValue (M_PI / 9),
                   MakeDoubleAccessor (&LeoPropagationLossModel::m_elevationAngle),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("AtmosphericLoss",
                   "Atmospheric loss due to attenuation in dB",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&LeoPropagationLossModel::m_atmosphericLoss),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("FreeSpacePathLoss",
                   "Free space path loss in dB",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&LeoPropagationLossModel::m_freeSpacePathLoss),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("LinkMargin",
                   "Link margin in dB",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&LeoPropagationLossModel::m_linkMargin),
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
  if (a->GetDistanceFrom (b) > m_cutoffDistance || GetAngle (a, b) > m_elevationAngle / 2.0)
    {
      return 0.0;
    }

  // txPowerDbm includes tx antenna gain and losses
  // receiver loss and gain added at net device
  // P_{RX} = P_{TX} + G_{TX} - L_{TX} - L_{FS} - L_M + G_{RX} - L_{RX}
  double rxc = txPowerDbm - m_atmosphericLoss - m_freeSpacePathLoss - m_linkMargin;

  return rxc;
}

int64_t
LeoPropagationLossModel::DoAssignStreams (int64_t stream)
{
  return 0;
}

};
