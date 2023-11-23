/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/leo-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("LeoCircularOrbitTracingExample");

void CourseChange (std::string context, Ptr<const MobilityModel> position)
{
  Vector pos = position->GetPosition ();
  Ptr<const Node> node = position->GetObject<Node> ();
  std::cout << Simulator::Now () << "," << node->GetId () << "," << pos.x << "," << pos.y << "," << pos.z << "," << position->GetVelocity ().GetLength() <<  std::endl;
}

int main(int argc, char *argv[])
{
  NodeContainer c;
  c.Create (600);

  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::LeoCircularOrbitPostionAllocator",
                                 "NumOrbits", IntegerValue (6),
                                 "NumSatellites", IntegerValue (100));
  mobility.SetMobilityModel ("ns3::LeoCircularOrbitMobilityModel",
  			     "Altitude", DoubleValue (1200.0),
  			     "Inclination", DoubleValue (80.0),
  			     "Precision", TimeValue (Minutes (1)));
  mobility.Install (c);

  Config::Connect ("/NodeList/*/$ns3::MobilityModel/CourseChange",
                   MakeCallback (&CourseChange));

  std::cout << "Time,Satellite,x,y,z,Speed" << std::endl;

  Simulator::Stop (Minutes (60.0));
  Simulator::Run ();
  Simulator::Destroy ();
}
