/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/node-container.h"

#ifndef NDS_CACHE_HELPER_
#define NDS_CACHE_HELPER_

namespace ns3 {

class NdCacheHelper
{
public:
  void Install (NetDeviceContainer &devices, Ipv6InterfaceContainer &interfaces) const;
  void Install (NetDeviceContainer &devicesSrc, NetDeviceContainer &devicesDst, Ipv6InterfaceContainer &interfaces) const;
};

}; /* namespace ns3 */

#endif /* NDS_CACHE_HELPER */
