/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef LEO_CIRCULAR_ORBIT_HELPER_H
#define LEO_CIRCULAR_ORBIT_HELPER_H

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
