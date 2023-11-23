/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/mobility-model.h"
#include "ns3/double.h"
#include "math.h"

#include "isl-propagation-loss-model.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("IslPropagationLossModel");

NS_OBJECT_ENSURE_REGISTERED (IslPropagationLossModel);

TypeId
IslPropagationLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::IslPropagationLossModel")
    .SetParent<PropagationLossModel> ()
    .SetGroupName ("Leo")
    .AddConstructor<IslPropagationLossModel> ()
    .AddAttribute ("MaxDistance",
                   "Cut-off distance for signal propagation",
                   DoubleValue (2000.0),
                   MakeDoubleAccessor (&IslPropagationLossModel::SetCutoffDistance,
				       &IslPropagationLossModel::GetCutoffDistance),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

IslPropagationLossModel::IslPropagationLossModel ()
{
}

IslPropagationLossModel::~IslPropagationLossModel ()
{
}

bool
IslPropagationLossModel::GetLos (Ptr<MobilityModel> moda, Ptr<MobilityModel> modb)
{
  // TODO get max distance with line-sphere intersection

  // origin of LOS
  Vector3D oc = moda->GetPosition ();
  Vector3D bp = modb->GetPosition ();

  // direction unit vector
  Vector3D u = Vector3D (bp.x - oc.x, bp.y - oc.y, bp.z - oc.z);
  u = Vector3D (u.x / u.GetLength (), u.y / u.GetLength (), u.z / u.GetLength ());

  double a = u.x*u.x + u.y*u.y + u.z*u.z;
  double b = 2.0 * (oc.x*u.x + oc.y*u.y + oc.z*u.z);
  double c = (oc.x*oc.x + oc.y*oc.y + oc.z*oc.z) - (LEO_EARTH_RAD*LEO_EARTH_RAD);
  double discriminant = b*b - 4*a*c;

  NS_LOG_DEBUG ("a_pos="<<moda->GetPosition ()<<";b_pos"<<modb->GetPosition ()
  		<<";u="<<u
  		<<";a="<<a
  		<<";b="<<b
  		<<";c="<<c
  		<<";disc="<<discriminant);

  return discriminant < 0;
}

double
IslPropagationLossModel::DoCalcRxPower (double txPowerDbm,
                                        Ptr<MobilityModel> a,
                                        Ptr<MobilityModel> b) const
{
  if (a->GetDistanceFrom (b) > m_cutoffDistance || !GetLos (a, b))
    {
      NS_LOG_DEBUG ("DROP;"<<a->GetPosition ()<<";"<<b->GetPosition ());
      return -1000.0;
    }

  NS_LOG_DEBUG ("LOS;"<<a->GetPosition ()<<";"<<b->GetPosition ());

  return txPowerDbm;
}

int64_t
IslPropagationLossModel::DoAssignStreams (int64_t stream)
{
  return 0;
}

void
IslPropagationLossModel::SetCutoffDistance (double d)
{
  m_cutoffDistance = d * 1000.0;
}

double
IslPropagationLossModel::GetCutoffDistance () const
{
  return m_cutoffDistance / 1000.0;
}
};
