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

#include <ns3/trace-source-accessor.h>
#include <ns3/packet.h>
#include <ns3/simulator.h>
#include <ns3/log.h>
#include <ns3/pointer.h>
#include "isl-mock-channel.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("IslMockChannel");

NS_OBJECT_ENSURE_REGISTERED (IslMockChannel);

TypeId
IslMockChannel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::IslMockChannel")
    .SetParent<MockChannel> ()
    .SetGroupName ("Leo")
    .AddConstructor<IslMockChannel> ()
  ;
  return tid;
}

//
// By default, you get a channel that
// has an "infitely" fast transmission speed and zero processing delay.
IslMockChannel::IslMockChannel() : MockChannel ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

IslMockChannel::~IslMockChannel()
{
}

bool IslMockChannel::Deliver (
    Ptr<const Packet> p,
    Ptr<MockNetDevice> src,
    Ptr<MockNetDevice> dst,
    Time txTime)
{
  NS_LOG_FUNCTION (this << p << src->GetAddress () << dst->GetAddress () << txTime);
  Time delay = GetDelay (src, dst, txTime);

  /* Check if there is LOS between the source and destination */
  if (GetPropagationLoss ()->CalcRxPower(1, src->GetMobilityModel(), dst->GetMobilityModel()) > 0)
  {
    Simulator::ScheduleWithContext (dst->GetNode ()->GetId (),
        delay,
        &MockNetDevice::Receive,
        dst,
        p->Copy (),
        src);

    // Call the tx anim callback on the net device
    m_txrxMock (p, src, dst, txTime, delay);
    return true;
  }
  else
  {
    NS_LOG_LOGIC (dst << " unreachable from " << src);

    return false;
  }
}

bool
IslMockChannel::TransmitStart (
    Ptr<const Packet> p,
    uint32_t srcId,
    Address destAddr,
    Time txTime)
{
  NS_LOG_FUNCTION (this << p << srcId << destAddr << txTime);
  NS_LOG_LOGIC ("UID is " << p->GetUid () << ")");

  Ptr<MockNetDevice> src = DynamicCast<MockNetDevice> (GetDevice (srcId));
  Ptr<MockNetDevice> dst = DynamicCast<MockNetDevice> (GetDevice (destAddr));

  if (dst == nullptr)
  {
    NS_LOG_LOGIC ("destination address " << destAddr << " unknown on channel");
    for (uint32_t i = 0; i < GetNDevices (); i++)
    {
      Deliver (p, src, DynamicCast<MockNetDevice> (GetDevice (i)), txTime);
    }
    return true;
  }
  else
  {
    return Deliver (p, src, dst, txTime);
  }
}

} // namespace ns3
