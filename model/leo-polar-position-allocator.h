/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author: Tim Schubert <ns-3-leo@timschubert.net>
 */

#ifndef LEO_POLAR_POSITION_ALLOCATOR_H
#define LEO_POLAR_POSITION_ALLOCATOR_H

#include "ns3/position-allocator.h"

#define LEO_GND_RAD_EARTH 6.371e6

namespace ns3 {

/**
 * \brief Allocate pairs of latitude and longitude.
 */
class LeoPolarPositionAllocator : public PositionAllocator
{
public:
  /**
   * Register this type with the TypeId system.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();

  LeoPolarPositionAllocator ();
  virtual ~LeoPolarPositionAllocator ();

  virtual Vector GetNext (void) const;
  virtual int64_t AssignStreams (int64_t stream);

private:
  uint32_t m_latNum;
  uint32_t m_lonNum;

  mutable uint32_t m_lat;
  mutable uint32_t m_lon;
};

};

#endif
