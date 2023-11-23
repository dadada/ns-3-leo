/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef LEO_PROPAGATION_LOSS_MODEL_H
#define LEO_PROPAGATION_LOSS_MODEL_H

#include <ns3/object.h>
#include <ns3/propagation-loss-model.h>

namespace ns3 {

class LeoPropagationLossModel : public PropagationLossModel
{
public:
  static TypeId GetTypeId (void);
  LeoPropagationLossModel ();
  virtual ~LeoPropagationLossModel ();

  static double GetAngle (Ptr<MobilityModel> a, Ptr<MobilityModel> b);

private:

  /**
   * Cutoff distance for signal
   */
  double m_cutoffDistance;

  /**
   * Cutoff angle for signal
   */
  double m_cutoffAngle;

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

#endif /* SATELLITE_LEO_PROPAGATION_LOSS_MODEL_H */
