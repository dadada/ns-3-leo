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

#ifndef LEO_CIRCULAR_ORBIT_POSITION_ALLOCATOR_H
#define LEO_CIRCULAR_ORBIT_POSITION_ALLOCATOR_H

#include "ns3/position-allocator.h"

namespace ns3 {

/**
 * \brief Allocate pairs of latitude and longitude (offset within orbit) for
 * use in LeoCircularOrbitMobilityModel
 */
class LeoCircularOrbitAllocator : public PositionAllocator
{
public:
  /**
   * Register this type with the TypeId system.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();

  LeoCircularOrbitAllocator ();
  virtual ~LeoCircularOrbitAllocator ();

  virtual Vector GetNext (void) const;
  virtual int64_t AssignStreams (int64_t stream);

private:
  uint64_t m_numOrbits;
  uint64_t m_numSatellites;

  mutable uint64_t m_lastOrbit;
  mutable uint64_t m_lastSatellite;
};

};

#endif
