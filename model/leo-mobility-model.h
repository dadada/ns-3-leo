/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007, 2008 University of Washington
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
 */

#ifndef LEO_MOBILITY_MODEL_H
#define LEO_MOBILITY_MODEL_H
#include "ns3/log.h"
#include "ns3/mobility-model.h"

namespace ns3 {

class LeoMobilityModel : public MobilityModel {
public:
  static TypeId GetTypeId (void);
  virtual ~LeoMobilityModel ();

private:
  /**
   * \return the current position.
   *
   * Concrete subclasses of this base class must
   * implement this method.
   */
  virtual Vector DoGetPosition (void) const;
  /**
   * \param position the position to set.
   *
   * Concrete subclasses of this base class must
   * implement this method.
   */
  virtual void DoSetPosition (const Vector &position);
  /**
   * \return the current velocity.
   *
   * Concrete subclasses of this base class must
   * implement this method.
   */
  virtual Vector DoGetVelocity (void) const;
  /**
   * The default implementation does nothing but return the passed-in
   * parameter.  Subclasses using random variables are expected to
   * override this.
   * \param start  starting stream index
   * \return the number of streams used
   */
  virtual int64_t DoAssignStreams (int64_t start);
  };
};

#endif /* LEO_MOBILITY_MODEL_H */
