/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef LEO_ONEWEB_CONSTANTS
#define LEO_ONEWEB_CONSTANTS

namespace ns3 {

/**
 * \ingroup leo
 * \defgroup constants Channel
 */
/**
 * \ingroup constants
 * \defgroup oneweb
 */
/**
 * \ingroup oneweb
 * \brief Constants for oneweb network estimated from channel parameters
 *
 * Source http://systemarchitect.mit.edu/docs/delportillo18b.pdf
 *
 */

#define LEO_ONEWEB_FREQUENCY        28.5            // GHz
#define LEO_ONEWEB_BANDWIDTH        0.25            // GHz
#define LEO_ONEWEB_TX_ANTENNA_D     2.4             // m
#define LEO_ONEWEB_EIRP             63.2            // dBW
#define LEO_ONEWEB_MODCOD           "256APSK 32/45" // -
#define LEO_ONEWEB_ROLL_OFF_FACTOR  0.1             // -
#define LEO_ONEWEB_SPECTRAL_EFF     5.1             // bps/Hz
#define LEO_ONEWEB_PATH_DISTANCE    1504            // km
#define LEO_ONEWEB_ELEVATION_ANGLE  55              // deg
#define LEO_ONEWEB_FSPL             185.1           // dB
#define LEO_ONEWEB_ATMOSPHERIC_LOSS 2.3             // dB
#define LEO_ONEWEB_RX_ANTENNA_GAIN  37.8            // dBi
#define LEO_ONEWEB_SYSTEM_TEMP      447.2           // K
#define LEO_ONEWEB_G_T              11.3            // dB/K
#define LEO_ONEWEB_RX_C_N0          32.5            // dB
#define LEO_ONEWEB_RX_C_ACI         27              // dB
#define LEO_ONEWEB_RX_C_ASI         27              // dB
#define LEO_ONEWEB_RX_C_XPI         25              // dB
#define LEO_ONEWEB_HPA_C_3IM        30              // dB
#define LEO_ONEWEB_RX_EB_N0_I0      13.3            // dB
#define LEO_ONEWEB_REQ_EB_N0        12.3            // dB
#define LEO_ONEWEB_LINK_MARGIN      1.03            // dB
#define LEO_ONEWEB_DATA_RATE        1341.1          // Mbps
#define LEO_ONEWEB_SHANNON_LIMIT    1.06            // dB

};

#endif
