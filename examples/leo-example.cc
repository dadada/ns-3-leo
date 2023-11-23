/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/node-container.h"
#include "ns3/core-module.h"

#include "../helper/leo-helper.h"

using namespace ns3;

int
main (int argc, char *argv[])
{
  bool verbose = true;

  CommandLine cmd;
  cmd.AddValue ("verbose", "Tell application to log if true", verbose);

  cmd.Parse (argc,argv);

  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);

  NodeContainer satellites;
  satellites.Create (100);
  NodeContainer gateways;
  gateways.Create (10);
  NodeContainer terminals;
  terminals.Create (2000);

  LeoHelper leo;
  leo.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  leo.SetChannelAttribute ("PropagationDelay", StringValue ("ns3::ConstantSpeedPropagationDelayModel"));
  leo.SetChannelAttribute ("PropagationLoss", StringValue ("ns3::IslPropagationLossModel"));
  leo.SetDeviceAttribute ("MobilityModel", StringValue ("ns3::LeoMobilityModel"));

  NetDeviceContainer devices;
  devices = leo.Install (satellites, gateways, terminals);

  InternetStackHelper stack;
  stack.Install (satellites);
  stack.Install (gateways);
  stack.Install (terminals);

  // make all devices addressable
  Ipv6AddressHelper address;
  Ipv6InterfaceContainer interfaces = address.Assign (devices);

  // we want to ping terminals
  UdpEchoServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (terminals);

  // install a client on each of the terminals
  ApplicationContainer clientApps;
  for (uint32_t i = 1; i < terminals.GetN (); i++)
    {
      UdpEchoClientHelper echoClient (terminals.Get (i)->GetDevice (0)->GetAddress (), 9);
      echoClient.SetAttribute ("MaxPackets", UintegerValue (10));
      echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
      echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

      clientApps.Add (echoClient.Install (terminals.Get (i-1)));
    }

  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
