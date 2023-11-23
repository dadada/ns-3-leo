/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef LEO_ORBIT_NODE_HELPER_H
#define LEO_ORBIT_NODE_HELPER_H

#include <string>

#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include "ns3/leo-circular-orbit-mobility-model.h"
#include "ns3/leo-circular-orbit-position-allocator.h"
#include "ns3/leo-orbit.h"

/**
 * \brief Builds a node container of nodes with LEO positions using a list of
 * orbit definitions.
 *
 * Adds orbits with from a file for each node.
 */

namespace ns3
{

class LeoOrbitNodeHelper
{
public:
  LeoOrbitNodeHelper ();
  virtual ~LeoOrbitNodeHelper ();

  /**
   *
   * \param orbitFile path to orbit definitions file
   * \returns a node container containing nodes using the specified attributes
   */
  NodeContainer Install (const std::string &orbitFile);

  /**
   *
   * \param orbits orbit definitions
   * \returns a node container containing nodes using the specified attributes
   */
  NodeContainer Install (const std::vector<LeoOrbit> &orbits);

  /**
   *
   * \param orbit orbit definition
   * \returns a node container containing nodes using the specified attributes
   */
  NodeContainer Install (const LeoOrbit &orbit);

  /**
   * Set an attribute for each node
   *
   * \param name name of the attribute
   * \param value value of the attribute
   */
  void SetAttribute (std::string name, const AttributeValue &value);

private:
  ObjectFactory m_nodeFactory;
};

}; // namespace ns3

#endif
