/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/config.h"
#include "leo-channel-helper.h"
#include "ns3/enum.h"
#include "ns3/queue.h"
#include "ns3/names.h"

#include "../model/leo-mock-channel.h"
#include "../model/leo-mock-net-device.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("LeoChannelHelper");

LeoChannelHelper::LeoChannelHelper ()
{
  m_gndQueueFactory.SetTypeId ("ns3::DropTailQueue<Packet>");
  m_satQueueFactory.SetTypeId ("ns3::DropTailQueue<Packet>");

  m_gndDeviceFactory.SetTypeId ("ns3::LeoMockNetDevice");
  m_gndDeviceFactory.Set ("DeviceType", EnumValue (LeoMockNetDevice::GND));

  m_satDeviceFactory.SetTypeId ("ns3::LeoMockNetDevice");
  m_satDeviceFactory.Set ("DeviceType", EnumValue (LeoMockNetDevice::SAT));

  m_channelFactory.SetTypeId ("ns3::LeoMockChannel");
}

void
LeoChannelHelper::SetQueue(ObjectFactory &factory,
			   std::string type,
             		   std::string n1, const AttributeValue &v1,
             		   std::string n2, const AttributeValue &v2,
             		   std::string n3, const AttributeValue &v3,
             		   std::string n4, const AttributeValue &v4)
{
  QueueBase::AppendItemTypeIfNotPresent (type, "Packet");

  factory.SetTypeId (type);
  factory.Set (n1, v1);
  factory.Set (n2, v2);
  factory.Set (n3, v3);
  factory.Set (n4, v4);
}

void
LeoChannelHelper::SetGndQueue (std::string type,
             		       std::string n1, const AttributeValue &v1,
             		       std::string n2, const AttributeValue &v2,
             		       std::string n3, const AttributeValue &v3,
             		       std::string n4, const AttributeValue &v4)
{
  SetQueue (m_gndQueueFactory, type, n1, v1, n2, v2, n3, v3, n4, v4);
}

void
LeoChannelHelper::SetSatQueue (std::string type,
             std::string n1, const AttributeValue &v1,
             std::string n2, const AttributeValue &v2,
             std::string n3, const AttributeValue &v3,
             std::string n4, const AttributeValue &v4)
{

  SetQueue (m_satQueueFactory, type, n1, v1, n2, v2, n3, v3, n4, v4);
}


void
LeoChannelHelper::SetGndDeviceAttribute (std::string name, const AttributeValue &value)
{
  m_gndDeviceFactory.Set (name, value);
}

void
LeoChannelHelper::SetSatDeviceAttribute (std::string name, const AttributeValue &value)
{
  m_satDeviceFactory.Set (name, value);
}

void
LeoChannelHelper::SetChannelAttribute (std::string name, const AttributeValue &value)
{
  m_channelFactory.Set (name, value);
}

void
LeoChannelHelper::EnablePcapInternal (std::string prefix, Ptr<NetDevice> nd, bool promiscuous, bool explicitFilename)
{
  //
  // All of the Pcap enable functions vector through here including the ones
  // that are wandering through all of devices on perhaps all of the nodes in
  // the system.  We can only deal with devices of type LeoMockNetDevice.
  //
  Ptr<LeoMockNetDevice> device = nd->GetObject<LeoMockNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("LeoChannelHelper::EnablePcapInternal(): Device " << device << " not of type ns3::LeoMockNetDevice");
      return;
    }

  PcapHelper pcapHelper;

  std::string filename;
  if (explicitFilename)
    {
      filename = prefix;
    }
  else
    {
      filename = pcapHelper.GetFilenameFromDevice (prefix, device);
    }

  Ptr<PcapFileWrapper> file = pcapHelper.CreateFile (filename, std::ios::out,
                                                     PcapHelper::DLT_PPP);
  pcapHelper.HookDefaultSink<LeoMockNetDevice> (device, "PromiscSniffer", file);
}

void
LeoChannelHelper::EnableAsciiInternal (
  Ptr<OutputStreamWrapper> stream,
  std::string prefix,
  Ptr<NetDevice> nd,
  bool explicitFilename)
{
  //
  // All of the ascii enable functions vector through here including the ones
  // that are wandering through all of devices on perhaps all of the nodes in
  // the system.  We can only deal with devices of type LeoMockNetDevice.
  //
  Ptr<LeoMockNetDevice> device = nd->GetObject<LeoMockNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("LeoChannelHelper::EnableAsciiInternal(): Device " << device <<
                   " not of type ns3::LeoMockNetDevice");
      return;
    }

  //
  // Our default trace sinks are going to use packet printing, so we have to
  // make sure that is turned on.
  //
  Packet::EnablePrinting ();

  //
  // If we are not provided an OutputStreamWrapper, we are expected to create
  // one using the usual trace filename conventions and do a Hook*WithoutContext
  // since there will be one file per context and therefore the context would
  // be redundant.
  //
  if (stream == 0)
    {
      //
      // Set up an output stream object to deal with private ofstream copy
      // constructor and lifetime issues.  Let the helper decide the actual
      // name of the file given the prefix.
      //
      AsciiTraceHelper asciiTraceHelper;

      std::string filename;
      if (explicitFilename)
        {
          filename = prefix;
        }
      else
        {
          filename = asciiTraceHelper.GetFilenameFromDevice (prefix, device);
        }

      Ptr<OutputStreamWrapper> theStream = asciiTraceHelper.CreateFileStream (filename);

      //
      // The MacRx trace source provides our "r" event.
      //
      asciiTraceHelper.HookDefaultReceiveSinkWithoutContext<LeoMockNetDevice> (device, "MacRx", theStream);

      //
      // The "+", '-', and 'd' events are driven by trace sources actually in the
      // transmit queue.
      //
      Ptr<Queue<Packet> > queue = device->GetQueue ();
      asciiTraceHelper.HookDefaultEnqueueSinkWithoutContext<Queue<Packet> > (queue, "Enqueue", theStream);
      asciiTraceHelper.HookDefaultDropSinkWithoutContext<Queue<Packet> > (queue, "Drop", theStream);
      asciiTraceHelper.HookDefaultDequeueSinkWithoutContext<Queue<Packet> > (queue, "Dequeue", theStream);

      // PhyRxDrop trace source for "d" event
      asciiTraceHelper.HookDefaultDropSinkWithoutContext<MockNetDevice> (device, "PhyRxDrop", theStream);

      return;
    }

  //
  // If we are provided an OutputStreamWrapper, we are expected to use it, and
  // to providd a context.  We are free to come up with our own context if we
  // want, and use the AsciiTraceHelper Hook*WithContext functions, but for
  // compatibility and simplicity, we just use Config::Connect and let it deal
  // with the context.
  //
  // Note that we are going to use the default trace sinks provided by the
  // ascii trace helper.  There is actually no AsciiTraceHelper in sight here,
  // but the default trace sinks are actually publicly available static
  // functions that are always there waiting for just such a case.
  //
  uint32_t nodeid = nd->GetNode ()->GetId ();
  uint32_t deviceid = nd->GetIfIndex ();
  std::ostringstream oss;

  oss << "/NodeList/" << nd->GetNode ()->GetId () << "/DeviceList/" << deviceid << "/$ns3::LeoMockNetDevice/MacRx";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultReceiveSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::LeoMockNetDevice/TxQueue/Enqueue";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultEnqueueSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::LeoMockNetDevice/TxQueue/Dequeue";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultDequeueSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::LeoMockNetDevice/TxQueue/Drop";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultDropSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::LeoMockNetDevice/PhyRxDrop";
  Config::Connect (oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultDropSinkWithContext, stream));
}

NetDeviceContainer
LeoChannelHelper::Install (std::vector<Ptr<Node> > &satellites, std::vector<Ptr<Node> > &stations)
{
  Ptr<LeoMockChannel> channel = m_channelFactory.Create<LeoMockChannel> ();

  NetDeviceContainer container;

  for (Ptr<Node> node : satellites)
  {
    Ptr<LeoMockNetDevice> dev = m_satDeviceFactory.Create<LeoMockNetDevice> ();
    dev->SetAddress (Mac48Address::Allocate ());
    node->AddDevice (dev);
    Ptr<Queue<Packet> > queue = m_satQueueFactory.Create<Queue<Packet> > ();
    dev->SetQueue (queue);
    dev->Attach (channel);
    container.Add (dev);
  }

  for (Ptr<Node> node : stations)
  {
    Ptr<LeoMockNetDevice> dev = m_gndDeviceFactory.Create<LeoMockNetDevice> ();
    dev->SetAddress (Mac48Address::Allocate ());
    node->AddDevice (dev);
    Ptr<Queue<Packet> > queue = m_gndQueueFactory.Create<Queue<Packet> > ();
    dev->SetQueue (queue);
    dev->Attach (channel);
    container.Add (dev);
  }

  return container;
}

NetDeviceContainer
LeoChannelHelper::Install (NodeContainer &satellites, NodeContainer &stations)
{
  std::vector<Ptr<Node> > satelliteNodes = std::vector<Ptr<Node> >(satellites.Begin(), satellites.End());
  std::vector<Ptr<Node> > stationNodes = std::vector<Ptr<Node> >(stations.Begin(), stations.End());
  return Install (satelliteNodes, stationNodes);
}

NetDeviceContainer
LeoChannelHelper::Install (std::vector<std::string> &satellites, std::vector<std::string> &stations)
{
  std::vector<Ptr<Node> > sats;
  std::vector<Ptr<Node> > stats;
  for (std::string name : satellites)
  {
    Ptr<Node> node = Names::Find<Node>(name);
    sats.push_back (node);
  }
  for (std::string name : stations)
  {
    Ptr<Node> node = Names::Find<Node>(name);
    stats.push_back (node);
  }

  return Install (sats, stats);
}

}; /* namespace ns3 */
