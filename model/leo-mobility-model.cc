/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007, 2008 University of Washington
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
 */

#include "leo-mobility-model.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LeoMobilityModel");

NS_OBJECT_ENSURE_REGISTERED (LeoMobilityModel);

TypeId
LeoMobilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LeoMobilityModel")
    .SetParent<MobilityModel> ()
    .SetGroupName ("Leo")
    .AddConstructor<LeoMobilityModel> ()
  ;
  return tid;
}

LeoMobilityModel::~LeoMobilityModel ()
{
}

Vector
LeoMobilityModel::DoGetPosition (void) const
{
  // TODO
  return Vector();
}

void
LeoMobilityModel::DoSetPosition (const Vector &position)
{
  // TODO
}

Vector
LeoMobilityModel::DoGetVelocity (void) const
{
  // TODO
  return Vector();
}

int64_t
LeoMobilityModel::DoAssignStreams (int64_t start)
{
  // TODO
  return 0;
}

};
