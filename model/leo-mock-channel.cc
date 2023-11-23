/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/enum.h"

#include "leo-mock-net-device.h"
#include "leo-mock-channel.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LeoMockChannel");

NS_OBJECT_ENSURE_REGISTERED (LeoMockChannel);

TypeId
LeoMockChannel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LeoMockChannel")
    .SetParent<Channel> ()
    .SetGroupName ("Leo")
    .AddConstructor<LeoMockChannel> ()
  ;
  return tid;
}

LeoMockChannel::LeoMockChannel() :
  MockChannel (),
  m_channelType (LeoMockChannel::ChannelType::UNKNOWN)
{
  NS_LOG_FUNCTION_NOARGS ();
}

LeoMockChannel::~LeoMockChannel()
{
}

bool
LeoMockChannel::TransmitStart (Ptr<const Packet> p,
               		       uint32_t devId,
               		       Address dst,
               		       Time txTime)
{
  // Find devices joined to channel
  Ptr<LeoMockNetDevice> srcDev = DynamicCast<LeoMockNetDevice> (GetDevice (devId));
  Ptr<LeoMockNetDevice> dstDev = DynamicCast<LeoMockNetDevice> (GetDevice (dst));

  bool isBroadcast = (srcDev->GetBroadcast () == dst);
  if (!isBroadcast && dstDev == nullptr)
    {
      NS_LOG_LOGIC (this << "Dropping packet: direct communication between ground stations and satellites not allowed using this channel");
      return false;
    }

  for (uint32_t i = 0; i < GetNDevices (); i++)
    {
      if (srcDev->GetDeviceType () != dstDev->GetDeviceType ())
      	{
      	  Deliver (p, srcDev, dstDev, txTime);
      	}
    }

  return true;
}

}; // namespace ns3
