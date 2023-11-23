/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef LEO_CANNEL_HELPER_H
#define LEO_CANNEL_HELPER_H

#include <string>

#include <ns3/object-factory.h>
#include <ns3/net-device-container.h>
#include <ns3/node-container.h>

#include <ns3/trace-helper.h>

namespace ns3 {

/**
 * \brief Build a channel for transmissions between ns3::LeoMockNetDevice s
 */
class LeoChannelHelper : public PcapHelperForDevice,
	                 public AsciiTraceHelperForDevice
{
public:
  LeoChannelHelper ();
  LeoChannelHelper (std::string constellation);
  virtual ~LeoChannelHelper ()
    {};

  NetDeviceContainer Install (NodeContainer &satellites, NodeContainer &stations);
  NetDeviceContainer Install (std::vector<Ptr<Node> > &satellites, std::vector<Ptr<Node> > &stations);
  NetDeviceContainer Install (std::vector<std::string> &satellites, std::vector<std::string> &stations);

  void SetGndQueue (std::string type,
                    std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                    std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                    std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                    std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue ());

  void SetSatQueue (std::string type,
                    std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                    std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                    std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                    std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue ());

  void SetGndDeviceAttribute (std::string name, const AttributeValue &value);
  void SetSatDeviceAttribute (std::string name, const AttributeValue &value);
  void SetChannelAttribute (std::string name, const AttributeValue &value);

  virtual void EnablePcapInternal (std::string prefix, Ptr<NetDevice> nd, bool promiscuous, bool explicitFilename);
  virtual void EnableAsciiInternal (Ptr<OutputStreamWrapper> stream,
  				    std::string prefix,
  				    Ptr<NetDevice> nd,
  				    bool explicitFilename);

  void SetConstellation (std::string constellation);

private:
  ObjectFactory m_satQueueFactory;
  ObjectFactory m_gndDeviceFactory;

  ObjectFactory m_satDeviceFactory;
  ObjectFactory m_gndQueueFactory;

  ObjectFactory m_channelFactory;

  ObjectFactory m_propagationLossFactory;

  void SetQueue (ObjectFactory &factory,
	   	 std::string type,
           	 std::string n1, const AttributeValue &v1,
           	 std::string n2, const AttributeValue &v2,
           	 std::string n3, const AttributeValue &v3,
           	 std::string n4, const AttributeValue &v4);


  void SetConstellationAttributes (double eirp,
				   double elevationAngle,
				   double fspl,
				   double atmosphericLoss,
				   double linkMargin,
				   double dataRate,
				   double rxGain,
				   double rxLoss);
};

};

#endif /* LEO_CHANNEL_HELPER_H */
