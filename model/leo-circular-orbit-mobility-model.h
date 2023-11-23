/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef LEO_CIRCULAR_ORBIT_MOBILITY_MODEL_H
#define LEO_CIRCULAR_ORBIT_MOBILITY_MODEL_H

#include "ns3/vector.h"
#include "ns3/object.h"
#include "ns3/log.h"
#include "ns3/mobility-model.h"
#include "ns3/nstime.h"

#define LEO_EARTH_RAD_KM 6371.0090
#define LEO_EARTH_GM_KM_E10 39.8600436

namespace ns3 {

/**
 * \ingroup leo
 * \brief Keep track of the orbital postion and velocity of a satellite.
 *
 * This uses simple circular orbits based on the inclination of the orbital
 * plane and the height of the satellite.
 */
class LeoCircularOrbitMobilityModel : public MobilityModel
{
public:
  /**
   * Register this type with the TypeId system.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  LeoCircularOrbitMobilityModel ();
  virtual ~LeoCircularOrbitMobilityModel ();

  /**
   * km/s
   */
  double GetSpeed () const;

  double GetAltitude () const;
  void SetAltitude (double h);

  double GetInclination () const;
  void SetInclination (double incl);

private:

  /**
   * Orbit height in m
   */
  double m_orbitHeight;

  /**
   * Inclination in rad
   */
  double m_inclination;

  /**
   * Latitude in rad
   */
  double m_latitude;

  /**
   * Offset on the orbital plane in rad
   */
  double m_offset;

  /**
   * Current position
   */
  Vector3D m_position;

  /**
   * Time precision for positions
   */
  Time m_precision;

  /**
   * \return the current position.
   */
  virtual Vector DoGetPosition (void) const;
  /**
   * \param position the position to set.
   */
  virtual void DoSetPosition (const Vector &position);
  /**
   * \return the current velocity.
   */
  virtual Vector DoGetVelocity (void) const;

  /**
   * Get the normal vector of the orbital plane
   */
  Vector3D PlaneNorm () const;

  double GetProgress (Time t) const;

  /**
   * Advances a satellite by a degrees on the orbital plane
   */
  Vector3D RotatePlane (double a, const Vector3D &x) const;

  /**
   * Calculate the position at time
   *
   * \param t time
   */
  Vector CalcPosition (Time t) const;

  /**
   * Calc the latitude depending on simulation time inside ITRF coordinate
   * system
   */
  double CalcLatitude () const;

  Vector Update ();
};

}

#endif
