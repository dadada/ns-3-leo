/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

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
