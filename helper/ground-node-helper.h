/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef LEO_GND_NODE_HELPER_H
#define LEO_GND_NODE_HELPER_H

#include <string>

#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include "ns3/constant-position-mobility-model.h"

/**
 * \brief Builds a node container of nodes with constant positions
 *
 * Adds waypoints from file for each node.
 */

namespace ns3
{

class LeoGndNodeHelper
{
public:
  LeoGndNodeHelper ();
  virtual ~LeoGndNodeHelper ();

  /**
   *
   * \param wpFile path to waypoint file
   * \returns a node container containing nodes using the specified attributes
   */
  NodeContainer Install (const std::string &wpFile);

  /**
   * Set an attribute for each node
   *
   * \param name name of the attribute
   * \param value value of the attribute
   */
  void SetAttribute (std::string name, const AttributeValue &value);

private:
  ObjectFactory m_gndNodeFactory;
};

}; // namespace ns3

#endif
