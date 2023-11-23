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

#ifndef LEO_CIRCULAR_ORBIT_MOBILITY_MODEL_H
#define LEO_CIRCULAR_ORBIT_MOBILITY_MODEL_H

#include "ns3/vector.h"
#include "ns3/object.h"
#include "ns3/log.h"
#include "ns3/mobility-model.h"
#include "ns3/nstime.h"

/**
 * \file
 * \ingroup leo
 *
 * Declaration of LeoCircularOrbitMobilityModel
 */

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
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  /// constructor
  LeoCircularOrbitMobilityModel ();
  /// destructor
  virtual ~LeoCircularOrbitMobilityModel ();

  /**
   * \brief Gets the speed of the node
   * \return the speed in m/s
   */
  double GetSpeed () const;

  /**
   * \brief Gets the altitude
   * \return the altitude
   */
  double GetAltitude () const;
  /**
   * \brief Sets the altitude
   * \param the altitude
   */
  void SetAltitude (double h);

  /**
   * \brief Gets the inclination
   * \return the inclination
   */
  double GetInclination () const;

  /**
   * \brief Sets the inclination
   * \param the inclination
   */
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
   * Longitudinal offset in rad
   */
  double m_longitude;

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
   * \brief Get the normal vector of the orbital plane
   */
  Vector3D PlaneNorm () const;

  /**
   * \brief Gets the distance the satellite has progressed from its original
   * position at time t in rad
   *
   * \param t a point in time
   * \return distance in rad
   */
  double GetProgress (Time t) const;

  /**
   * \brief Advances a satellite by a degrees inside the orbital plane
   * \param a angle by which to rotate
   * \param x vector to rotate
   * \return rotated vector
   */
  Vector3D RotatePlane (double a, const Vector3D &x) const;

  /**
   * \brief Calculate the position at time t
   * \param t time
   * \return position at time t
   */
  Vector CalcPosition (Time t) const;

  /**
   * \brief Calc the latitude depending on simulation time inside ITRF coordinate
   * system
   *
   * \return latitude
   */
  double CalcLatitude () const;

  /**
   * \brief Update the internal position of the mobility model
   * \return position that will be returned upon next call to DoGetPosition
   */
  Vector Update ();
};

}

#endif
