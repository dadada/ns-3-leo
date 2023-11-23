/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/enum.h"

#include "leo-mock-net-device.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("LeoMockNetDevice");

NS_OBJECT_ENSURE_REGISTERED (LeoMockNetDevice);

TypeId
LeoMockNetDevice::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LeoMockNetDevice")
    .SetParent<MockNetDevice> ()
    .SetGroupName ("Leo")
    .AddConstructor<LeoMockNetDevice> ()
    .AddAttribute ("DeviceType", "The type of the mocked device",
                   EnumValue (),
                   MakeEnumAccessor (&LeoMockNetDevice::m_deviceType),
                   MakeEnumChecker (
                     DeviceType::GW, "ns3::LeoMockNetDevice::NetDeviceType::GW_FORWARD",
                     DeviceType::UT, "ns3::LeoMockNetDevice::NetDeviceType::GW_RETURN",
                     DeviceType::SAT_GW, "ns3::LeoMockNetDevice::NetDeviceType::UT_FORWARD",
                     DeviceType::SAT_UT, "ns3::LeoMockNetDevice::NetDeviceType::UT_RETURN"))
  ;
  return tid;
};

LeoMockNetDevice::LeoMockNetDevice ()
{
}

LeoMockNetDevice::DeviceType
LeoMockNetDevice::GetDeviceType () const
{
  return m_deviceType;
}

void
LeoMockNetDevice::SetDeviceType (LeoMockNetDevice::DeviceType deviceType)
{
  m_deviceType = deviceType;
}

}; /* namspace ns3 */
