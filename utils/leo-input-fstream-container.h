/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef LEO_INPUT_FSTREAM_CONTAINER
#define LEO_INPUT_FSTREAM_CONTAINER

#include <fstream>
#include "ns3/object.h"
#include "ns3/waypoint.h"

using namespace std;

namespace ns3
{

class LeoWaypointInputFileStreamContainer : public Object
{
public:
  static TypeId GetTypeId (void);

  LeoWaypointInputFileStreamContainer ();
  virtual ~LeoWaypointInputFileStreamContainer ();

  LeoWaypointInputFileStreamContainer (string filePath, Time lastTime);

  bool GetNextSample (Waypoint &sample);

  void SetFile (const string path);
  string GetFile () const;

  void SetLastTime (const Time lastTime);
  Time GetLastTime () const;

private:
  string m_filePath;
  Time m_lastTime;
  ifstream m_input;

};

};

#endif
