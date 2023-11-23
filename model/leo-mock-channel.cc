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
    .AddAttribute ("ChannelType",
                   "The type of the channel",
                   EnumValue (),
                   MakeEnumAccessor (&LeoMockChannel::m_channelType),
                   MakeEnumChecker (
                     ChannelType::GW, "ns3::LeoMockChannel::ChannelType::GW",
                     ChannelType::UT, "ns3::LeoMockChannel::ChannelType::UT"))
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

  if (srcDev == nullptr)
    {
      NS_LOG_ERROR ("Dropping packet: invalid source device");
      return false;
    }

  Ptr<LeoMockNetDevice> dstDev = DynamicCast<LeoMockNetDevice> (GetDevice (dst));
  bool isBroadcast = (srcDev->GetBroadcast () == dst);
  if (dstDev == nullptr && !isBroadcast)
    {
      NS_LOG_ERROR ("Dropping packet: invalid destination device");
      return false;
    }

  LeoMockNetDevice::DeviceType srcType = srcDev->GetDeviceType ();

  switch (m_channelType)
    {
    case LeoMockChannel::ChannelType::GW:
      if (srcType == LeoMockNetDevice::GW)
      	{
          // Get device from SAT_GW set
          return true;
      	}
      else if (srcType == LeoMockNetDevice::SAT_GW)
      	{
          // Get device from GW set
          return true;
      	}
      break;
    case LeoMockChannel::ChannelType::UT:
      if (srcType == LeoMockNetDevice::UT)
      	{
          // Get device from SAT_UT set
          return true;
      	}
      else if (srcType == LeoMockNetDevice::SAT_UT)
      	{
          // Get device from UT set
          return true;
      	}
      break;
    // Other kinds of transmissions are not allowed
    default:
      NS_LOG_ERROR ("Dropping packet: invalid channel type");
      return false;
    }

  // Apply propagation loss and schedule transmission


  // Get propagation delay
  Time delay = GetDelay (srcDev, dstDev, txTime);

  // Make compiler happy
  return false;
}

}; // namespace ns3
