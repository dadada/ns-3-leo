Example Module Documentation
----------------------------

.. include:: replace.txt
.. highlight:: cpp

.. heading hierarchy:
   ------------- Chapter
   ************* Section (#.#)
   ============= Subsection (#.#.#)
   ############# Paragraph (no number)

Model Description
*****************

The source code for the new module lives in the directory ``contrib/leo``.

This module provides a mobility model for LEO satellites, propagation loss
models for satellite to satellite and satellite to ground transmission.  It
also includes a simplistic model of the inter-satellite and satellite-ground
channels and the associated network devices. It also contains required helpers
to build a LEO satellite network based on configurable parameters or import
mobility data from TLE files.

Design
======

The models do not depend on each other, with the exception of
``LeoMockChannel``, which requires the participating devices to be
``MockNetDevices``, and may be used outside of the context of this module.

Scope and Limitations
=====================

There are some limitations to the precision of the model: The circular movement
model does not take into the earths gravitational model or amospheric drag. The
link parameters are only modelled using their statistical estimates. There is
currently no support for multiple antennas per link. MAC layer behaviour or
frequency coordination are currently not modelled, but any existing MAC model
may be combined with the models provided by this module.

References
==========

See the project thesis report for more details.

Usage
*****

Interaction with the mobility model is mostly at configuration time using the
associated helpers.  At simulation time, various parts of |ns3| may interact
with the mobility model and propagation loss models to obtain the position,
heading, speed of nodes and the path loss, including if a link can be
established between two nodes. 

Topologies may be constructed using ``MockDevice``s and ``MockChannels``.

Helpers
=======

Since the configuration of these large network can be a large task with many
parameters and configuration steps, the helpers try to collect common
configutation steps, such as setting up the LEO and ISL channels and adding
devices with the correct mobility patterns based on the constellation.

A typical usage for the position allocator and the mobility model would be through ``MobilityHelper``.

.. sourcecode:: cpp

  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::LeoCircularOrbitPostionAllocator",
                                "NumOrbits", IntegerValue (32),
                                "NumSatellites", IntegerValue (50));
  mobility.SetMobilityModel ("ns3::LeoCircularOrbitMobilityModel",
                            "Altitude", DoubleValue (1200),
                            "Inclination", DoubleValue (40),
                            "Precision", TimeValue (Minutes (1)));

To configure the ground nodes, positions can be pre-defined and loaded into the simulation or generated using `PolarGridPositionAllocator`.

.. sourcecode:: cpp

  LeoGndNodeHelper ground;
  NodeContainer stations = ground.Install ("contrib/leo/data/ground-stations/usa-60.waypoints");

  MobilityHelper mobility;

.. sourcecode:: cpp

  mobility.SetPositionAllocator ("ns3::PolarGridPositionAllocator",
    "MinX", DoubleValue (0.0),
    "MinY", DoubleValue (0.0),
    "DeltaX", DoubleValue (5.0),
    "DeltaY", DoubleValue (10.0),
    "GridWidth", UintegerValue (3),
    "LayoutType", StringValue ("RowFirst"));

The ISL network can be configured using `IslHelper`

.. sourcecode:: cpp

  IslHelper islCh;
  islCh.SetDeviceAttribute ("DataRate", StringValue ("1Gbps"));
  islCh.SetChannelAttribute ("PropagationDelay", StringValue ("ns3::ConstantSpeedPropagationDelayModel"));
  islCh.SetChannelAttribute ("PropagationLoss", StringValue ("ns3::IslPropagationLossModel"));
  islNet = islCh.Install (satellites);

To select the parameters for a pre-defined constellation and construct a LEO
channel from it us the `LeoChannelHelper` and connect the satellites and ground
stations to it.

.. sourcecode:: cpp

  LeoChannelHelper utCh;
  utCh.SetConstellation ("StarlinkUser");
  utNet = utCh.Install (satellites, stations);

Output
======

The module itself does only provide trace sources for the transmission /
droppping of packets using ``MockNetDevice``. All other trace infomation
originating from this module is written to the trace sources associated with
the parent models (such as ``MobilityModel``).

Examples
========

leo-circlular-orbit
###################

The program configures a ``LeoCircularOrbitMobilityModel`` using the provided
parameters and logs all course changes to a CSV file.  The file format for the
orbit configuration file is ``altitude in km,inclination in deg,number of
planes,number of satellites per plane``. The duration is given in seconds.

.. sourcode::bash

  $ ./waf --run "leo-orbit \
  --orbitFile=contrib/leo/data/orbits/starlink.csv \
  --duration=360.0 \
  --precision=1.0 \
  --traceFile=out.csv"

leo-delay
#########

The delay tracing example uses `UdpServer` and `UdpClient` to measure the delay
and packet loss on between two nodes.  The source and destination locations are
given as pairs of longitude and latitude.

.. sourcode::bash

  $ ./waf --run "leo-delay \
  --orbitFile=contrib/leo/data/orbits/starlink.csv \
  --traceFile=out.csv
  --precision=1.0 \
  --duration=360.0 \
  --numGws=120 \
  --source=54.4:77.1 \
  --destination=40.58:-74.97 \
  --islRate=1Gbps \
  --constellation="StarlinkGateway" \
  --interval=1 \
  --ttlThresh=30 \
  --routeTimeout=0.25"

leo-throughput
##############

The throughput tracing example uses ``BulSendHelper`` and ``PacketSinkHelper``
to measure the throughput inbetween two nodes. The throughput and the TCP
parameters are logged to the `traceFile`.

.. sourcode::bash

  $ ./waf --run leo-throughput \
  --orbitsFile=orbits.csv \
  --groundFile=ground-stations.csv \
  --traceFile=tcp-trace.csv \
  --source=54.4,77.1 \
  --destination=-10.0,25.8 \
  --islRate=1Gbps \
  --constellation="StarlinkGateway" \
  --duration=10.0 \
  --maxBytes=3GB

Troubleshooting
===============

Add any tips for avoiding pitfalls, etc.

Validation
**********

Much of the module is covered using tests. The evalutation of the module in
part of the project thesis.
