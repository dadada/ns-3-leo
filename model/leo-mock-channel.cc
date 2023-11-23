/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/enum.h"
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
                     ChannelType::GW_FORWARD, "ns3::LeoMockChannel::ChannelType::GW_FORWARD",
                     ChannelType::GW_RETURN, "ns3::LeoMockChannel::ChannelType::GW_RETURN",
                     ChannelType::UT_FORWARD, "ns3::LeoMockChannel::ChannelType::UT_FORWARD",
                     ChannelType::UT_RETURN, "ns3::LeoMockChannel::ChannelType::UT_RETURN"))
    .AddTraceSource ("TxRxLeoMockChannel",
                     "Trace source indicating transmission of packet "
                     "from the LeoMockChannel, used by the Animation "
                     "interface.",
                     MakeTraceSourceAccessor (&LeoMockChannel::m_txrxMock),
                     "ns3::LeoMockChannel::TxRxAnimationCallback")
  ;
  return tid;
}

//
// By default, you get a channel that
// has an "infitely" fast transmission speed and zero processing delay.
LeoMockChannel::LeoMockChannel() : MockChannel (), m_channelType (LeoMockChannel::ChannelType::UNKNOWN)
{
  NS_LOG_FUNCTION_NOARGS ();
}

LeoMockChannel::~LeoMockChannel()
{
}

bool
TransmitStart (Ptr<const Packet> p,
               uint32_t devId,
               Address dst, Time txTime)
{
  return false;
}

}; // namespace ns3
