/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/node-container.h"

#include "ns3/leo-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("IslExample");

void PrepareNds (NodeContainer &nodes, Ipv6InterfaceContainer &interfaces)
{
  // prepare NDS cache
  for (uint32_t i = 0; i < nodes.GetN (); i++)
    {
      Ptr<Node> node = nodes.Get (i);
      Ptr<NetDevice> dev = node->GetDevice (1); // TODO right index?
      uint32_t ifIndex = dev->GetIfIndex ();
      Ptr<Ipv6L3Protocol> ipv6 = node->GetObject<Ipv6L3Protocol> ();
      Ptr<Ipv6Interface> interface = ipv6->GetInterface (ifIndex);
      Ptr<NdiscCache> cache = interface->GetNdiscCache ();
      for (uint32_t j = 0; j < nodes.GetN (); j++)
        {
          // every other device
          if (i == j)
            {
              continue;
            }
          Ptr<NetDevice> otherDevice = nodes.Get (j)->GetDevice (1);
          Address address = otherDevice->GetAddress (); // MAC

          // and associated address
          uint32_t otherIfIndex = otherDevice->GetIfIndex ();
          // TODO which address to use? site-local, global, link?
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

int
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);

  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("IslExample", LOG_LEVEL_DEBUG);

  NodeContainer nodes;
  nodes.Create (3);

  IslHelper isl;
  isl.SetDeviceAttribute ("DataRate", StringValue ("5Gbps"));
  isl.SetChannelAttribute ("PropagationDelay", StringValue ("ns3::ConstantSpeedPropagationDelayModel"));
  isl.SetChannelAttribute ("PropagationLoss", StringValue ("ns3::IslPropagationLossModel"));
  isl.SetDeviceAttribute ("MobilityModel", StringValue ("ns3::LeoMobilityModel"));

  NetDeviceContainer devices;
  devices = isl.Install (nodes);

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv6AddressHelper address;

  Ipv6InterfaceContainer interfaces = address.Assign (devices);

  PrepareNds (nodes, interfaces);

  UdpEchoServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (nodes);

  ApplicationContainer clientApps;
  UdpEchoClientHelper echoClient (devices.Get (0)->GetAddress (), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (10));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  for (uint32_t i = 1; i < nodes.GetN (); i++)
    {
      Address destAddress = interfaces.GetAddress (i, 0);
      echoClient.SetAttribute ("RemoteAddress", AddressValue (destAddress));

      clientApps.Add (echoClient.Install (nodes.Get (i-1)));
    }

  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
