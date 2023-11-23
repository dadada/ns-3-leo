/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/node-container.h"

#include "ns3/leo-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("IslExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);

  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpClient", LOG_LEVEL_DEBUG);
  LogComponentEnable ("IslChannel", LOG_LEVEL_LOGIC);

  NodeContainer nodes;
  nodes.Create (2);

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

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  Address destAddress = interfaces.GetAddress (1, 0);
  UdpEchoClientHelper echoClient (destAddress, 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (10));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
