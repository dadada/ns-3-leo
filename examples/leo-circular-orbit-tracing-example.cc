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


class Orbit {
public:
  Orbit (double a, double i, double p, double s) : alt (a), inc (i), planes (p), sats (s) {}
  double alt;
  double inc;
  uint16_t planes;
  uint16_t sats;
};

int main(int argc, char *argv[])
{
  std::vector<Orbit> orbits = {
      Orbit (1.150, 53.0, 32, 50),
      Orbit (1.110, 53.8, 32, 50),
      Orbit (1.130, 74.0,  8, 50),
      Orbit (1.275, 81, 5, 75),
      Orbit (1.325, 70, 6, 75),
  };
  NodeContainer satellites;
  for (Orbit orb: orbits)
    {
      NodeContainer c;
      c.Create (orb.sats*orb.planes);

      MobilityHelper mobility;
      mobility.SetPositionAllocator ("ns3::LeoCircularOrbitPostionAllocator",
                                     "NumOrbits", IntegerValue (orb.planes),
                                     "NumSatellites", IntegerValue (orb.sats));
      mobility.SetMobilityModel ("ns3::LeoCircularOrbitMobilityModel",
  			     	 "Altitude", DoubleValue (orb.alt),
  			     	 "Inclination", DoubleValue (orb.inc),
  			     	 "Precision", TimeValue (Minutes (1)));
      mobility.Install (c);
      satellites.Add (c);
    }


  Config::Connect ("/NodeList/*/$ns3::MobilityModel/CourseChange",
                   MakeCallback (&CourseChange));

  outfile << "Time,Satellite,x,y,z,Speed" << std::endl;

  Simulator::Stop (Hours (2.0));
  Simulator::Run ();
  Simulator::Destroy ();
}
