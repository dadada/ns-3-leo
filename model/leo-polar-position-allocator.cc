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
  : m_latStart (0), m_lonStart (0), m_latStop (0), m_lonStop (0), m_latNum (1), m_lonNum (1), m_lat (0), m_lon (0)
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
                   MakeDoubleAccessor (&LeoPolarPositionAllocator::SetLatStart,
                   		       &LeoPolarPositionAllocator::GetLatStart),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("LatStop",
                   "Stop at this longitude",
                   DoubleValue (90),
                   MakeDoubleAccessor (&LeoPolarPositionAllocator::SetLatStop,
                   		       &LeoPolarPositionAllocator::GetLatStop),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("LatNum",
                   "The number nodes along one latitude",
                   UintegerValue (10),
                   MakeUintegerAccessor (&LeoPolarPositionAllocator::m_latNum),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("LongStart",
                   "Start at this longitude",
                   DoubleValue (-180),
                   MakeDoubleAccessor (&LeoPolarPositionAllocator::SetLonStart,
                   		       &LeoPolarPositionAllocator::GetLonStart),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("LongStop",
                   "Stop at this longitude",
                   DoubleValue (180),
                   MakeDoubleAccessor (&LeoPolarPositionAllocator::SetLonStop,
                   		       &LeoPolarPositionAllocator::GetLonStop),
                   MakeDoubleChecker<double> ())
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

  double lat = m_lat * (M_PI / m_latNum) - (M_PI / 2);
  double lon = m_lon * (2 * M_PI / m_lonNum) - M_PI;
  Vector3D next = Vector3D (LEO_GND_RAD_EARTH * cos (lat) * cos (lon),
  			   LEO_GND_RAD_EARTH * cos (lat) * sin (lon),
  			   LEO_GND_RAD_EARTH * sin (lat));

  m_lat ++;
  if (m_lat > m_latNum)
    {
      m_lat = 0;
      m_lon = (m_lon+1) % m_lonNum;
    }

  NS_LOG_INFO ("Ground station at " << lat << ":" << lon << " -> " << next);

  return next;
}

double
LeoPolarPositionAllocator::GetLatStart () const
{
  NS_LOG_FUNCTION (this);
  return m_latStart * (180.0 / M_PI);
}

double
LeoPolarPositionAllocator::GetLonStart () const
{
  NS_LOG_FUNCTION (this);
  return m_lonStart * (180.0 / M_PI);
}

double
LeoPolarPositionAllocator::GetLatStop () const
{
  NS_LOG_FUNCTION (this);
  return m_latStop * (180.0 / M_PI);
}

double
LeoPolarPositionAllocator::GetLonStop () const
{
  NS_LOG_FUNCTION (this);
  return m_lonStop * (180.0 / M_PI);
}

void
LeoPolarPositionAllocator::SetLatStart (double lat)
{
  NS_LOG_FUNCTION (this << lat);
  m_latStart = (lat / 180.0) * M_PI;
}

void
LeoPolarPositionAllocator::SetLonStart (double lon)
{
  NS_LOG_FUNCTION (this << lon);
  m_lonStart = (lon / 180.0) * M_PI;
}

void
LeoPolarPositionAllocator::SetLatStop (double lat)
{
  NS_LOG_FUNCTION (this << lat);
  m_latStop = (lat / 180.0) * M_PI;
}

void
LeoPolarPositionAllocator::SetLonStop (double lon)
{
  NS_LOG_FUNCTION (this << lon);
  m_lonStop = (lon / 180.0) * M_PI;
}

};
