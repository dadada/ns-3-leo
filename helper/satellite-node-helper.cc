#include "ns3/log.h"
#include "ns3/config.h"
#include "ns3/waypoint-mobility-model.h"

#include "satellite-node-helper.h"

using namespace std;

namespace ns3
{
NS_LOG_COMPONENT_DEFINE ("LeoSatNodeHelper");

LeoSatNodeHelper::LeoSatNodeHelper ()
{
  m_satNodeFactory.SetTypeId ("ns3::Node");
}

LeoSatNodeHelper::~LeoSatNodeHelper ()
{
}

void
LeoSatNodeHelper::SetAttribute (string name, const AttributeValue &value)
{
  m_satNodeFactory.Set (name, value);
}

NodeContainer
LeoSatNodeHelper::Install (vector<string> &wpFiles)
{
  NS_LOG_FUNCTION (this);

  NodeContainer nodes;
  for (size_t i = 0; i < wpFiles.size (); i ++)
    {
      Ptr<WaypointMobilityModel> mob = CreateObject<WaypointMobilityModel> ();
      string fileName = wpFiles[i];
      m_fileStreamContainer.SetFile (fileName);
      Waypoint wp;
      while (m_fileStreamContainer.GetNextSample (wp))
      	{
      	  mob->AddWaypoint (wp);
          NS_LOG_DEBUG ("Added waypoint " << wp);
      	}
      Ptr<Node> node = m_satNodeFactory.Create<Node> ();
      node->AggregateObject (mob);

      nodes.Add (node);
      NS_LOG_INFO ("Added satellite node " << node->GetId ());
    }

  return nodes;
}

}; // namespace ns3
