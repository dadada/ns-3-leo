/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 * Contributions: Timo Bingmann <timo.bingmann@student.kit.edu>
 * Contributions: Gary Pei <guangyu.pei@boeing.com> for fixed RSS
 * Contributions: Tom Hewer <tomhewer@mac.com> for two ray ground model
 *                Pavel Boyko <boyko@iitp.ru> for matrix
 */

#include "ns3/log.h"
#include "ns3/mobility-model.h"
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
  ;
  return tid;
}

IslPropagationLossModel::IslPropagationLossModel ()
{
}

IslPropagationLossModel::~IslPropagationLossModel ()
{
}


const double
IslPropagationLossModel::EARTH_RAD_E6 = 6.371;

bool
IslPropagationLossModel::GetLos (Ptr<MobilityModel> a, Ptr<MobilityModel> b)
{
  // origin of LOS
  Vector3D o = a->GetPosition ();
  o = Vector3D (o.x / 1e6, o.y / 1e6, o.z / 1e6);
  double ol = o.GetLength ();

  // second point of LOS
  Vector3D bp = b->GetPosition ();
  bp = Vector3D (bp.x / 1e6, bp.y / 1e6, bp.z / 1e6);

  // unit vector
  Vector3D u = Vector3D (o.x-bp.x, o.y-bp.y, o.z-bp.z);
  u = Vector3D (u.x / u.GetLength (), u.y / u.GetLength (), u.z / u.GetLength ());

  // center point of sphere is 0
  double uo = (u.x*o.x) + (u.y*o.y) + (u.z*o.z);
  double delta = (uo*uo) - (ol*ol - (EARTH_RAD_E6*EARTH_RAD_E6));

  //NS_LOG_DEBUG ("a_pos="<<o<<";b_pos"<<bp<<";delta="<<delta);

  return delta < 0;
}

double
IslPropagationLossModel::DoCalcRxPower (double txPowerDbm,
                                        Ptr<MobilityModel> a,
                                        Ptr<MobilityModel> b) const
{
  if (!GetLos (a, b))
    {
      return 0.0;
    }

  NS_LOG_INFO ("LOS;"<<a->GetPosition ()<<";"<<b->GetPosition ());

  return txPowerDbm;
}

int64_t
IslPropagationLossModel::DoAssignStreams (int64_t stream)
{
  return 0;
}

};
