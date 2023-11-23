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
#include "isl-channel.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("IslChannel");

NS_OBJECT_ENSURE_REGISTERED (IslChannel);

TypeId
IslChannel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::IslChannel")
    .SetParent<Channel> ()
    .SetGroupName ("Leo")
    .AddConstructor<IslChannel> ()
    .AddAttribute ("PropagationDelay",
                   "A propagation delay model for the channel.",
                   PointerValue (),
                   MakePointerAccessor (&IslChannel::m_propagationDelay),
                   MakePointerChecker<PropagationDelayModel> ())
    .AddAttribute ("PropagationLoss",
                   "A propagation loss model for the channel.",
                   PointerValue (),
                   MakePointerAccessor (&IslChannel::m_propagationLoss),
                   MakePointerChecker<PropagationLossModel> ())
    .AddTraceSource ("TxRxIslChannel",
                     "Trace source indicating transmission of packet "
                     "from the IslChannel, used by the Animation "
                     "interface.",
                     MakeTraceSourceAccessor (&IslChannel::m_txrxIsl),
                     "ns3::IslChannel::TxRxAnimationCallback")
  ;
  return tid;
}

//
// By default, you get a channel that
// has an "infitely" fast transmission speed and zero processing delay.
IslChannel::IslChannel() : Channel (), m_link (0)
{
  NS_LOG_FUNCTION_NOARGS ();
}

IslChannel::~IslChannel()
{
}

bool
IslChannel::Detach (uint32_t deviceId)
{
  NS_LOG_FUNCTION (this << deviceId);
  if (deviceId < m_link.size ())
  {
    if (!m_link[deviceId]->IsLinkUp ())
    {
      NS_LOG_WARN ("IslChannel::Detach(): Device is already detached (" << deviceId << ")");
      return false;
    }

    m_link[deviceId]->NotifyLinkDown ();
  }
  else
  {
    return false;
  }
  return true;
}

int32_t
IslChannel::Attach (Ptr<IslNetDevice> device)
{
  NS_LOG_FUNCTION (this << device);
  NS_ASSERT (device != 0);
  m_link.push_back(device);
  return  m_link.size() - 1;
}

std::size_t
IslChannel::GetNDevices (void) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_link.size ();
}

Ptr<NetDevice>
IslChannel::GetDevice (std::size_t i) const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_link[i];
}

bool IslChannel::Deliver (
    Ptr<const Packet> p,
    Ptr<IslNetDevice> src,
    Ptr<IslNetDevice> dst,
    Time txTime)
{
  Time delay = GetDelay (src, dst, txTime);

  /* Check if there is LOS between the source and destination */
  if (m_propagationLoss->CalcRxPower(1, src->GetMobilityModel(), dst->GetMobilityModel()) > 0)
  {
    Simulator::ScheduleWithContext (dst->GetNode ()->GetId (),
        delay,
        &IslNetDevice::Receive,
        dst,
        p->Copy (),
        src);

    // Call the tx anim callback on the net device
    m_txrxIsl (p, src, dst, txTime, delay);
    return true;
  }
  else
  {
    NS_LOG_LOGIC (dst << " unreachable from " << src);

    return false;
  }
}

bool
IslChannel::TransmitStart (
    Ptr<const Packet> p,
    uint32_t srcId,
    Address destAddr,
    Time txTime)
{
  NS_LOG_FUNCTION (destAddr << this << p << srcId);
  NS_LOG_LOGIC ("UID is " << p->GetUid () << ")");

  Ptr<IslNetDevice> src = m_link[srcId];
  Ptr<IslNetDevice> dst = GetDevice (destAddr);

  if (dst == nullptr)
  {
    NS_LOG_LOGIC ("destination address " << destAddr << " unknown on channel");
    for (uint32_t i = 0; i < m_link.size (); i++)
    {
      Deliver (p, src, m_link[i], txTime);
    }
    return true;
  }
  else
  {
    return Deliver (p, src, dst, txTime);
  }
}

Time
IslChannel::GetDelay (Ptr<const IslNetDevice> src, Ptr<const IslNetDevice> dst, Time txTime) const
{
  NS_LOG_DEBUG ("Get delay from " << src << " to " << dst);

  Ptr<MobilityModel> modSrc = src->GetMobilityModel ();
  Ptr<MobilityModel> modDst = dst->GetMobilityModel ();

  Time propagationDelay = m_propagationDelay->GetDelay (modSrc, modDst);

  return txTime + propagationDelay;
}

// TODO optimize
Ptr<IslNetDevice>
IslChannel::GetDevice (Address &addr) const
{
  for (Ptr<IslNetDevice> dev : m_link)
  {
    if (dev->GetAddress () == addr)
    {
      return dev;
    }
  }

  return 0;
}

} // namespace ns3
