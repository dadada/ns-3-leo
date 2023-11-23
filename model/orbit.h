/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef LEO_ORBIT_H
#define LEO_ORBIT_H

#include "ns3/uinteger.h"

class Orbit {
public:
  Orbit (double a, double i, double p, double s) : alt (a), inc (i), planes (p), sats (s) {}
  double alt;
  double inc;
  uint16_t planes;
  uint16_t sats;
};

#endif
