/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef LEO_GND_NODE_HELPER_H
#define LEO_GND_NODE_HELPER_H

#include <string>

#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/leo-lat-long.h"

#define LEO_GND_RAD_EARTH 6.371e6

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
   * \param file path to latitude longitude file
   * \returns a node container containing nodes using the specified attributes
   */
  NodeContainer Install (const std::string &file);

  /**
   *
   * \param numNodes a number of nodes to uniformly distribute accross earth
   * \returns a node container containing nodes using the specified attributes
   */
  NodeContainer Install (uint64_t numNodes);

  /**
   *
   * \param location1 first location
   * \param location2 second location
   * \returns a node container containing nodes using the specified attributes
   */
  NodeContainer Install (const LeoLatLong &location1,
  			 const LeoLatLong &location2);

  /**
   * Set an attribute for each node
   *
   * \param name name of the attribute
   * \param value value of the attribute
   */
  void SetAttribute (std::string name, const AttributeValue &value);

private:
  ObjectFactory m_gndNodeFactory;

  static Vector3D GetEarthPosition (const LeoLatLong &loc);
};

}; // namespace ns3

#endif
