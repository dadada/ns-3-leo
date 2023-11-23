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
                   DoubleValue (4000.0),
                   MakeDoubleAccessor (&LeoPropagationLossModel::SetCutoffDistance,
				       &LeoPropagationLossModel::GetCutoffDistance),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("ElevationAngle",
                   "Cut-off angle for signal propagation",
                   DoubleValue (40.0),
                   MakeDoubleAccessor (&LeoPropagationLossModel::SetElevationAngle,
				       &LeoPropagationLossModel::GetElevationAngle),
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
  Vector3D x = a->GetPosition ();
  Vector3D y = b->GetPosition ();

  Vector3D pa, pb;
  if (x.GetLength () < y.GetLength ())
    {
      pa = x - y;
      pb = y;
      NS_LOG_DEBUG ("LEO ground -> space");
    }
  else
    {
      pa = y - x;
      pb = x;
      NS_LOG_DEBUG ("LEO space -> ground");
    }

  double prod = (pa.x*-pb.x) + (pa.y*-pb.y) + (pa.z*-pb.z);
  double norm = pa.GetLength () * pb.GetLength ();

  return acos (prod / norm);
}

void
LeoPropagationLossModel::SetElevationAngle (double angle)
{
  m_elevationAngle = (90 - angle) * (M_PI/180.0);
}

double
LeoPropagationLossModel::GetElevationAngle () const
{
  return 90 - (m_elevationAngle * (180.0/M_PI));
}

void
LeoPropagationLossModel::SetCutoffDistance (double d)
{
  m_cutoffDistance = d * 1000.0;
}

double
LeoPropagationLossModel::GetCutoffDistance () const
{
  return m_cutoffDistance / 1000.0;
}

double
LeoPropagationLossModel::DoCalcRxPower (double txPowerDbm,
                                        Ptr<MobilityModel> a,
                                        Ptr<MobilityModel> b) const
{
  double distance = a->GetDistanceFrom (b);

  if (distance > m_cutoffDistance)
    {
      NS_LOG_DEBUG ("LEO DROP distance: a=" << a->GetPosition () << " b=" << b->GetPosition () << " m_cutOff="<<m_cutoffDistance<<" dist=" << distance);

      return -1000.0;
    }

  double angle = GetAngle (a, b);
  if (angle > m_elevationAngle)
    {
      NS_LOG_DEBUG ("LEO DROP angle: a=" << a->GetPosition () << " b=" << b->GetPosition () << " m_cutOff="<<m_cutoffDistance<<" m_angle="<<m_elevationAngle<<" dist=" << distance << "angle=" << angle);
      return -1000.0;
    }

  // txPowerDbm includes tx antenna gain and losses
  // receiver loss and gain added at net device
  // P_{RX} = P_{TX} + G_{TX} - L_{TX} - L_{FS} - L_M + G_{RX} - L_{RX}
  double rxc = txPowerDbm - m_atmosphericLoss - m_freeSpacePathLoss - m_linkMargin;
  NS_LOG_DEBUG ("LEO TRANSMIT: angle=" << angle <<";distance=" << distance << ";rxc=" << rxc);

  return rxc;
}

int64_t
LeoPropagationLossModel::DoAssignStreams (int64_t stream)
{
  return 0;
}

};
