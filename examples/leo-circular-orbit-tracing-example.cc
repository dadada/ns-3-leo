/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <fstream>

#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/leo-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("LeoCircularOrbitTracingExample");

void CourseChange (std::string context, Ptr<const MobilityModel> position)
{
  Vector pos = position->GetPosition ();
  Ptr<const Node> node = position->GetObject<Node> ();
  std::cout << Simulator::Now () << ":" << node->GetId () << ":" << pos.x << ":" << pos.y << ":" << pos.z << ":" << position->GetVelocity ().GetLength() << std::endl;
}

int main(int argc, char *argv[])
{
  CommandLine cmd;
  std::string orbitFile;
  std::string traceFile;
  Time duration;
  cmd.AddValue("orbitFile", "CSV file with orbit parameters", orbitFile);
  cmd.AddValue("traceFile", "CSV file to store mobility trace in", traceFile);
  cmd.AddValue("precision", "ns3::LeoCircularOrbitMobilityModel::Precision");
  cmd.AddValue("duration", "Duration of the simulation", duration);
  cmd.Parse (argc, argv);

  LeoOrbitNodeHelper orbit;
  NodeContainer satellites = orbit.Install (orbitFile);

  Config::Connect ("/NodeList/*/$ns3::MobilityModel/CourseChange",
                   MakeCallback (&CourseChange));

  std::streambuf *coutbuf = std::cout.rdbuf();
  // redirect cout if traceFile is specified
  std::ofstream out;
  out.open (traceFile);
  if (out.is_open ())
    {
      std::cout.rdbuf(out.rdbuf());
    }

  std::cout << "Time,Satellite,x,y,z,Speed" << std::endl;

  Simulator::Stop (duration);
  Simulator::Run ();
  Simulator::Destroy ();

  out.close ();
  std::cout.rdbuf(coutbuf);
}
