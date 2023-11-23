#!/usr/bin/env python3

import logging as log
import argparse

from skyfield.api import Topos, load

log.basicConfig(level=log.DEBUG)

"""
Converts pairs of format `longitute latitude` to `ns3::Vector` format in ITRF frame

See `ns3::Vector::operator >>`
"""

"""
Stores the vector data
"""
class Vector:

    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

    def __repr__(self):
        return "%f:%f:%f" % (self.x, self.y, self.z)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Get the position of a position on earth')
    parser.add_argument('latitude', type=float)
    parser.add_argument('longitude', type=float)
    args = parser.parse_args()

    location = Topos(args.latitude, args.latitude)
    d = location.itrf_xyz().m
    print(Vector(d[0], d[1], d[2]))

