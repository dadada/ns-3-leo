/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef LEO_MOCK_NET_DEVICE_H_
#define LEO_MOCK_NET_DEVICE_H_

#include "mock-net-device.h"

namespace ns3
{
  /**
   * \brief A mocked satellite-ground link communication device with a type
   */
class LeoMockNetDevice : public MockNetDevice
{
public:
  enum DeviceType
  {
    GND,
    SAT
  };

  static TypeId GetTypeId (void);

  LeoMockNetDevice ();

  virtual ~LeoMockNetDevice ()
  {
  }

  DeviceType GetDeviceType () const;
  void SetDeviceType (DeviceType deviceType);

protected:
  virtual double DoCalcRxPower (double rxPower) const;

private:
  DeviceType m_deviceType;
  double m_rxLoss;
  double m_rxGain;
};

};

#endif /* LEO_MOCK_NET_DEVICE_H_ */
