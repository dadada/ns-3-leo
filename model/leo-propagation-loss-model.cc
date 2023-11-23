/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/mobility-model.h"

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
LeoPropagationLossModel::DoCalcRxPower (double txPowerDbm,
                                           Ptr<MobilityModel> a,
                                           Ptr<MobilityModel> b) const
{
  //Vector aPos = a->GetPosition ();
  //Vector bPos = b->GetPosition ();

  // TODO perform line-earth intersection (ray tracing)
  // TODO check angle between satellite and ground-station
  // primitivec cut-of at 1000 km
  if (a->GetDistanceFrom (b) > 1000000.0)
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
