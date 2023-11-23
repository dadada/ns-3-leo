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

double
IslPropagationLossModel::DoCalcRxPower (double txPowerDbm,
                                           Ptr<MobilityModel> a,
                                           Ptr<MobilityModel> b) const
{
  //Vector aPos = a->GetPosition ();
  //Vector bPos = b->GetPosition ();

  // TODO perform line-earth intersection (ray tracing)

  double rxc = 0;//-m_variable->GetValue ();
  NS_LOG_DEBUG ("attenuation coefficient="<<rxc<<"Db");
  return txPowerDbm + rxc;
}

int64_t
IslPropagationLossModel::DoAssignStreams (int64_t stream)
{
  return 0;
}

};
