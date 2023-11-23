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

#ifndef LEO_MOCK_CHANNEL_H_
#define LEO_MOCK_CHANNEL_H_

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
#include "leo-mock-net-device.h"
#include "mock-channel.h"

namespace ns3 {

/**
 * \ingroup network
 * \defgroup channel Channel
 */
/**
 * \ingroup channel
 * \brief Mocked satellite-gateway, satellite-terminal channel types
 *
 */
    // TODO make separate clases for ut and gw links
    // TODO make separate clases for ut and gw devices
class LeoMockChannel : public MockChannel
{
public:
  static TypeId GetTypeId (void);

  LeoMockChannel ();
  virtual ~LeoMockChannel ();

  /**
   * \see MockChannel::TransmitStart
   *
   * A packet is transmitted if the destination is reachable via the beam.
   */
  virtual bool TransmitStart (Ptr<const Packet> p, uint32_t devId, Address dst, Time txTime);

protected:
  enum ChannelType
  {
    GW,
    UT,
    UNKNOWN
  };

  ChannelType GetChannelType () const;

private:
  /**
   * \brief Type of the channel
   */
  ChannelType m_channelType;

  bool IsChannel (Ptr<LeoMockNetDevice> dstType, Ptr<LeoMockNetDevice> srcType, bool isBroadcast);

  typedef std::map<Address, Ptr<LeoMockNetDevice> > DeviceIndex;
  DeviceIndex m_gndDevs;
  DeviceIndex m_satDevs;

}; // class MockChannel

} // namespace ns3

#endif /* LEO_MOCK_CHANNEL_H */
