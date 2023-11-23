/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef ISL_PROPAGATION_LOSS_MODEL_H
#define ISL_PROPAGATION_LOSS_MODEL_H

#include <ns3/object.h>
#include <ns3/propagation-loss-model.h>

#define LEO_EARTH_RAD 6.371009e6

namespace ns3 {

class IslPropagationLossModel : public PropagationLossModel
{
public:
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
   * Cutoff distance for signal
   */
  double m_cutoffDistance;

  void SetCutoffDistance (double d);
  double GetCutoffDistance () const;

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
