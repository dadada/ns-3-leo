/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "math.h"

#include "ns3/integer.h"
#include "leo-circular-orbit-position-allocator.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LeoCircularOrbitAllocator);

LeoCircularOrbitAllocator::LeoCircularOrbitAllocator ()
  : m_lastOrbit (0), m_lastSatellite (0)
{}

LeoCircularOrbitAllocator::~LeoCircularOrbitAllocator ()
{}

TypeId
LeoCircularOrbitAllocator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LeoCircularOrbitPostionAllocator")
    .SetParent<PositionAllocator> ()
    .SetGroupName ("Leo")
    .AddConstructor<LeoCircularOrbitAllocator> ()
    .AddAttribute ("NumOrbits",
                   "The number of orbits",
                   IntegerValue (1),
                   MakeIntegerAccessor (&LeoCircularOrbitAllocator::m_numOrbits),
                   MakeIntegerChecker<uint16_t> ())
    .AddAttribute ("NumSatellites",
                   "The number of satellites per orbit",
                   IntegerValue (1),
                   MakeIntegerAccessor (&LeoCircularOrbitAllocator::m_numSatellites),
                   MakeIntegerChecker<uint16_t> ())
  ;
  return tid;
}

int64_t
LeoCircularOrbitAllocator::AssignStreams (int64_t stream)
{
  return -1;
}

Vector
LeoCircularOrbitAllocator::GetNext () const
{
  Vector next = Vector (2 * M_PI * (m_lastOrbit / (double) m_numOrbits),
	  	 2 * M_PI * (m_lastSatellite / (double) m_numSatellites),
	  	 0);

  if (m_lastSatellite + 1 == m_numSatellites)
    {
      m_lastOrbit = (m_lastOrbit + 1) % m_numOrbits;
    }
  m_lastSatellite = (m_lastSatellite + 1) % m_numSatellites;

  return next;
}

};
