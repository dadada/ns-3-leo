/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef LEO_TELESAT_CONSTANTS
#define LEO_TELESAT_CONSTANTS

namespace ns3 {

/**
 * \ingroup leo
 * \defgroup constants Channel
 */
/**
 * \ingroup constants
 * \defgroup telesat
 */
/**
 * \ingroup starlink
 * \brief Constants for Telesat network estimated from channel parameters
 *
 * Source http://systemarchitect.mit.edu/docs/delportillo18b.pdf
 *
 */

#define LEO_TELESAT_FREQUENCY        28.5         // GHz
#define LEO_TELESAT_BANDWIDTH        2.1          // GHz
#define LEO_TELESAT_TX_ANTENNA_D     3.5          // m
#define LEO_TELESAT_EIRP             105.9        // dBm
#define LEO_TELESAT_MODCOD           "64APSK 3/4" // -
#define LEO_TELESAT_ROLL_OFF_FACTOR  0.1          // -
#define LEO_TELESAT_SPECTRAL_EFF     4.1          // bps/Hz
#define LEO_TELESAT_PATH_DISTANCE    2439         // km
#define LEO_TELESAT_ELEVATION_ANGLE  20           // deg
#define LEO_TELESAT_FSPL             189.3        // dB
#define LEO_TELESAT_ATMOSPHERIC_LOSS 4.8          // dB
#define LEO_TELESAT_RX_ANTENNA_GAIN  31.8         // dBi
#define LEO_TELESAT_SYSTEM_TEMP      868.4        // K
#define LEO_TELESAT_G_T              2.4          // dB/K
#define LEO_TELESAT_RX_C_N0          25.6         // dB
#define LEO_TELESAT_RX_C_ACI         27           // dB
#define LEO_TELESAT_RX_C_ASI         23.5         // dB
#define LEO_TELESAT_RX_C_XPI         25           // dB
#define LEO_TELESAT_HPA_C_3IM        25           // dB
#define LEO_TELESAT_RX_EB_N0_I0      11.4         // dB
#define LEO_TELESAT_REQ_EB_N0        11.0         // dB
#define LEO_TELESAT_LINK_MARGIN      0.36         // dB
#define LEO_TELESAT_DATA_RATE        9857.1       // Mbps
#define LEO_TELESAT_SHANNON_LIMIT    1.09         // dB

};

#endif
