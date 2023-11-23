/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/node-container.h"
#include "ns3/core-module.h"

#include "ns3/leo-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("LeoExample");

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
  LogComponentEnable ("MockNetDevice", LOG_LEVEL_DEBUG);

  NodeContainer satellites;
  satellites.Create (100);
  NodeContainer gateways;
  gateways.Create (10);
  NodeContainer terminals;
  terminals.Create (20);

  LeoHelper leo;
  leo.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  leo.SetChannelAttribute ("PropagationDelay", StringValue ("ns3::ConstantSpeedPropagationDelayModel"));
  leo.SetChannelAttribute ("PropagationLoss", StringValue ("ns3::IslPropagationLossModel"));
  leo.SetDeviceAttribute ("MobilityModel", StringValue ("ns3::LeoMobilityModel"));

  leo.Install (satellites, gateways, terminals);

  // we want to ping terminals
  UdpEchoServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (terminals);

  // TODO routing

  // install a client on each of the terminals
  ApplicationContainer clientApps;
  for (uint32_t i = 1; i < terminals.GetN (); i++)
    {
      Address remote = terminals.Get (i)->GetObject<Ipv6> ()->GetAddress (1, 0).GetAddress ();
      NS_LOG_DEBUG ("REMOTE " << i << " " << remote);
      UdpEchoClientHelper echoClient (remote, 9);
      echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
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
