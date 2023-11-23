#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "../model/leo-mock-net-device.h"

#include "nd-cache-helper.h"

namespace ns3
{

void
NdCacheHelper::Install (NetDeviceContainer &devices, Ipv6InterfaceContainer &interfaces) const
{
  // prepare NDS cache
  for (uint32_t i = 0; i < devices.GetN (); i++)
    {
      Ptr<NetDevice> dev = devices.Get (i);
      Ptr<Node> node = dev->GetNode ();
      Ptr<Ipv6L3Protocol> ipv6 = node->GetObject<Ipv6L3Protocol> ();
      int32_t ifIndex = ipv6->GetInterfaceForDevice (dev);
      Ptr<Ipv6Interface> interface = ipv6->GetInterface (ifIndex);
      Ptr<NdiscCache> cache = interface->GetNdiscCache ();
      for (uint32_t j = 0; j < devices.GetN (); j++)
        {
          // every other device, that is not of same "type"
          Ptr<NetDevice> otherDevice = devices.Get (j);
          Ptr<LeoMockNetDevice> leoDev = DynamicCast<LeoMockNetDevice> (dev);
          Ptr<LeoMockNetDevice> otherLeoDev = DynamicCast<LeoMockNetDevice> (otherDevice);
          if (i == j || (leoDev != 0
              && otherLeoDev != 0
              && leoDev->GetDeviceType () == otherLeoDev->GetDeviceType ()))
            {
              continue;
            }
          Address address = otherDevice->GetAddress (); // MAC

          // and associated address
          uint32_t otherIfIndex = otherDevice->GetIfIndex ();
          Ipv6Address ipaddr = interfaces.GetAddress (otherIfIndex, 1); // IP

          // update cache
          NdiscCache::Entry* entry = cache->Lookup (ipaddr);
          if (entry == 0)
            {
              entry = cache->Add (ipaddr);
            }
          entry->SetMacAddress (address);
        }
    }
}

}; /* namespace ns3 */
