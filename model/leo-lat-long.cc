/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "leo-lat-long.h"

namespace ns3 {

std::ostream &operator << (std::ostream &os, const LeoLatLong &l)
{
  os << l.label << "," << l.latitude << "," << l.longitude;
  return os;
}

std::istream &operator >> (std::istream &is, LeoLatLong &l)
{
  char c1, c2;
  is >> l.label >> c1 >> l.latitude >> c2 >> l.longitude;
  if (c1 != ',' ||
      c2 != ',')
    {
      is.setstate (std::ios_base::failbit);
    }
  return is;
}

LeoLatLong::LeoLatLong () : latitude (0), longitude (0) {}
LeoLatLong::LeoLatLong (double la, double lo) : latitude (la), longitude (lo) {}
LeoLatLong::LeoLatLong (std::string label, double la, double lo) : latitude (la), longitude (lo) {}
LeoLatLong::~LeoLatLong () {}

};
