/*
 * twsndef.h
 *
 * Commonly used definitions and macros throughout framework.
 *
 *  Created on: Mar 11, 2014
 *      Author: quannt
 */

#ifndef TWSNDEF_H_
#define TWSNDEF_H_

namespace twsn {

/* Type definitions */
typedef double distance_t;
typedef long moduleid_t;
typedef long macaddr_t;
typedef long netaddr_t;

enum ErrLevel {VERBOSE, DEBUG, INFO, WARNING, ERROR};

/* Constants */
const macaddr_t MAC_BROADCAST_ADDR = -1;
const netaddr_t NET_BROADCAST_ADDR = -1;
const double symbolPeriod = 16e-6; // 16us, corresponding to 4 bits over 250kbps

}  // namespace twsn

#endif /* TWSNDEF_H_ */
