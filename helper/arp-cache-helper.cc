/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "../model/leo-mock-net-device.h"

#include "arp-cache-helper.h"

namespace ns3
{

void
ArpCacheHelper::Install (NetDeviceContainer &devices, Ipv4InterfaceContainer &interfaces) const
{
  for (size_t i = 0; i < devices.GetN (); i ++)
    {
      Ptr<NetDevice> dev = devices.Get (i);
      Ptr<Node> node = dev->GetNode ();
      Ptr<Ipv4L3Protocol> ipv4 = node->GetObject<Ipv4L3Protocol> ();
      int32_t ifIndex = ipv4->GetInterfaceForDevice (dev);
      Ptr<Ipv4Interface> interface = ipv4->GetInterface (ifIndex);
      Ptr<ArpCache> cache = interface->GetArpCache ();

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
          Ipv4Address ipaddr = interfaces.GetAddress (otherIfIndex, 1); // IP

          // update cache
          ArpCache::Entry* entry = cache->Lookup (ipaddr);
          if (entry == 0)
            {
              entry = cache->Add (ipaddr);
            }
          entry->SetMacAddress (address);
      	}
    }
}

};
