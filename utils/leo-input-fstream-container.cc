/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/string.h"
#include "ns3/time-data-calculators.h"
#include "leo-input-fstream-container.h"

namespace ns3
{

TypeId
LeoWaypointInputFileStreamContainer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LeoWaypointInputFileStreamContainer")
    .SetGroupName ("Leo")
    .SetParent<Object> ()
    .AddConstructor<LeoWaypointInputFileStreamContainer> ()
    .AddAttribute ("File",
    		   "The path to the file opened in the stream",
    		   StringValue (),
    		   MakeStringAccessor (&LeoWaypointInputFileStreamContainer::m_filePath),
    		   MakeStringChecker ())
    .AddAttribute ("LastTime",
    		   "The minimum point in time for returned samples",
    		   TimeValue (),
    		   MakeTimeAccessor (&LeoWaypointInputFileStreamContainer::m_lastTime),
    		   MakeTimeChecker ())
    ;
  return tid;
}

LeoWaypointInputFileStreamContainer::LeoWaypointInputFileStreamContainer () :
  m_filePath (),
  m_lastTime (0)
{
}

LeoWaypointInputFileStreamContainer::LeoWaypointInputFileStreamContainer (string filePath, Time lastTime) :
  m_filePath (filePath),
  m_lastTime (lastTime)
{
}

LeoWaypointInputFileStreamContainer::~LeoWaypointInputFileStreamContainer ()
{
  m_input.close ();
}

bool
LeoWaypointInputFileStreamContainer::GetNextSample (Waypoint &sample)
{
  if (!m_input.is_open ())
    {
      m_input.open (m_filePath);
    }

  if (!m_input.is_open ())
    {
      NS_ABORT_MSG ("Input stream is not open");
    }

  if (m_input.bad ())
    {
      NS_ABORT_MSG ("Input stream is bad");
    }

  sample.time = Time (0);
  sample.position = Vector (0.0, 0.0, 0.0);
  bool updated = false;
  while (sample.time <= m_lastTime && (m_input >> sample))
    {
      updated = true;
    }
  if (updated)
    {
      m_lastTime = sample.time;
    }

  return updated;
}

void
LeoWaypointInputFileStreamContainer::SetFile (const string path)
{
  m_input.close ();
  m_filePath = path;
  m_input.open (m_filePath);
}

string
LeoWaypointInputFileStreamContainer::GetFile () const
{
  return m_filePath;
}

void
LeoWaypointInputFileStreamContainer::SetLastTime (const Time lastTime)
{
  m_input.clear ();
  m_input.seekg (0, std::ios::beg);
  m_lastTime = lastTime;
}

Time
LeoWaypointInputFileStreamContainer::GetLastTime () const
{
  return m_lastTime;
}

};
