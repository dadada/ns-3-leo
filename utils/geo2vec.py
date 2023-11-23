#!/usr/bin/env python3

import logging as log
import fileinput

from sys import argv
from skyfield.api import Topos, load

log.basicConfig(level=log.DEBUG)

"""
Converts pairs of format `name longitute latitude` to `ns3::Vector` format in ITRF frame

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
    if len(argv) > 1:
        f = fileinput.input(argv[1])
    else:
        f = fileinput.input()

    for line in f:
        _, lat, lng = line.split(',')
        location = Topos(float(lat), float(lng))
        d = location.itrf_xyz().m
        print(Vector(d[0], d[1], d[2]))

