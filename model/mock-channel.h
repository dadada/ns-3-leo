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
 */

#ifndef MOCK_CHANNEL_H
#define MOCK_CHANNEL_H

#include <string>
#include <stdint.h>

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/channel.h"
#include "ns3/mobility-model.h"
#include "ns3/net-device.h"
#include "ns3/time-data-calculators.h"
#include "ns3/traced-callback.h"
#include "ns3/mobility-module.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"
#include "mock-net-device.h"

namespace ns3 {

class MockNetDevice;

/**
 * \ingroup network
 * \defgroup channel Channel
 */
/**
 * \ingroup channel
 * \brief Mocked channel
 *
 */
class MockChannel : public Channel
{
public:
  static TypeId GetTypeId (void);

  MockChannel ();
  virtual ~MockChannel ();

  Ptr<NetDevice> GetDevice (std::size_t i) const;

  /**
   * \brief Attach a device to the channel.
   * \param device Device to attach to the channel
   * \return Index of the device inside the devices list
   */
  int32_t Attach (Ptr<MockNetDevice> device);

  /**
   * \brief Detach a given netdevice from this channel
   * \param device pointer to the netdevice to detach from the channel
   * \return true on success, false on failure
   */
  bool Detach (uint32_t deviceId);

  std::size_t GetNDevices (void) const;
  virtual bool TransmitStart (Ptr<const Packet> p, uint32_t devId, Address dst, Time txTime) = 0;

protected:
  TracedCallback<Ptr<const Packet>,     // Packet being transmitted
                 Ptr<NetDevice>,  // Transmitting NetDevice
                 Ptr<NetDevice>,  // Receiving NetDevice
                 Time,                  // Amount of time to transmit the pkt
                 Time                   // Last bit receive time (relative to now)
                 > m_txrxMock;

  /**
   * \brief Get the delay associated with this channel
   * \returns Time delay
   */
  Time GetDelay (Ptr<const MockNetDevice> first, Ptr<const MockNetDevice> second, Time txTime) const;

  Ptr<PropagationDelayModel> GetPropagationDelay () const;
  Ptr<PropagationLossModel> GetPropagationLoss () const;
  Ptr<MockNetDevice> GetDevice (Address &addr) const;
private:

  std::vector<Ptr<MockNetDevice> > m_link;

  /**
   * \brief Propagation delay model to be used with this channel
   */
  Ptr<PropagationDelayModel> m_propagationDelay;
  /**
   * \brief Propagation loss model to be used with this channel
   */
  Ptr<PropagationLossModel> m_propagationLoss;
}; // class MockChannel

} // namespace ns3

#endif /* MOCK_CHANNEL_H */
