/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
 *
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 * Contributions: Timo Bingmann <timo.bingmann@student.kit.edu>
 * Contributions: Gary Pei <guangyu.pei@boeing.com> for fixed RSS
 * Contributions: Tom Hewer <tomhewer@mac.com> for two ray ground model
 *                Pavel Boyko <boyko@iitp.ru> for matrix
 */

#ifndef ISL_PROPAGATION_LOSS_MODEL_H
#define ISL_PROPAGATION_LOSS_MODEL_H

#include <ns3/object.h>
#include <ns3/propagation-loss-model.h>

namespace ns3 {

class IslPropagationLossModel : public PropagationLossModel
{
public:
  static const double EARTH_RAD;

  static TypeId GetTypeId (void);
  IslPropagationLossModel ();
  virtual ~IslPropagationLossModel ();

  /**
   * true if there is at least one intersection of ISL line-of-sight with earth
   * (LOS is blocked by earth)
   *
   * Assumes earth is sperical.
   *
   * \param a first node
   * \param b second node
   */
  static bool GetLos (Ptr<MobilityModel> a, Ptr<MobilityModel> b);
private:
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
};

}

#endif /* SATELLITE_ISL_PROPAGATION_LOSS_MODEL_H */
