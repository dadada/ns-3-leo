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
