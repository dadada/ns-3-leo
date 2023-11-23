/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef LEO_STARLINK_CONSTANTS
#define LEO_STARLINK_CONSTANTS

namespace ns3 {

/**
 * \ingroup leo
 * \defgroup constants Channel
 */
/**
 * \ingroup constants
 * \defgroup starlink
 */
/**
 * \ingroup starlink
 * \brief Constants for starlink network estimated from channel parameters
 *
 * Source http://systemarchitect.mit.edu/docs/delportillo18b.pdf
 *
 */

#define LEO_STARLINK_FREQUENCY        28.5           // GHz
#define LEO_STARLINK_BANDWIDTH        0.5            // GHz
#define LEO_STARLINK_TX_ANTENNA_D     3.5            // m
#define LEO_STARLINK_EIRP             68.4           // dBW
#define LEO_STARLINK_MODCOD           "256APSK 3/4"  // -
#define LEO_STARLINK_ROLL_OFF_FACTOR  0.1            // -
#define LEO_STARLINK_SPECTRAL_EFF     5.4            // bps/Hz
#define LEO_STARLINK_PATH_DISTANCE    1684           // km
#define LEO_STARLINK_ELEVATION_ANGLE  40             // deg
#define LEO_STARLINK_FSPL             186.1          // dB
#define LEO_STARLINK_ATMOSPHERIC_LOSS 2.9            // dB
#define LEO_STARLINK_RX_ANTENNA_GAIN  40.9           // dBi
#define LEO_STARLINK_SYSTEM_TEMP      535.9          // K
#define LEO_STARLINK_G_T              13.6           // dB/K
#define LEO_STARLINK_RX_C_N0          32.4           // dB
#define LEO_STARLINK_RX_C_ACI         27             // dB
#define LEO_STARLINK_RX_C_ASI         27             // dB
#define LEO_STARLINK_RX_C_XPI         25             // dB
#define LEO_STARLINK_HPA_C_3IM        30             // dB
#define LEO_STARLINK_RX_EB_N0_I0      13.3           // dB
#define LEO_STARLINK_REQ_EB_N0        12.3           // dB
#define LEO_STARLINK_LINK_MARGIN      1.02           // dB
#define LEO_STARLINK_DATA_RATE        2682.1         // Mbps
#define LEO_STARLINK_SHANNON_LIMIT    1.06           // dB

};

#endif
