/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef SAT_NODE_HELPER_H
#define SAT_NODE_HELPER_H

#include <string>

#include "ns3/object-factory.h"
#include "ns3/node-container.h"

#include "ns3/leo-input-fstream-container.h"

/**
 * \brief Builds a node container with a waypoint mobility model
 *
 * Adds waypoints from file for each node.
 * The node satId must must correspond to the NORAD id from Celestrack.
 */

namespace ns3
{

class LeoSatNodeHelper
{
public:
  LeoSatNodeHelper ();
  virtual ~LeoSatNodeHelper ();

  /**
   *
   * \param nodeIds paths to satellite to waypoint files
   * \returns a node container containing nodes using the specified attributes
   */
  NodeContainer Install (std::vector<std::string> &wpFiles);

  /**
   * Set an attribute for each node
   *
   * \param name name of the attribute
   * \param value value of the attribute
   */
  void SetAttribute (std::string name, const AttributeValue &value);

private:
  ObjectFactory m_satNodeFactory;
  LeoWaypointInputFileStreamContainer m_fileStreamContainer;
};

}; // namespace ns3

#endif
