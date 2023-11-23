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

#ifndef ISL_CHANNEL_H
#define ISL_CHANNEL_H

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
#include "mock-channel.h"

namespace ns3 {

class MockNetDevice;

/**
 * \ingroup network
 * \defgroup channel Channel
 */
/**
 * \ingroup channel
 * \brief Simplified inter-satellite channel
 *
 * A perfect channel with varariable delay (time-of-flight).
 *
 */
class IslMockChannel : public MockChannel
{
public:
  static TypeId GetTypeId (void);

  IslMockChannel ();
  virtual ~IslMockChannel ();

  bool TransmitStart (Ptr<const Packet> p, uint32_t devId, Address dst, Time txTime);

private:
  std::vector<Ptr<MockNetDevice> > m_link;


}; // class MockChannel

} // namespace ns3

#endif /* ISL_CHANNEL_H */
