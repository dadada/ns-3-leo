/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef LEO_PROPAGATION_LOSS_MODEL_H
#define LEO_PROPAGATION_LOSS_MODEL_H

#include <ns3/object.h>
#include <ns3/propagation-loss-model.h>

#define LEO_PROP_EARTH_RAD 6.37101e6

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
