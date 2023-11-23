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
    .AddConstructor<LeoCircularOrbitMobilityModel> ()
    .AddAttribute ("Altitude",
                   "A height from the earth's surface in kilometers",
                   DoubleValue (1000.0),
                   MakeDoubleAccessor (&LeoCircularOrbitMobilityModel::SetAltitude,
                   		       &LeoCircularOrbitMobilityModel::GetAltitude),
                   MakeDoubleChecker<double> ())
    // TODO check value limits
    .AddAttribute ("Inclination",
                   "The inclination of the orbital plane in degrees",
                   DoubleValue (10.0),
                   MakeDoubleAccessor (&LeoCircularOrbitMobilityModel::SetInclination,
                   		       &LeoCircularOrbitMobilityModel::GetInclination),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("Precision",
                   "The time precision with which to compute position updates. 0 means arbitrary precision",
                   TimeValue (Seconds (1)),
                   MakeTimeAccessor (&LeoCircularOrbitMobilityModel::m_precision),
                   MakeTimeChecker ())
    ;
  return tid;
}

LeoCircularOrbitMobilityModel::LeoCircularOrbitMobilityModel() : MobilityModel (), m_latitude (0.0), m_offset (0.0), m_position ()
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
   return sqrt (LEO_EARTH_GM_KM_E10 / m_orbitHeight) * 1e5;
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
  return (2 * M_PI * ((GetSpeed () * t.GetSeconds ()) / LEO_EARTH_RAD_M)) + m_offset;
}

Vector3D
LeoCircularOrbitMobilityModel::RotatePlane (double a, const Vector3D &x) const
{
  Vector3D n = m_plane;

  return Product (DotProduct (n, x), n)
    + Product (cos (a), CrossProduct (CrossProduct (n, x), n))
    + Product (sin (a), CrossProduct (n, x));
}

Vector
LeoCircularOrbitMobilityModel::CalcPosition (Time t) const
{
  Vector3D x = Product (m_orbitHeight, Vector3D (cos (m_inclination) * cos (m_latitude),
  			       cos (m_inclination) * sin (m_latitude),
  			       sin (m_inclination)));

  return RotatePlane (GetProgress (t), x);
}

Vector LeoCircularOrbitMobilityModel::Update ()
{
  m_plane = PlaneNorm ();

  m_position = CalcPosition (Simulator::Now ());
  NotifyCourseChange ();

  if (m_precision > Seconds (0))
    {
      Simulator::Schedule (m_precision, &LeoCircularOrbitMobilityModel::Update, this);
    }

  return m_position;
}

Vector
LeoCircularOrbitMobilityModel::DoGetPosition (void) const
{
  if (m_precision == Time (0))
    {
      // Notice: NotifyCourseChange () will not be called
      return CalcPosition (Simulator::Now ());
    }
  return m_position;
}

void
LeoCircularOrbitMobilityModel::DoSetPosition (const Vector &position)
{
  // use first element of position vector as latitude, second for longitude
  // this works nicely with MobilityHelper and GetPostion will still get the
  // correct position, but be aware that it will not be the same as supplied to
  // SetPostion
  m_latitude = position.x;
  m_offset = position.y;
  Update ();
}

double LeoCircularOrbitMobilityModel::GetAltitude () const
{
  return m_orbitHeight - LEO_EARTH_RAD_M;
}

void LeoCircularOrbitMobilityModel::SetAltitude (double h)
{
  m_orbitHeight = LEO_EARTH_RAD_M + h;
  Update ();
}

double LeoCircularOrbitMobilityModel::GetInclination () const
{
  return (m_inclination / M_PI) * 180.0;
}

void LeoCircularOrbitMobilityModel::SetInclination (double incl)
{
  NS_ASSERT_MSG (incl != 0.0, "Plane must not be orthogonal to axis");
  m_inclination = (incl / 180) * M_PI;
  Update ();
}

};
