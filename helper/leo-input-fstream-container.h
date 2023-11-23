/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Tim Schubert <ns-3-leo@timschubert.net>
 */

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
