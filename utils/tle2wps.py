#!/usr/bin/env python3

"""
Creates a stream of waypoints using SGP4 and two-line element (TLE) information

Up-to-date TLEs can be obtained from [Celestrak](https://celestrak.com/satcat/search.php)
"""

import logging as log
import argparse

from datetime import datetime
from skyfield.api import EarthSatellite, Time, load, utc

log.basicConfig(level=log.DEBUG)

"""
Class for exporting `ns3::Waypoint` in its own serialized format.

See `ns3::Waypoint::operator <<`
"""
class Waypoint:
    def __init__(self, time, x, y, z):
        self.time = time
        self.x = x
        self.y = y
        self.z = z

    def __repr__(self):
        return("%fs $ %f:%f:%f" % (self.time, self.x, self.y, self.z))


"""
Use Skyfield to generate the waypoints
"""
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('tle', type=str, help='Two-line element data')
    parser.add_argument('year', type=int, help='year to start at')
    parser.add_argument('month', type=int, help='month to start at')
    parser.add_argument('day', type=int, help='day to start at')
    parser.add_argument('duration', type=int, help='duration in seconds')
    args = parser.parse_args()

    ts = load.timescale(builtin=True)
    sats = load.tle_file(args.tle)
    log.info("Loaded %d satellites" % len(sats))

    duration = args.duration

    for sat in sats:
        log.info("Generating waypoints for %s" % sat.name)
        # skyfield will automatically overflow the seconds and produce the next correct second
        t_start = ts.utc(year=args.year, month=args.month, day=args.day)
        for t in ts.utc(year=args.year, month=args.month, day=args.day, second=range(1, duration, 1)):
            geocentric = sat.at(t)
            t_rel = (t.tai - t_start.tai) * 10**5
            d = geocentric.itrf_xyz().m
            wp = Waypoint(t_rel, d[0], d[1], d[2])
            print(wp)

