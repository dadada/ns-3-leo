/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef LEO_LAT_LONG_H
#define LEO_LAT_LONG_H

#include <iostream>

namespace ns3 {

class LeoLatLong
{
public:
  LeoLatLong ();
  LeoLatLong (double latitude, double longitude);
  virtual ~LeoLatLong();

  double latitude;
  double longitude;
};

std::ostream &operator << (std::ostream &os, const LeoLatLong &latLong);
std::istream &operator >> (std::istream &is, LeoLatLong &latLong);

};

#endif
