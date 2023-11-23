/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "math.h"

#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/log.h"

#include "leo-polar-position-allocator.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LeoPolarPositionAllocator);

NS_LOG_COMPONENT_DEFINE ("LeoPolarPositionAllocator");

LeoPolarPositionAllocator::LeoPolarPositionAllocator ()
  : m_latNum (1), m_lonNum (1), m_lat (0), m_lon (0)
{}

LeoPolarPositionAllocator::~LeoPolarPositionAllocator ()
{}

TypeId
LeoPolarPositionAllocator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LeoPolarPositionAllocator")
    .SetParent<PositionAllocator> ()
    .SetGroupName ("Leo")
    .AddConstructor<LeoPolarPositionAllocator> ()
    .AddAttribute ("LatNum",
                   "The number nodes along one latitude",
                   UintegerValue (10),
                   MakeUintegerAccessor (&LeoPolarPositionAllocator::m_latNum),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("LonNum",
                   "The number nodes along one longitude",
                   UintegerValue (10),
                   MakeUintegerAccessor (&LeoPolarPositionAllocator::m_lonNum),
                   MakeUintegerChecker<uint32_t> ())
  ;
  return tid;
}

int64_t
LeoPolarPositionAllocator::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);

  return -1;
}

Vector
LeoPolarPositionAllocator::GetNext () const
{
  NS_LOG_FUNCTION (this);

  double lat = m_lat * (M_PI / m_latNum);
  double lon = m_lon * (2 * M_PI / m_lonNum);
  Vector3D next = Vector3D (LEO_GND_RAD_EARTH * sin (lat) * cos (lon),
  			   LEO_GND_RAD_EARTH * sin (lat) * sin (lon),
  			   LEO_GND_RAD_EARTH * cos (lat));

  m_lat ++;
  if (m_lat > m_latNum)
    {
      m_lat = 0;
      m_lon = (m_lon+1) % m_lonNum;
    }

  NS_LOG_INFO ("Ground station at " << lat << ":" << lon << " -> " << next);

  return next;
}

};
