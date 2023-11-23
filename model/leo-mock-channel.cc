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
    .SetParent<MockChannel> ()
    .SetGroupName ("Leo")
    .AddConstructor<LeoMockChannel> ()
  ;
  return tid;
}

LeoMockChannel::LeoMockChannel() :
  MockChannel ()
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
  Ptr<MockNetDevice> srcDev = DynamicCast<MockNetDevice> (GetDevice (devId));
  if (srcDev == 0)
    {
      NS_LOG_ERROR ("Source device unknown");
      return false;
    }

  // Hack for NDP and ARP caches
  // TODO check if NDP or ARP address
  bool isBroadcast = (m_groundDevices.find (dst) == m_groundDevices.end ()
                      && m_satelliteDevices.find (dst) == m_satelliteDevices.end ());

  bool fromGround = m_groundDevices.find (srcDev->GetAddress ()) != m_groundDevices.end ();

  if (isBroadcast)
    {
      NS_LOG_DEBUG (">>>broadcast for " << srcDev->GetAddress () << " -> " << dst);
      // Broadcast hack for ARP and neighbor cache
      // TODO remove if found a way to fill neighbor cache / ARP by hand
      if (fromGround)
        {
          for (DeviceIndex::iterator it = m_satelliteDevices.begin ();
               it != m_satelliteDevices.end ();
               it++)
            {
              // TODO deliver only to devices in the same beam
              NS_LOG_DEBUG ("from ground " << srcDev->GetAddress () << " -> " << it->second->GetAddress ());
              Deliver (p, srcDev, it->second, txTime);
            }
        }
      else
        {
          for (DeviceIndex::iterator it = m_groundDevices.begin ();
               it != m_groundDevices.end ();
               it++)
            {
              // TODO deliver only to devices in the same beam
              NS_LOG_DEBUG ("from space " << srcDev->GetAddress () << " -> " << it->second->GetAddress ());
              Deliver (p, srcDev, it->second, txTime);
            }
        }
    }
  else
    {
      if (fromGround)
        {
          // Satellites can always directly be addresses
          // Assume beams are narrow enough to not also receive the signal at other
          // satellites for performance reasons.
          DeviceIndex::iterator it = m_satelliteDevices.find (dst);
          if (it == m_satelliteDevices.end ())
            {
              NS_LOG_ERROR ("unable to find satellite with address " << dst);
              return false;
            }
          NS_LOG_DEBUG ("BOOOOM " << srcDev->GetAddress () << " -> " << dst);
          return Deliver (p, srcDev, it->second, txTime);
        }
      else
        // space to ground delivers to everything within the beam
        {
          for (DeviceIndex::iterator it = m_groundDevices.begin ();
               it != m_groundDevices.end ();
               it++)
            {
              // TODO deliver only to devices in the same beam
              Deliver (p, srcDev, it->second, txTime);
            }
        }
    }

  return true;
}

int32_t
LeoMockChannel::Attach (Ptr<MockNetDevice> device)
{
  Ptr<LeoMockNetDevice> leodev = DynamicCast<LeoMockNetDevice> (device);

  // Add to index
  switch (leodev->GetDeviceType ())
    {
    case LeoMockNetDevice::DeviceType::GND:
      m_groundDevices[leodev->GetAddress ()] = leodev;
      break;
    case LeoMockNetDevice::DeviceType::SAT:
      m_satelliteDevices[leodev->GetAddress ()] = leodev;
      break;
    default:
      break;
    }

  return MockChannel::Attach (device);
}

bool
LeoMockChannel::Detach (uint32_t deviceId)
{
  Ptr<NetDevice> dev = GetDevice (deviceId);
  m_groundDevices.erase (dev->GetAddress ());
  m_satelliteDevices.erase (dev->GetAddress ());

  return MockChannel::Detach (deviceId);
}

}; // namespace ns3
