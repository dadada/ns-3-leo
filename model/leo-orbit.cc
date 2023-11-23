/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "leo-orbit.h"

namespace ns3 {

std::ostream &operator << (std::ostream &os, const LeoOrbit &orbit)
{
  os << orbit.alt << ":" << orbit.inc << ":" << orbit.planes << ":" << orbit.sats;
  return os;
}

std::istream &operator >> (std::istream &is, LeoOrbit &orbit)
{
  char c1, c2, c3;
  is >> orbit.alt >> c1 >> orbit.inc >> c2 >> orbit.planes >> c3 >> orbit.sats;
  if (c1 != ':' ||
      c2 != ':' ||
      c3 != ':')
    {
      is.setstate (std::ios_base::failbit);
    }
  return is;
}

LeoOrbit::LeoOrbit () : alt (0), inc (0), planes (0), sats (0) {}
LeoOrbit::~LeoOrbit () {}

};
