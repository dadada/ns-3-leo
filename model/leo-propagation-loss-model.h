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

#ifndef LEO_PROPAGATION_LOSS_MODEL_H
#define LEO_PROPAGATION_LOSS_MODEL_H

#include <ns3/object.h>
#include <ns3/propagation-loss-model.h>

#define LEO_PROP_EARTH_RAD 6.37101e6
#define LEO_SPEED_OF_LIGHT_IN_AIR 299702458

namespace ns3 {

class LeoPropagationLossModel : public PropagationLossModel
{
public:
  static TypeId GetTypeId (void);
  LeoPropagationLossModel ();
  virtual ~LeoPropagationLossModel ();

private:

  /**
   * Maximum elevation angle
   */
  double m_elevationAngle;

  /**
   * Atmospheric loss
   */
  double m_atmosphericLoss;

  /**
   * Free space path loss (FSPL)
   */
  double m_freeSpacePathLoss;

  /**
   * Link margin
   */
  double m_linkMargin;

  /**
   * Returns the Rx Power taking into account only the particular
   * PropagationLossModel.
   *
   * \param txPowerDbm current transmission power (in dBm)
   * \param a the mobility model of the source
   * \param b the mobility model of the destination
   * \returns the reception power after adding/multiplying propagation loss (in dBm)
   */
  virtual double DoCalcRxPower (double txPowerDbm,
                                Ptr<MobilityModel> a,
                                Ptr<MobilityModel> b) const;
  /**
   * Subclasses must implement this; those not using random variables
   * can return zero
   */
  virtual int64_t DoAssignStreams (int64_t stream);

  void SetElevationAngle (double angle);
  double GetElevationAngle () const;

  double GetCutoffDistance (const Ptr<MobilityModel> sat) const;
};

}

#endif /* SATELLITE_LEO_PROPAGATION_LOSS_MODEL_H */
