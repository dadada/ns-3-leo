/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef ARP_CACHE_HELPER_H
#define ARP_CACHE_HELPER_H

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/node-container.h"

namespace ns3 {

class ArpCacheHelper
{
public:
  void Install (NetDeviceContainer &devices, Ipv4InterfaceContainer &interfaces) const;
  void Install (NetDeviceContainer &devicesSrc, NetDeviceContainer &devicesDst, Ipv4InterfaceContainer &interfaces) const;
};

}; /* namespace ns3 */

#endif /* ARP_CACHE_HELPER_H */
