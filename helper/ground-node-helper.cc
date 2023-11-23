#include <fstream>

#include "ns3/log.h"
#include "ns3/config.h"
#include "ns3/waypoint.h"

#include "ground-node-helper.h"

using namespace std;

namespace ns3
{
NS_LOG_COMPONENT_DEFINE ("LeoGndNodeHelper");

LeoGndNodeHelper::LeoGndNodeHelper ()
{
  m_gndNodeFactory.SetTypeId ("ns3::Node");
}

LeoGndNodeHelper::~LeoGndNodeHelper ()
{
}

void
LeoGndNodeHelper::SetAttribute (string name, const AttributeValue &value)
{
  m_gndNodeFactory.Set (name, value);
}

NodeContainer
LeoGndNodeHelper::Install (const std::string &wpFile)
{
  NS_LOG_FUNCTION (wpFile);

  NodeContainer nodes;
  ifstream waypoints;
  waypoints.open (wpFile, ifstream::in);
  Vector pos;
  while ((waypoints >> pos))
    {
      Ptr<ConstantPositionMobilityModel> mob = CreateObject<ConstantPositionMobilityModel> ();
      mob->SetPosition (pos);
      Ptr<Node> node = m_gndNodeFactory.Create<Node> ();
      node->AggregateObject (mob);
      nodes.Add (node);
      NS_LOG_INFO ("Added ground node at " << pos);
    }
  waypoints.close ();

  NS_LOG_INFO ("Added " << nodes.GetN () << " ground nodes");

  return nodes;
}

}; // namespace ns3
