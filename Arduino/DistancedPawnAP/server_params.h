/**
 * \file server_params.h
 * \brief Global parameters header to implement the web server and the software AP
 */

//! AP SSID
#define SECRET_SSID "MKR1010"
//! Wep password. Should be know by the remote client
#define SECRET_PASS "ChessMaster"

//! https custom server port
#define SERVER_PORT 8080

//! Delay before the AP can connect the WiFi (ms)
#define AP_DELAY 10000

//! Definition of the default AP IP address
inline int IP(int x) { int ip[] = {10, 0, 0, 1}; return ip[x]; }
