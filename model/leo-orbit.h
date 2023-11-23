/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef LEO_ORBIT_H
#define LEO_ORBIT_H

#include "ns3/uinteger.h"

namespace ns3
{
class LeoOrbit;

std::ostream &operator << (std::ostream &os, const LeoOrbit &orbit);
std::istream &operator >> (std::istream &is, LeoOrbit &orbit);

class LeoOrbit {
public:
  LeoOrbit ();
  LeoOrbit (double a, double i, double p, double s) : alt (a), inc (i), planes (p), sats (s) {}
  virtual ~LeoOrbit ();
  double alt;
  double inc;
  uint16_t planes;
  uint16_t sats;
};

};

#endif
