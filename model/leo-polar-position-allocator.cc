/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "math.h"

#include "ns3/double.h"

#include "leo-polar-position-allocator.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LeoPolarPositionAllocator);

LeoPolarPositionAllocator::LeoPolarPositionAllocator ()
  : m_latStart (0), m_lonStart (0), m_latEnd (0), m_lonEnd (0), m_step (5), m_lat (-1000.0), m_lon (-1000.0)
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
    .AddAttribute ("LatStart",
                   "Start at this latitude",
                   DoubleValue (-90),
                   MakeDoubleAccessor (&LeoPolarPositionAllocator::m_latStart),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("LatStop",
                   "Stop at this longitude",
                   DoubleValue (90),
                   MakeDoubleAccessor (&LeoPolarPositionAllocator::m_latEnd),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("LongStart",
                   "Start at this longitude",
                   DoubleValue (-180),
                   MakeDoubleAccessor (&LeoPolarPositionAllocator::m_lonStart),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("LongStop",
                   "Stop at this longitude",
                   DoubleValue (180),
                   MakeDoubleAccessor (&LeoPolarPositionAllocator::m_lonEnd),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Step",
                   "The degrees inbetween neighboring locations",
                   DoubleValue (5),
                   MakeDoubleAccessor (&LeoPolarPositionAllocator::m_step),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

int64_t
LeoPolarPositionAllocator::AssignStreams (int64_t stream)
{
  return -1;
}

Vector
LeoPolarPositionAllocator::GetNext () const
{
  m_lat = std::max (m_latStart, m_lat);
  m_lon = std::max (m_lonStart, m_lon);

  double lat = m_lat * (M_PI / 90);
  double lon = m_lon * (M_PI / 180);
  Vector3D next = Vector3D (LEO_GND_RAD_EARTH * sin (lat) * cos (lon),
  			   LEO_GND_RAD_EARTH * sin (lat) * sin (lon),
  			   LEO_GND_RAD_EARTH * cos (lat));

  m_lat = m_lat + m_step;
  if (m_lat > m_latEnd)
    {
      m_lat = m_latStart;
      m_lon += m_step;
      if (m_lon > m_lonEnd)
      	{
      	  m_lon = m_lonStart;
      	}
    }

  return next;
}

};
