/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <fstream>

#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/leo-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("LeoCircularOrbitTracingExample");

ofstream outfile ("leo-circular-orbit-tracing-example.csv");

void CourseChange (std::string context, Ptr<const MobilityModel> position)
{
  Vector pos = position->GetPosition ();
  Ptr<const Node> node = position->GetObject<Node> ();
  outfile << Simulator::Now () << ":" << node->GetId () << ":" << pos.x << ":" << pos.y << ":" << pos.z << ":" << position->GetVelocity ().GetLength() << std::endl;
}

int main(int argc, char *argv[])
{
  for (double incl: { 70.0, 100.0 })
    {
      NodeContainer c;
      c.Create (600);

      MobilityHelper mobility;
      mobility.SetPositionAllocator ("ns3::LeoCircularOrbitPostionAllocator",
                                     "NumOrbits", IntegerValue (6),
                                     "NumSatellites", IntegerValue (100));
      mobility.SetMobilityModel ("ns3::LeoCircularOrbitMobilityModel",
  			     	 "Altitude", DoubleValue (1200.0),
  			     	 "Inclination", DoubleValue (incl),
  			     	 "Precision", TimeValue (Minutes (1)));
      mobility.Install (c);
    }

  Config::Connect ("/NodeList/*/$ns3::MobilityModel/CourseChange",
                   MakeCallback (&CourseChange));

  outfile << "Time,Satellite,x,y,z,Speed" << std::endl;

  Simulator::Stop (Hours (2.0));
  Simulator::Run ();
  Simulator::Destroy ();
}
