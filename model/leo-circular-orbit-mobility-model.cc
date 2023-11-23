/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "math.h"

#include "ns3/double.h"
#include "ns3/simulator.h"

#include "leo-circular-orbit-mobility-model.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LeoCircularOrbitMobilityModel");

NS_OBJECT_ENSURE_REGISTERED (LeoCircularOrbitMobilityModel);

TypeId
LeoCircularOrbitMobilityModel::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::LeoCircularOrbitMobilityModel")
    .SetParent<MobilityModel> ()
    .SetGroupName ("Leo")
    .AddAttribute ("Altitude",
                   "A height from the earth's surface in meters",
                   DoubleValue (1000.0),
                   MakeDoubleAccessor (&LeoCircularOrbitMobilityModel::SetAltitude,
                   		       &LeoCircularOrbitMobilityModel::GetAltitude),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Inclination",
                   "The inclination of the orbital plane in degrees",
                   DoubleValue (10.0),
                   MakeDoubleAccessor (&LeoCircularOrbitMobilityModel::SetInclination,
                   		       &LeoCircularOrbitMobilityModel::GetInclination),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Latitude",
                   "The latitude at which the orital plane intersects the equatorial plane in degrees",
                   DoubleValue (10.0),
                   MakeDoubleAccessor (&LeoCircularOrbitMobilityModel::SetLatitude,
                   		       &LeoCircularOrbitMobilityModel::GetLatitude),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Offset",
                   "The relative offset of the satellite along the orbital plane from the equatorial plane in degrees",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&LeoCircularOrbitMobilityModel::SetOffset,
                   		       &LeoCircularOrbitMobilityModel::GetOffset),
                   MakeDoubleChecker<double> ())
    ;
  return tid;
}

LeoCircularOrbitMobilityModel::LeoCircularOrbitMobilityModel() : MobilityModel ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

LeoCircularOrbitMobilityModel::~LeoCircularOrbitMobilityModel()
{
}

Vector3D
CrossProduct (const Vector3D &l, const Vector3D &r)
{
  return Vector3D (l.y * r.z - l.z * r.y,
		   l.z * r.x - l.x * r.z,
		   l.x * r.y - l.y * r.x);
}

Vector3D
Product (const double &l, const Vector3D &r)
{
  return Vector3D (l * r.x,
		   l * r.y,
		   l * r.z);
}

double
DotProduct (const Vector3D &l, const Vector3D &r)
{
  return (l.x* r.x) + (l.y*r.y) + (l.z*r.z);
}

double
LeoCircularOrbitMobilityModel::GetSpeed () const
{
   return sqrt (LEO_EARTH_GM / m_orbitHeight);
}

Vector
LeoCircularOrbitMobilityModel::DoGetVelocity () const
{
  Vector3D heading = CrossProduct (PlaneNorm (), DoGetPosition ());
  // TODO
  return Product (GetSpeed (), heading);
}

Vector3D
LeoCircularOrbitMobilityModel::PlaneNorm () const
{
  return Vector3D (sin (-m_inclination) * cos (m_latitude),
  		   sin (-m_inclination) * sin (m_latitude),
  		   cos (m_inclination));
}

double
LeoCircularOrbitMobilityModel::GetProgress (Time t) const
{
  // TODO use nanos or ms instead? does it give higher precision?
  return 2 * M_PI * ((GetSpeed () * t.GetSeconds ()) / LEO_EARTH_RAD_M);
}

Vector3D
LeoCircularOrbitMobilityModel::RotatePlane (double a, const Vector3D &x) const
{
  Vector3D n = PlaneNorm ();

  // TODO optimize?
  return Product (DotProduct (n, x), n)
    + Product (cos (a), CrossProduct (CrossProduct (n, x), n))
    + Product (sin (a), CrossProduct (n, x));
}

Vector
LeoCircularOrbitMobilityModel::DoGetPosition (void) const
{
  Vector3D x = Product (m_orbitHeight, Vector3D (cos (m_inclination) * cos (m_latitude),
  			       cos (m_inclination) * sin (m_latitude),
  			       sin (m_inclination)));

  return RotatePlane (GetProgress (Simulator::Now ()), x);
}

void
LeoCircularOrbitMobilityModel::DoSetPosition (const Vector &position)
{
  NS_ASSERT_MSG (false, "Can not set position of satellite on circular orbit");
}

double LeoCircularOrbitMobilityModel::GetAltitude () const
{
  return m_orbitHeight - LEO_EARTH_RAD_M;
}

void LeoCircularOrbitMobilityModel::SetAltitude (double h)
{
  m_orbitHeight = LEO_EARTH_RAD_M + h;
  m_plane = PlaneNorm ();
}

double LeoCircularOrbitMobilityModel::GetLatitude () const
{
  return m_latitude / M_PI * 180.0;
}

void LeoCircularOrbitMobilityModel::SetLatitude (double lat)
{
  m_latitude = lat / 180 * M_PI;
  m_plane = PlaneNorm ();
}

double LeoCircularOrbitMobilityModel::GetOffset () const
{
  return (m_offset / M_PI) * 180;
}

void LeoCircularOrbitMobilityModel::SetOffset (double off)
{
  m_offset = (off / 180) * M_PI;
}

double LeoCircularOrbitMobilityModel::GetInclination () const
{
  return (m_inclination / M_PI) * 180.0;
}

void LeoCircularOrbitMobilityModel::SetInclination (double incl)
{
  NS_ASSERT_MSG (incl != 0.0, "Plane must not be orthogonal to axis");
  m_inclination = (incl / 180) * M_PI;
  m_plane = PlaneNorm ();
}

};
